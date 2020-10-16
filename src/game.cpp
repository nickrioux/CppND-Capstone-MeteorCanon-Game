#include <iostream>
#include <SDL.h>

#include "glm/glm.hpp"

#include "entitymanager.h"
#include "entity.h"
#include "map.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "keyboardcontrolcomponent.h"
#include "collidercomponent.h"
#include "textlabelcomponent.h"
#include "bulletemittercomponent.h"

#include "game.h"

Game::Game() : _sdl_window(nullptr), _assetManager(&_entityManager), _gen(_rd()) {
}

Game::~Game() {

}

void Game::Init(int width, int height) {
    
    _width = width;
    _height = height;

    _camera = {0, 0, _width, _height};

    _random_degree_left = std::uniform_int_distribution<>(90,120);
    _random_degree_right = std::uniform_int_distribution<>(60,90);
    _random_speed  = std::uniform_int_distribution<>(20,100);
    _random_pos_w  = std::uniform_int_distribution<>(0,_width);
    _random_meteor  = std::uniform_int_distribution<>(0,11);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    _sdl_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        SDL_WINDOW_BORDERLESS
    );

    if (!_sdl_window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    _sdl_renderer = SDL_CreateRenderer(_sdl_window, -1, 0);
    if (!_sdl_renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    loadLevel(0);

    return;
}

void Game::Run(std::size_t target_frame_duration) {

  while (IsRunning()) {
        processInput();
        update(target_frame_duration);
        render();
  }  

  destroy();
}

bool Game::IsInGrid(float x, float y) {
    if ((x > 0 && x <= _width) && (y > 0 && y <= _height)) {
        return(true);
    }
    else
    {
        return(false);
    }
}

int Game::GetScore() const {
  return _score;
}

void Game::loadLevel(int levelNo) {
    int w,h;

    //Retrieve Renderer Size
    SDL_GetRendererOutputSize(_sdl_renderer, &w, &h);

    //Init Score and Level
    _score = 0;
    _currLevel = 1;

    //Load Textures
    GetAssetManager()->AddTexture("skybox","../assets/skybox-blue.png");
    GetAssetManager()->AddTexture("canon","../assets/base-new.png");
    GetAssetManager()->AddTexture("bigrock0","../assets/bigrock0.png");
    GetAssetManager()->AddTexture("bigrock1","../assets/bigrock1.png");
    GetAssetManager()->AddTexture("bigrock2","../assets/bigrock2.png");
    GetAssetManager()->AddTexture("bigrock3","../assets/bigrock3.png");
    GetAssetManager()->AddTexture("bigrock4","../assets/bigrock4.png");
    GetAssetManager()->AddTexture("bigrock5","../assets/bigrock5.png");
    GetAssetManager()->AddTexture("smallrock0","../assets/smallrock0.png");
    GetAssetManager()->AddTexture("smallrock1","../assets/smallrock1.png");
    GetAssetManager()->AddTexture("smallrock2","../assets/smallrock2.png");
    GetAssetManager()->AddTexture("smallrock3","../assets/smallrock3.png");
    GetAssetManager()->AddTexture("smallrock4","../assets/smallrock4.png");
    GetAssetManager()->AddTexture("smallrock5","../assets/smallrock5.png");

    GetAssetManager()->AddTexture("bullet","../assets/basebullet.png");

    //Load Font
    GetAssetManager()->AddFont("charriot-font","../assets/fonts/charriot.ttf",32);

    //Load Map
    _map = new Map("skybox",1,64);
    _map->LoadMap("../assets/skybox.map",16,12);

    _player = &_entityManager.AddEntity("canon",GameConstants::PLAYER_ENTITY,GameConstants::PLAYER_LAYER);
    _player->AddComponent<TransformComponent>(w/2,h-12-96, 0, 0, 0, 0, 1.0);
    _player->AddComponent<SpriteComponent>("canon");
    _player->AddComponent<KeyboardControlComponent>("left","right","space");
    _player->AddComponent<ColliderComponent>("PLAYER",w/2,h-12-96,32,32);


    Entity & leftWall = _entityManager.AddEntity("left_boundary",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    leftWall.AddComponent<TransformComponent>(-500,0,0,0,500,GetHeight(),1.0);
    leftWall.AddComponent<ColliderComponent>("LEFT_BOUNDARY",-500,0,500,GetHeight());

    Entity & rightWall = _entityManager.AddEntity("right_boundary",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    rightWall.AddComponent<TransformComponent>(GetWidth(),0,0,0,500,GetHeight(),1.0);
    rightWall.AddComponent<ColliderComponent>("RIGHT_BOUNDARY",GetWidth(),0,500,GetHeight());

    Entity & ground = _entityManager.AddEntity("ground",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    ground.AddComponent<TransformComponent>(0,GetHeight(),0,0,GetWidth(),2000,1.0);
    ground.AddComponent<ColliderComponent>("GROUND",0,GetHeight(),GetWidth(),2000);

    _scoreLabel = &_entityManager.AddEntity("scoreLabel",GameConstants::UI_ENTITY,GameConstants::UI_LAYER);
    _scoreLabel->AddComponent<TextLabelComponent>(w/2-15,14,"Score : ", "charriot-font", GameConstants::WHITE_COLOR);

    _levelLabel = &_entityManager.AddEntity("levelLabel",GameConstants::UI_ENTITY,GameConstants::UI_LAYER);
    _levelLabel->AddComponent<TextLabelComponent>(0,14,"Level : ","charriot-font", GameConstants::WHITE_COLOR);
}

void Game::processInput() {

    SDL_PollEvent(Game::GetEvent());
    switch (Game::GetEvent()->type) {
        case SDL_QUIT: {
            _runningStatus = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (Game::GetEvent()->key.keysym.sym == SDLK_ESCAPE) {
                _runningStatus = false;
            }
        }
        default: {
            break;
        }
    }  

}

void Game::update(std::size_t target_frame_duration ) {
    // Wait until 16ms has ellapsed since the last frame
    int delayTime = target_frame_duration - (SDL_GetTicks() - _ticksLastFrame);
    
    if (delayTime > 0 && delayTime <= target_frame_duration) {
        SDL_Delay(delayTime);
    }

    // Delta time is the difference in ticks from last frame converted to secomds
    float deltaTime = (SDL_GetTicks() - _ticksLastFrame) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    _ticksLastFrame = SDL_GetTicks();

    _entityManager.Update(deltaTime);

    handleCameraMovement();

    checkCollisions();

    //Update Level
    updateLevel();

    //Update UI
    updateUI();


    generateMeteor(deltaTime);

}

void Game::updateLevel() {
    if (_score > GameConstants::LEVEL_6) {  
        _currLevel = 6;
        _speedFactor = 6.0f;
    }
    else if (_score > GameConstants::LEVEL_5) {
        _currLevel = 5;
        _speedFactor = 5.0f;
    }
    else if (_score > GameConstants::LEVEL_4) {
        _currLevel = 4;
        _speedFactor = 4.0f;
    }
    else if (_score > GameConstants::LEVEL_3) {
        _currLevel = 3;
        _speedFactor = 3.0f;
    }
    else if (_score > GameConstants::LEVEL_2) {
        _currLevel = 2;
        _speedFactor = 2.0f;
    }
}

void Game::updateUI() {
  TextLabelComponent * scoreLabel = _scoreLabel->GetComponent<TextLabelComponent>();
  TextLabelComponent * levelLabel = _levelLabel->GetComponent<TextLabelComponent>();

  string score = "Score : " + std::to_string(_score);
  scoreLabel->SetLabelText(score,"charriot-font");

  string level = "Level : " + std::to_string(_currLevel);
  levelLabel->SetLabelText(level,"charriot-font");
}

void Game::handleCameraMovement() {
    TransformComponent* mainPlayerTransform = _player->GetComponent<TransformComponent>();
    _camera.x = 0;
    _camera.y = 0;
    _camera.x = _camera.x < 0 ? 0 : _camera.x;
    _camera.y = _camera.y < 0 ? 0 : _camera.y;
    _camera.x = _camera.x > _camera.w ? _camera.w : _camera.x;
    _camera.y = _camera.y > _camera.h ? _camera.h : _camera.y;
}

void Game::checkCollisions() {
    vector<EntityManager::CollisionData> collisions =  _entityManager.CheckCollisions();
    
    if (collisions.size() > 0) {

        for (int i = 0; i < collisions.size(); ++i) {
            EntityManager::CollisionData collisionData = collisions[i];

            switch(collisionData.collisionType) {
              case GameConstants::PLAYER_METEOR_COLLISION:
                std::cout << "METEOR COLLISION" << std::endl;
                break;
              case GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION:
              case GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION:
                adjustCanonPosition(collisionData.collisionType);
                break;
              case GameConstants::METEOR_GROUND_COLLISION:
                if (collisionData.entityOne->GetEntityType() == GameConstants::COLLIDE_ENTITY) {

                    if (collisionData.entityTwo->GetEntityType() == GameConstants::BIG_METEOR_ENTITY)
                    {
                        _score += GameConstants::SCORE_BIG_ROCK_LANDING * _currLevel;
                    }
                    else
                    {
                        _score += GameConstants::SCORE_SMALL_ROCK_LANDING * _currLevel;
                    }
                    
                    collisionData.entityTwo->Destroy();
                }
                else
                {
                    if (collisionData.entityOne->GetEntityType() == GameConstants::BIG_METEOR_ENTITY)
                    {
                        _score += GameConstants::SCORE_BIG_ROCK_LANDING * _currLevel;
                    }
                    else
                    {
                        _score += GameConstants::SCORE_SMALL_ROCK_LANDING * _currLevel;
                    }

                    collisionData.entityOne->Destroy();
                }
                break;
              case GameConstants::ENEMY_BULLET_COLLISION:
                std::cout << "METEOR DESTROYED\n";
                if (collisionData.entityOne->GetEntityType() == GameConstants::PLAYER_ENTITY)
                {
                    processMeteorExplode(collisionData.entityTwo);
                }
                {
                    processMeteorExplode(collisionData.entityOne);
                }
                collisionData.entityOne->Destroy();
                collisionData.entityTwo->Destroy();
                break;
              case GameConstants::METEOR_BOUNDARY_COLLISION:
                if ((collisionData.entityOne->GetEntityType() == GameConstants::BIG_METEOR_ENTITY) || (collisionData.entityOne->GetEntityType() == GameConstants::SMALL_METEOR_ENTITY)) {
                    collisionData.entityOne->Destroy();
                    std::cout << "METEOR OUT OF BOUNDARY\n";
                }
                else if ((collisionData.entityTwo->GetEntityType() == GameConstants::BIG_METEOR_ENTITY) || (collisionData.entityTwo->GetEntityType() == GameConstants::SMALL_METEOR_ENTITY)) {
                    collisionData.entityTwo->Destroy();
                    std::cout << "METEOR OUT OF BOUNDARY\n";
                }
                break;
              default:
                break;
            }
        }
    } 
}

void Game::processMeteorExplode (Entity * meteor)
{
    int w,h;

    if (meteor->GetEntityType() == GameConstants::SMALL_METEOR_ENTITY)
    {
        _score += GameConstants::SCORE_SMALL_ROCK * _currLevel;
    }    
    else
    {
        //Generate Two Small Meteor
        glm::vec2 pos = meteor->GetComponent<TransformComponent>()->GetPosition();
        glm::vec2 speed = meteor->GetComponent<TransformComponent>()->GetVelocity();

        glm::vec2 posSmall1 = {pos.x + GameConstants::BIG_METEOR_W * 1/4,pos.y + GameConstants::BIG_METEOR_H/2 - GameConstants::SMALL_METEOR_H/2};
        glm::vec2 posSmall2 = {pos.x + GameConstants::BIG_METEOR_W * 3/4,pos.y + GameConstants::BIG_METEOR_H/2 - GameConstants::SMALL_METEOR_H/2};
        std::uniform_int_distribution<> speedRnd(-40, -20);
        glm::vec2 speed1 = {speedRnd(_gen), speed.y};
        glm::vec2 speed2 = {-speed1.x, speed.y};

        // Rotate the speeds of the small rocks some times:
        int dir = std::rand() % 3;
        for (int times = std::rand() % 2 + 1; times > 0; times--) {
            switch (dir)
            {
                case 0:
                  speed1.x--;
                  speed2.y++;
                break;
                case 1:
                  speed2.x++;
                  speed1.y++;
                  break;
                default:
                  break;
            }
        }

        //Retrieve the Sprite Number
        std::cout << meteor->GetName() << " No : " << meteor->GetName().back() << std::endl;
        std::string smallRockSpriteName = "smallrock";
        smallRockSpriteName += meteor->GetName().back();
        Entity & smallRock1 = _entityManager.AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
        smallRock1.AddComponent<TransformComponent>(posSmall1.x,posSmall1.y,speed1.x,speed1.y,0,0,1.0);
        smallRock1.AddComponent<SpriteComponent>(smallRockSpriteName);
        w = 30; 
        h = 18;
        smallRock1.AddComponent<ColliderComponent>("METEOR",posSmall1.x,posSmall1.y,w,h);

        Entity & smallRock2 = _entityManager.AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
        smallRock2.AddComponent<TransformComponent>(posSmall2.x,posSmall2.y,speed2.x,speed2.y,0,0,1.0);
        smallRock2.AddComponent<SpriteComponent>(smallRockSpriteName);
        w = 30;
        h = 18;
        smallRock2.AddComponent<ColliderComponent>("METEOR",posSmall2.x,posSmall2.y,w,h);

        _score += GameConstants::SCORE_BIG_ROCK * _currLevel;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(_sdl_renderer, 21, 21, 21, 255);
    SDL_RenderClear(_sdl_renderer);

    if (_entityManager.HasNoEntities()) {
        return;
    }

    _entityManager.Render(); 

    SDL_RenderPresent(_sdl_renderer);  
}

void Game::destroy() {
    SDL_DestroyRenderer(_sdl_renderer);
    SDL_DestroyWindow(_sdl_window);
    SDL_Quit();
}

void Game::adjustCanonPosition(GameConstants::CollisionType collisionType) {
    TransformComponent * transform = _player->GetComponent<TransformComponent>();

    if (collisionType == GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION) {
        transform->GetPosition().x = 0;
    }
    else if (collisionType == GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION)
    {
        transform->GetPosition().x = _width - transform->GetWidth();
    }   
}

void Game::generateMeteor(float deltaTime) {

    //Each 2 seconds generate a new Meteor
    static float generateTime{0};

    generateTime += deltaTime;

    if (generateTime > 2) {
      int posX = _random_pos_w(_gen);
      int speed = _random_speed(_gen);
      float angle{0.0f};

      if (posX <= (GetWidth()/2))
      {
          angle = glm::radians(static_cast<float>(_random_degree_right(_gen)));
      }
      else {
          angle = glm::radians(static_cast<float>(_random_degree_left(_gen)));
      }

      int w,h;

      int type = _random_meteor(_gen);

      if (type > 5) //Small Rock
      {
          type = (type - 5);
          Entity & asteroidEntity = _entityManager.AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
          asteroidEntity.AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,0,0,1.0);
          asteroidEntity.AddComponent<SpriteComponent>("smallrock" + std::to_string(type));
          w = 30;
          h = 18;
          asteroidEntity.AddComponent<ColliderComponent>("METEOR",posX,0,w,h);
      }
      else //Big Rock
      {
          Entity & asteroidEntity = _entityManager.AddEntity(GameConstants::BIG_METEOR_ENTITY_NAME+std::to_string(type),GameConstants::BIG_METEOR_ENTITY,GameConstants::NPC_LAYER);
          asteroidEntity.AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,0,0,1.0);
          asteroidEntity.AddComponent<SpriteComponent>("bigrock" + std::to_string(type));
          w = 60;
          h = 35;      
          asteroidEntity.AddComponent<ColliderComponent>("METEOR",posX,0,w,h);
      }
      

      generateTime = 0;
    }
}
