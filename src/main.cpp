#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <freetype/freetype.h>
#include <ft2build.h>

#include <cstdio>
#include <vector>
#include <memory>
#include <stdexcept>
#include <array>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "fonts.h"
#include "style.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

std::string exec_lsusb() {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lsusb", "r"), pclose);
  if(!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  std::cout << result;
  return result;
}

void exec_xtrlock() {
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("bash -c \"sleep 1 && xtrlock\"", "r"), pclose);
  if(!pipe) {
    throw std::runtime_error("popen() failed!");
  }
}

void blacklistUSBFileLinux() {
  std::ofstream outfile("/etc/modprobe.d/blacklist.conf",
                        std::ios_base::app | std::ios_base::out);
  outfile << "blacklist usb-storage\n";
  system("rmmod usb-storage");
  system("shutdown -r now");
}

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void TextCenter(std::string text) {
  float font_size = ImGui::GetFontSize() * text.size() / 2;
  ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
  ImGui::Text(text.c_str());
}

int main(int, char **) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return 1;
  }

  glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(IMGUI_IMPL_OPENGL_ES2)
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#elif defined(_WIN32)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

  float highDPIscaleFactor = 1.0;
#ifdef _WIN32
  // if it's a HighDPI monitor, try to scale everything
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  float xscale, yscale;
  glfwGetMonitorContentScale(monitor, &xscale, &yscale);
  if (xscale > 1 || yscale > 1) {
    highDPIscaleFactor = xscale;
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  }
#elif __APPLE__
  // to prevent 1200x800 from becoming 2400x1600
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
  // glfwWindowHint(GLFW_DECORATED, false); // Remove Title Bar

  GLFWwindow *window = glfwCreateWindow(1280, 720, "USB BARRIER", NULL, NULL);
  if (!window) {
    return 1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable VSYNC

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  imGuiStyle();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  io.Fonts->AddFontFromMemoryCompressedTTF(
      SourceCodePro_Regular_compressed_data,
      SourceCodePro_Regular_compressed_size, 24.0f);

  std::string lsusb_result;
  int lsusb_count = 1000;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    const int cnt = 0;

    lsusb_count++;
    if (lsusb_count == 1000) {
      lsusb_result = exec_lsusb();
      lsusb_count = 0;
    }

    // GUI Code Here
    {
      ImGui::Begin("USB Barrier Prototype", NULL,
                   ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
      std::string mainText = "USB Barrier Software";
      TextCenter(mainText);
      ImGui::NewLine();

      if(ImGui::Button("Protection Enable/Disable")) {
        exec_xtrlock();
      }
      enum ContentsType { CT_Text, CT_FillButton };
      static ImGuiTableFlags flags =
          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
      static bool display_headers = true;
      static int contents_type = CT_Text;

      int number_of_devices = 0;
      std::stringstream result(lsusb_result);
      std::string result_line;
      std::vector<std::string> devices;
      while (std::getline(result, result_line, "\n")) {
        devices.push_back(result_line);
        number_of_devices++;
      }

      if (ImGui::BeginTable("Connected Devices", 1, flags)) {
        if (display_headers) {
          ImGui::TableSetupColumn("Currently Connected");
          ImGui::TableHeadersRow();
        }
        for (int row = 0; row < number_of_devices; row++) {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          char buf[32];
          sprintf(buf, "hello", 0, row);
          ImGui::TextUnformatted(buf);
        }

      }

      ImGui::EndTable();
      if (ImGui::Button("Linux Blacklist Storage Devices (Requires Restart)")) {
        blacklistUSBFileLinux();
      }
      
    }

      ImGui::End();
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  //libusb_exit(context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
