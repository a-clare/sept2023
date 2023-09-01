#include <iostream>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "implot.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

GLFWwindow *window;
static void SetupWindow();

int main() {
  SetupWindow();

  while (!glfwWindowShouldClose(window)) {
    // Start of frame
    glfwPollEvents();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


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
//  glfwSetScrollCallback(gui_state.glfw_window, mouse_scroll_callback);
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
