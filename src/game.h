#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "gameobject.h"
#include "canon.h"
#include "bullet.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  std::size_t GetWidth() const { return _grid_width; }
  std::size_t GetHeight() const { return _grid_height; }
  bool & GetRunningStatus() { return(_runningStatus); }
  float & GetSpeedFactor() { return(_speedFactor); }


 private:
  std::size_t _grid_width;
  std::size_t _grid_height;
  bool _runningStatus;
  float _speedFactor{1.0f};
  
  std::vector<GameObject *> _npcObjects;
  std::vector<Bullet *> _bulletObjects;
  std::unique_ptr<Canon> _canon;
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  void init(Renderer &, Controller const &);

  int score{0};

  void PlaceFood();
  void Update();
  void Render(Renderer &);
};

#endif