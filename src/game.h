#ifndef GAME_H
#define GAME_H

#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "map.h"

class Game {
  public:
     Game();
     ~Game();
     void Init(int width, int height);
     void Run(std::size_t target_frame_duration);
     int GetScore() const;
     bool IsRunning() const { return(_runningStatus); }
     float & GetSpeedFactor() { return(_speedFactor); }
     const int GetWidth() const { return(_width); }
     const int GetHeight() const { return(_height); }
     bool IsInGrid(float x, float y);
     static Game * GetGame() { static Game game; return(&game); }
     SDL_Renderer * GetRenderer() { return _sdl_renderer; }
     SDL_Rect * GetCamera() { return &_camera; }
     EntityManager * GetEntityManager() { return(&_entityManager);};
     AssetManager * GetAssetManager() { return(&_assetManager);}
     SDL_Event * GetEvent() { return(&_event); }

 private:
     AssetManager _assetManager;
     EntityManager _entityManager;
     SDL_Window * _sdl_window;
     SDL_Event _event;
     SDL_Renderer * _sdl_renderer;
     SDL_Rect _camera;
     Map * _map;
     Entity * _player;
     Entity * _scoreLabel;
     Entity * _levelLabel;
     bool _runningStatus{true};
     int _score{0};
     int _currLevel{1};
     float _speedFactor{1.0f};
     int _ticksLastFrame{0};
     int _width;
     int _height;

     //Random Generator
     std::random_device _rd;
     std::mt19937  _gen;
     std::uniform_int_distribution<int> _random_pos_w;
     std::uniform_int_distribution<int> _random_degree_left;
     std::uniform_int_distribution<int> _random_degree_right;
     std::uniform_int_distribution<int> _random_speed;
     std::uniform_int_distribution<int> _random_meteor;


     void loadLevel(int levelNo);
     void init(int width, int height);
     void processInput();
     void update(std::size_t target_frame_duration);
     void updateLevel();
     void updateUI();
     void handleCameraMovement();
     void render();
     void checkCollisions();
     void destroy();
     void adjustCanonPosition(GameConstants::CollisionType);
     void generateMeteor(float deltaTime);
     void generateUFO();
     void generateMissile();
     void processMeteorExplode(Entity * meteor);
};

#endif