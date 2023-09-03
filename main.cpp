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

  while (!glfwWindowShouldClose(window)) {
    // Start of frame
    glfwPollEvents();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_),
                                            (float)1200 / (float)800,
                                            0.1f,
                                            100.0f);
    robot.shader_.SetMat4("projection", projection);
    robot.shader_.SetMat4("view", camera.GetViewMatrix());

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W))) {
      robot.position_.y += 0.2;
      robot.UpdateModelMatrix();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
      robot.position_.y -= 0.2;
      robot.UpdateModelMatrix();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A))) {
      robot.position_.x -= 0.2;
      robot.UpdateModelMatrix();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_D))) {
      robot.position_.x += 0.2;
      robot.UpdateModelMatrix();
    }

    robot.Draw();

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
  printf("yoffset: %.3f\n", yoffset);
  camera.UpdateZoom(static_cast<float>(yoffset));
}
