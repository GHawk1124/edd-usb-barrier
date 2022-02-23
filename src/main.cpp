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

#include <fstream>
#include <iostream>
//#include <libusb.h>
#include <stdio.h>
#include <string>

#include "fonts.h"
#include "style.hpp"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/usb/USBSpec.h>
#include <dlfcn.h>
#include <locale.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <wchar.h>

#include "hidapi.h"
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

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
  glfwWindowHint(GLFW_DECORATED, false); // Remove Title Bar

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

  bool enableOrDisable = false;

  /*libusb_context *context = nullptr;
  if (libusb_init(&context) != LIBUSB_SUCCESS) {
    std::cout << "Failed to init Libusb" << std::endl;
    return -1;
  }
  */

  // #define MAX_STR 255
  // int res;
  // unsigned char buf[65];
  // wchar_t wstr[MAX_STR];
  // hid_device *handle;
  // int i;
  // res = hid_init();
  // handle = hid_open(0x4d8, 0x3f, NULL);
  // res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
  // wprintf(L"Manufacturer String: %s\n", wstr);
  // res = hid_get_product_string(handle, wstr, MAX_STR);
  // wprintf(L"Product String: %s\n", wstr);
  // res = hid_get_serial_number_string(handle, wstr, MAX_STR);
  // wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);
  // res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
  // wprintf(L"Indexed String 1: %s\n", wstr);
  // res = hid_exit();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    // libusb_device **usb_list = nullptr;
    // const int cnt = libusb_get_device_list(context, &usb_list);
    const int cnt = 0;
    // if (cnt < 0) {
    // std::cout << "Failed getting device list" << std::endl;
    // libusb_exit(context);
    // return 0;
    // }

    // GUI Code Here
    {
      ImGui::Begin("USB Barrier Prototype", NULL,
                   ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
      std::string mainText = "USB Barrier Software";
      TextCenter(mainText);
      ImGui::NewLine();
      ImGui::Checkbox("Protection Enable/Disable", &enableOrDisable);

      enum ContentsType { CT_Text, CT_FillButton };
      static ImGuiTableFlags flags =
          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
      static bool display_headers = true;
      static int contents_type = CT_Text;

      if (ImGui::BeginTable("Whitelist/Blacklist", 3, flags)) {
        if (display_headers) {
          ImGui::TableSetupColumn("Whitelist");
          ImGui::TableSetupColumn("Blacklist");
          ImGui::TableSetupColumn("Currently Connected");
          ImGui::TableHeadersRow();
        }

        for (int i = 0; i < cnt; ++i) {
          // libusb_device_descriptor desc;
          // libusb_get_device_descriptor(usb_list[i], &desc);

          // int row = 0;
          // int column = 2;
          // ImGui::TableNextRow();
          // ImGui::TableSetColumnIndex(column);
          // printf((char *)desc.idVendor);
          // std::cout << std::hex << desc.idVendor << "\n";
          // ImGui::TextUnformatted((char *)desc.idVendor);

          // libusb_device_handle *handle = nullptr;
          // if (libusb_open(usb_list[i], &handle) != LIBUSB_SUCCESS)
          // continue;

          // libusb_claim_interface(handle, 0);
          // libusb_close(handle);
        }

        // for (int row = 0; row < sizeof(usb_list) / sizeof(usb_list[0]);
        // row++) { ImGui::TableNextRow(); libusb_device_handle *handle; int err
        // = libusb_open(found, &handle); if (err) { std::cout << "Error opening
        // USB" << std::endl; return 0;
        // }

        // for (int column = 0; column < 3; column++) {
        // ImGui::TableSetColumnIndex(column);
        // char buf[32];
        // sprintf(buf, "Hello %d,%d", column, row);
        // if (contents_type == CT_Text)
        // ImGui::TextUnformatted(buf);
        // else if (contents_type)
        // ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
        // }
        // }
        ImGui::EndTable();
        if (ImGui::Button("Linux Blacklist Devices")) {
          blacklistUSBFileLinux();
        }
      }

      ImGui::End();
    }

    // libusb_free_device_list(usb_list, 1);

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
