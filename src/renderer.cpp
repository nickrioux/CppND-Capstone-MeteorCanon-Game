#include <iostream>
#include <string>

#include "renderer.h"
#include "sprite.h"
#include "gameobject.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

std::shared_ptr<Sprite> Renderer::CreateSprite(std::string path) {
#if 0
  IMG_Init(IMG_INIT_PNG);
  SDL_Texture * texture = IMG_LoadTexture(renderer,path.c_str());
#endif
  SDL_Surface * temp   = SDL_LoadBMP(path.c_str());
  int w = temp->w;
  int h = temp->h;
  int colorkey = SDL_MapRGB(temp->format, 255, 0, 255);
  SDL_SetColorKey(temp, SDL_TRUE, colorkey);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(sdl_renderer, temp);
  SDL_FreeSurface(temp);

  return(std::make_shared<Sprite>(texture,w,h));
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
#if 0
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  block.w = 40;
  block.h = 20;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
#endif
  SDL_Rect src;

  src.x = 0;
  src.y = 0;
  src.w = 32;
  src.h = 32;

  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h - 16;
  block.w = 32;
  block.h = 32;

  //SDL_RenderCopy(sdl_renderer, canonShape, &src, &block);
  //canonSprite->Render(sdl_renderer,&block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(GameObject & gameobject) {
  
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  gameobject.render(*this);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(std::shared_ptr<Sprite> sprite, int x, int y) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  block.x = static_cast<int>(x) * block.w;
  block.y = static_cast<int>(y) * block.h;
  block.w = 35;
  block.h = 39;

  sprite->Render(sdl_renderer,&block);    
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
