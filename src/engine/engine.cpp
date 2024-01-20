#include "engine.h"

#include <SDL.h>
#include <SDL_filesystem.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <Sdl_ttf.h>

SDL2Engine::SDL2Engine(int screen_w, int screen_h)
    : base_path_(SDL_GetBasePath()), screen_w_(screen_w), screen_h_(screen_h) {}

SDL2Engine::~SDL2Engine() {
  SDL_DestroyWindow(window_);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void SDL2Engine::Init(std::string title) {
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
  window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, screen_w_, screen_h_,
                             SDL_WINDOW_SHOWN);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

void SDL2Engine::LoadFont(std::string filepath, int size) {
  std::string fullpath = base_path_ + filepath;
  font_ = TTF_OpenFont(fullpath.c_str(), size);
}

void SDL2Engine::IMGLoadTexture(std::string texture_name,
                                std::string filepath) {
  std::string fullpath = base_path_ + filepath;
  SDL_Surface *surface = IMG_Load(fullpath.c_str());
  textures_[texture_name] = SDL_CreateTextureFromSurface(renderer_, surface);
}

void SDL2Engine::DrawTexture(std::string texture_name, int x, int y, int w,
                             int h) {
  SDL_Rect dst = {x, y, w, h};
  SDL_RenderCopy(renderer_, textures_[texture_name], NULL, &dst);
}

void SDL2Engine::SetTextureColor(std::string texture_name, uint8_t r, uint8_t g,
                                 uint8_t b) {
  SDL_SetTextureColorMod(textures_[texture_name], r, g, b);
}

void SDL2Engine::SetDrawColor(int r, int g, int b, int a) {
  SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void SDL2Engine::UpdateText(std::string name, std::string text, SDL_Rect &rect,
                            SDL_Color color) {
  SDL_Surface *surface_text =
      TTF_RenderText_Solid_Wrapped(font_, text.c_str(), color, 0);
  textures_[name] = SDL_CreateTextureFromSurface(renderer_, surface_text);

  SDL_RenderCopy(renderer_, textures_[name], NULL, &rect);
  SDL_FreeSurface(surface_text);
}

void SDL2Engine::DrawText(std::string name, SDL_Rect &rect) {
  SDL_RenderCopy(renderer_, textures_[name], NULL, &rect);
}

void SDL2Engine::Render() {
  SDL_RenderPresent(renderer_);
  SDL_SetRenderDrawColor(renderer_, 200, 200, 200, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer_);
}
