#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <SDL.h>
#include <freetype/freetype.h>
#include <ft2build.h>

#include <stdio.h>

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  float ddpi, hdpi, vdpi;
  if (SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi) != 0) {
    fprintf(stderr, "Failed to obtain DPI information for display 0: %s\n",
            SDL_GetError());
    exit(1);
  }
  float dpi_scaling = ddpi / 72.f;
  SDL_Rect display_bounds;
  if (SDL_GetDisplayUsableBounds(0, &display_bounds) != 0) {
    fprintf(stderr, "Failed to obtain bounds of display 0: %s\n",
            SDL_GetError());
    exit(1);
  }
  int win_w = display_bounds.w * 7 / 8, win_h = display_bounds.h * 7 / 8;

  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_Window *window =
      SDL_CreateWindow("USB Barrier", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, win_w, win_h, window_flags);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_Log("Error creating SDL_Renderer!");
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //

  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // SetupStyleFromHue();

  ImGui_ImplSDL2_InitForSDLRenderer(window);
  ImGui_ImplSDLRenderer_Init(renderer);

  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);
  io.Fonts->AddFontFromFileTTF(
      "./assets/SourceCodePro/SourceCodePro-Regular.ttf", dpi_scaling * 7.0f,
      NULL, NULL);

  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        done = true;
      }
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window)) {
        done = true;
      }
    }

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // GUI Code Here
    {
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Begin("Hello, world!");
      ImGui::Text("This is some useful text.");
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float *)&clear_color);
      if (ImGui::Button("Button")) {
        counter++;
      }
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    if (show_another_window) {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) {
        show_another_window = false;
      }
      ImGui::End();
    }

    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, (uint8_t)(clear_color.x * 255),
                           (uint8_t)(clear_color.y * 255),
                           (uint8_t)(clear_color.z * 255),
                           (uint8_t)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
