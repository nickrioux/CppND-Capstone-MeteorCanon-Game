#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <SDL.h>

#include "sprite.h"
#include "snake.h"

class GameObject;

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void Render(std::shared_ptr<Sprite> sprite,float, float);
  void Render(GameObject & gameobject);
  void BeginFrame();
  void EndFrame();
  void UpdateWindowTitle(int score, int fps);

  std::shared_ptr<Sprite> CreateSprite(std::string path); 

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif