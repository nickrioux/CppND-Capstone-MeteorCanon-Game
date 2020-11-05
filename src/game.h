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

      //Status Methods
      int GetScore() const;
      bool IsRunning() const { return(runningStatus_); }
      const float & GetSpeedFactor() { return(speedFactor_); }

      //Return the Singleton Instance of Game
      static Game & GetGame() { static Game game; return(game); }

      //Access methods for Managers, Camera and SDL Event
      std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> & GetRenderer() { return sdl_renderer_; }
      const SDL_Rect & GetCamera() { return camera_; }
      EntityManager & GetEntityManager()  { return(*entityManager_);}
      AssetManager & GetAssetManager()  { return(*assetManager_);}
      const SDL_Event & GetEvent() { return(event_); }

      //Event Functions
      void BulletEvent();


 private:
     std::unique_ptr<AssetManager> assetManager_;
     std::unique_ptr<EntityManager> entityManager_;
     std::unique_ptr<SDL_Window,decltype(&SDL_DestroyWindow)> sdl_window_;
     SDL_Event event_;
     std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)> sdl_renderer_;
     SDL_Rect camera_;

     //Game Entities Player and Map
     std::unique_ptr<Map> map_;
     std::shared_ptr<Entity> player_;

     //UI Entities
     std::shared_ptr<Entity> scoreLabel_;
     std::shared_ptr<Entity> levelLabel_;
     std::shared_ptr<Entity> lifeLabel_;
     std::shared_ptr<Entity> lifeSprite_;
     std::shared_ptr<Entity> gameOverLabel_;

     //Game Variables and Status
     bool eventBullet_{false};
     bool runningStatus_{true};
     int score_{0};
     int maxScore_{0};
     int currLevel_{1};
     float speedFactor_{1.0f};
     int ticksLastFrame_{0};
     int ticksLastKill_{0};
     float generateSpinnerTime_{0.0f};
     float generateMeteorTime_{0.0f};
     int width_{0};
     int height_{0};
     int lifes_{3};
     int fallingObjects_{0};

     //Random Generators
     std::random_device rd_;
     std::mt19937  gen_;
     std::uniform_int_distribution<int> random_pos_w_;
     std::uniform_int_distribution<int> random_degree_left_;
     std::uniform_int_distribution<int> random_degree_right_;
     std::uniform_int_distribution<int> random_speed_;
     std::uniform_int_distribution<int> random_meteor_;
     std::uniform_int_distribution<int> random_;

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
     void generateBullet();
     void generateMeteor(float deltaTime);
     void generateSpinner(float deltaTime);
     void generateExplosion(glm::vec2 pos, const GameConstants::EntityType & entityType);
     void processMeteorExplode(std::shared_ptr<Entity> meteor);
     void killPlayer();
     void restartPlayer();
};

#endif