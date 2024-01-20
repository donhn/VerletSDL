#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL_render.h>
#include <SDL_video.h>
#include <Sdl_ttf.h>

#include <string>
#include <unordered_map>

class SDL2Engine {
 public:
  SDL2Engine(int screen_w, int screen_h);
  ~SDL2Engine();
  void Init(std::string title);
  void IMGLoadTexture(std::string texture_name, std::string filepath);
  void LoadFont(std::string filepath, int size);
  void DrawTexture(std::string texture_name, int x, int y, int w, int h);
  void SetDrawColor(int r, int g, int b, int a);
  void UpdateText(std::string name, std::string text, SDL_Rect& rect,
                  SDL_Color color);
  void DrawText(std::string name, SDL_Rect& rect);
  void SetTextureColor(std::string texture_name, uint8_t r, uint8_t g,
                       uint8_t b);
  void Render();

 private:
  std::string base_path_;
  int screen_w_;
  int screen_h_;
  std::unordered_map<std::string, SDL_Texture*> textures_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  TTF_Font* font_;
};

#endif  // !ENGINE_H_
