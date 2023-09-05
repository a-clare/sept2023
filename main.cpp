#include <iostream>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "implot.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include "robot.h"

static GLFWwindow *window;
static Camera camera;
void ScrollCallback(GLFWwindow *win,
                    double xoffset,
                    double yoffset);
static void SetupWindow();

int main() {
  SetupWindow();
  camera.position_ = glm::vec3(0, 0, 10);
  camera.Update();

  Robot robot;
  robot.length_ = 1;
  robot.width_ = 0.5;
  robot.Init();

  bool simulation_running = false;
  double linear_velocity = 0;
  double angular_velocity = 0;
  const double linear_velocity_step = 0.1;
  const double angular_velocity_step = 0.5;
  const double simulation_dt = 0.01;
  while (!glfwWindowShouldClose(window)) {
    // Start of frame
    glfwPollEvents();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    if (ImGui::Begin("Simulation", NULL, ImGuiWindowFlags_None)) {

      ImGui::Text("W: + step linear velocity");
      ImGui::Text("S: - step linear velocity");
      ImGui::Text("A: - step angular velocity");
      ImGui::Text("D: + step angular velocity");
      ImGui::Text("R: Reset to 0");
      ImGui::BeginDisabled();
      ImGui::Checkbox("Simulation Started (space key)", &simulation_running);

      ImGui::InputDouble("Linear Velocity", &linear_velocity, 0, 0, "%.2f m/s");
      ImGui::InputDouble("Angular Velocity", &angular_velocity, 0, 0, "%.2f deg/s");
      ImGui::EndDisabled();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space))) {
      simulation_running = !simulation_running;
    }

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_),
                                            (float)1200 / (float)800,
                                            0.1f,
                                            100.0f);
    robot.shader_.SetMat4("projection", projection);
    robot.shader_.SetMat4("view", camera.GetViewMatrix());

    // Keep theta between [0, 360)
    if (robot.position_[2] >= 360 * M_PI / 180) {
      robot.position_[2] -= 360 * M_PI / 180;
    }
    else if (robot.position_[2] < 0) {
      robot.position_[2] += 360 * M_PI / 180;
    }

    // TODO: Add limits on each linear/angular velocities
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R))) {
      linear_velocity = 0;
      angular_velocity = 0;
      robot.position_ = glm::vec3(0, 0, 0);
      robot.UpdateModelMatrix();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W))) {
      linear_velocity += linear_velocity_step;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
      linear_velocity -= linear_velocity_step;
    }
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A))) {
//      printf("yes\n");
      angular_velocity -= angular_velocity_step;
    }
    else if (ImGui::IsKeyReleased((ImGui::GetKeyIndex(ImGuiKey_A)))) {
      angular_velocity = 0;
    }
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D))) {
      angular_velocity += angular_velocity_step;
    }
    else if (ImGui::IsKeyReleased((ImGui::GetKeyIndex(ImGuiKey_D)))) {
      angular_velocity = 0;
    }

    if (simulation_running) {
      /*
       * We want theta to be:
       * 1. Positive rotation means clockwise (not counter clockwise)
       * 2. With respect to the Y axis (up/down on the screen, X is left/right)
       * This way it is the same as heading in geodetic/gnss/ins navigation. Heading angle
       * is the angle with respect to North axis (up/down) and positive heading rotation means going clockwise
       * This is why the calculations below for X and Y position and swapped (cosine instead of sine, and vice
       * versa).
       */
      double w = angular_velocity * M_PI / 180 * simulation_dt;
      double v = linear_velocity * simulation_dt;
      // Near 0, so assume driving in a straight line
      if (abs(w) < 1.0e-5 ) {
        double y = robot.position_[1] += v * cos(robot.position_[2]);
        double x = robot.position_[0] += v * sin(robot.position_[2]);
        robot.position_ = glm::vec3(x, y, robot.position_[2]);
      }
      else {
        double theta_old = robot.position_[2];
        double r = v / w;
        robot.position_[2] += w;
        robot.position_[1] += r * (sin(robot.position_[2]) - sin(theta_old));
        robot.position_[0] += -r * (cos(robot.position_[2]) - cos(theta_old));
      }
      robot.UpdateModelMatrix();
    }

    ImGui::Text("Robot X: %.3f", robot.position_[0]);
    ImGui::Text("Robot Y: %.3f", robot.position_[1]);
    ImGui::Text("Robot T: %.3f", robot.position_[2] * 180 / M_PI);

    robot.Draw();

    ImGui::End();
    // End of frame
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
  }
  return 0;
}

void SetupWindow() {
  // OpenGL setup
#if defined(__APPLE__)
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
  if (!glfwInit()) {
    printf("GLFW ERROR: Failed to initialize\n");
    return;
  }
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__)
  /* OSX Requires fordward compatibility */
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  window = glfwCreateWindow(1200,
                            800,
                            "Simulator",
                            nullptr,
                            nullptr);
  if (window == nullptr) {
    printf("GLFW ERROR: Failed to create window\n");
    return;
  }

//  glfwSetFramebufferSizeCallback(gui_state.glfw_window, WindowResizeCallback);
//  glfwSetCursorPosCallback(gui_state.glfw_window, mouse_position_callback);
  glfwSetScrollCallback(window, ScrollCallback);
//  glfwSetMouseButtonCallback(gui_state.glfw_window, mouse_button_callback);

  glfwMakeContextCurrent(window);
  /* Enable vsync, when this is disabled was getting high cpu usage */
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("GLAD ERROR: Failed to initialize\n");
    return;
  }

  // Imgui and implot setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//  io.ConfigDockingWithShift = true;
//  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void ScrollCallback(GLFWwindow *win,
                    double xoffset,
                    double yoffset) {
  camera.UpdateZoom(static_cast<float>(yoffset));
}
