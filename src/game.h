#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include "glm/glm.hpp"
#include "game.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "map.h"

class TransformComponent;

class Game {
  public:
      Game();
      ~Game();
      void Init(int width, int height);
      void Run(std::size_t target_frame_duration);
      int GetScore() const;
      bool IsRunning() const { return(_runningStatus); }
      float & GetSpeedFactor() { return(_speedFactor); }

      static Game & GetGame() { static Game game; return(game); }
      std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> & GetRenderer() { return _sdl_renderer; }
      const SDL_Rect & GetCamera() { return _camera; }
      EntityManager & GetEntityManager()  { return(*_entityManager);}
      AssetManager & GetAssetManager()  { return(*_assetManager);}
      const SDL_Event & GetEvent() { return(_event); }

      //Event Functions
      void GenerateBullet(const TransformComponent &);


 private:
     std::unique_ptr<AssetManager> _assetManager;
     std::unique_ptr<EntityManager> _entityManager;
     std::unique_ptr<SDL_Window,decltype(&SDL_DestroyWindow)> _sdl_window;
     SDL_Event _event;
     std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> _sdl_renderer;
     SDL_Rect _camera;

     //Game Entities and Objects
     std::unique_ptr<Map> _map;
     std::shared_ptr<Entity> _player;
     std::shared_ptr<Entity> _scoreLabel;
     std::shared_ptr<Entity> _levelLabel;
     std::shared_ptr<Entity> _lifeLabel;
     std::shared_ptr<Entity> _lifeSprite;
     std::shared_ptr<Entity> _gameOverLabel;

     //Game Variables and Status
     bool _runningStatus{true};
     int _score{0};
     int _maxScore{0};
     int _currLevel{1};
     float _speedFactor{1.0f};
     int _ticksLastFrame{0};
     int _ticksLastKill{0};
     int _width;
     int _height;
     int _lifes{3};
     int _fallingObjects{0};

     //Random Generator
     std::random_device _rd;
     std::mt19937  _gen;
     std::uniform_int_distribution<int> _random_pos_w;
     std::uniform_int_distribution<int> _random_degree_left;
     std::uniform_int_distribution<int> _random_degree_right;
     std::uniform_int_distribution<int> _random_speed;
     std::uniform_int_distribution<int> _random_meteor;


    //Access Data Methods
    const int getWidth() const { return(_width); }
    const int getHeight() const { return(_height); }

    //Generic Game Function
    void init(int width, int height);
    void processInput();
    void render();
    void destroy();

    //Specific Game Functions
     void loadGame();
     void update(std::size_t target_frame_duration);
     void updateLevel();
     void updateUI();
     void updateLife();
     void handleCameraMovement();
     void checkCollisions();
     void adjustCanonPosition(GameConstants::CollisionType);
     void generateMeteor(float deltaTime);
     void generateSpinner(float deltaTime);
     void generateExplosion(glm::vec2 pos, bool death = false);
     void generateUFO();
     void generateMissile();
     void processMeteorExplode(std::shared_ptr<Entity> meteor);
     void killPlayer();
     void restartPlayer();
};

#endif