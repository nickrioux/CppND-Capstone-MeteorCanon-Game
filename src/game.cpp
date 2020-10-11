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

Game::Game() : _sdl_window(nullptr), _assetManager(&_entityManager) {
}

Game::~Game() {

}

void Game::Init(int width, int height) {
    
    _width = width;
    _height = height;

    _camera = {0, 0, _width, _height};

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

    //Load Map
    GetAssetManager()->AddTexture("skybox","../assets/skybox.png");
    _map = new Map("skybox",1,32);
    _map->LoadMap("../assets/skybox.map",25,20);

    _player = &_entityManager.AddEntity("canon",GameConstants::PLAYER_LAYER);
    GetAssetManager()->AddTexture("canon","../assets/base.png");
    _player->AddComponent<TransformComponent>(w/2,h-32, 0, 0, 0, 0, 1.0);
    _player->AddComponent<SpriteComponent>("canon");
    _player->AddComponent<KeyboardControlComponent>("left","right","space");
    _player->AddComponent<ColliderComponent>("PLAYER",w/2,h-32,35,39);

    Entity & bulletEntity = _entityManager.AddEntity("bullet",GameConstants::BULLET_LAYER);
    GetAssetManager()->AddTexture("bullet","../assets/basebullet.png");
    bulletEntity.AddComponent<TransformComponent>(w/2+13,h-55, 0, 0, 0, 0, 1.0);
    bulletEntity.AddComponent<SpriteComponent>("bullet");
    bulletEntity.AddComponent<BulletEmitterComponent>(200,270,h-55,true);
    
    Entity & asteroidEntity = _entityManager.AddEntity("asteroid",GameConstants::NPC_LAYER);
    GetAssetManager()->AddTexture("bigrock0","../assets/bigrock0.png");
    asteroidEntity.AddComponent<TransformComponent>(0,25,20,20,0,0,1.0);
    asteroidEntity.AddComponent<SpriteComponent>("bigrock0");
    asteroidEntity.AddComponent<ColliderComponent>("METEOR",0,25,50,50);



    Entity & leftWall = _entityManager.AddEntity("left_boundary",GameConstants::PLAYER_LAYER);
    leftWall.AddComponent<TransformComponent>(-10,0,0,0,10,GetHeight(),1.0);
    leftWall.AddComponent<ColliderComponent>("LEFT_BOUNDARY",-10,0,10,GetHeight());

    Entity & rightWall = _entityManager.AddEntity("right_boundary",GameConstants::PLAYER_LAYER);
    rightWall.AddComponent<TransformComponent>(GetWidth(),0,0,0,10,GetHeight(),1.0);
    rightWall.AddComponent<ColliderComponent>("RIGHT_BOUNDARY",GetWidth(),0,10,GetHeight());

    Entity & ground = _entityManager.AddEntity("ground",GameConstants::PLAYER_LAYER);
    ground.AddComponent<TransformComponent>(0,GetHeight()-10,0,0,GetWidth(),10,1.0);
    ground.AddComponent<ColliderComponent>("GROUND",0,GetHeight()-10,GetWidth(),10);

    _scoreLabel = &_entityManager.AddEntity("scoreLabel",GameConstants::UI_LAYER);
    GetAssetManager()->AddFont("charriot-font","../assets/fonts/charriot.ttf",14);
    _scoreLabel->AddComponent<TextLabelComponent>(w/2,14,"Score : ", "charriot-font", GameConstants::WHITE_COLOR);
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
}

void Game::handleCameraMovement() {
    TransformComponent* mainPlayerTransform = _player->GetComponent<TransformComponent>();

    //_camera.x = mainPlayerTransform->GetPosition().x - (_width / 2);
    //_camera.y = mainPlayerTransform->GetPosition().y - (_height / 2);
    _camera.x = 0;
    _camera.y = 0;
    _camera.x = _camera.x < 0 ? 0 : _camera.x;
    _camera.y = _camera.y < 0 ? 0 : _camera.y;
    _camera.x = _camera.x > _camera.w ? _camera.w : _camera.x;
    _camera.y = _camera.y > _camera.h ? _camera.h : _camera.y;
}

void Game::checkCollisions() {
    GameConstants::CollisionType collisionType = _entityManager.CheckCollisions();
    
    switch(collisionType) {
      case GameConstants::PLAYER_METEOR_COLLISION:
        std::cout << "METEOR COLLISION" << std::endl;
        break;
      case GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION:
      case GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION:
        adjustCanonPosition(collisionType);
        break;
      case GameConstants::METEOR_GROUND_COLLISION:
        _score -= 100;
        break;
      default:
        break;
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
