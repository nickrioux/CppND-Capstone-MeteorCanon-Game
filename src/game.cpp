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

Game::Game() :  _gen(_rd()), _sdl_window(nullptr,SDL_DestroyWindow), _sdl_renderer(nullptr,SDL_DestroyRenderer),
                _entityManager(std::make_unique<EntityManager>()), _assetManager(std::make_unique<AssetManager>()) {

}

Game::~Game() {
    //Implicit call destruction for the window and renderer object before SDL_Quit
    _sdl_window.reset(nullptr);
    _sdl_renderer.reset(nullptr);
    SDL_Quit();
}

void Game::Init(int width, int height) {
    
    _width = width;
    _height = height;

    _camera = {0, 0, _width, _height};

    _random_degree_left = std::uniform_int_distribution<>(90,105);
    _random_degree_right = std::uniform_int_distribution<>(85,90);
    _random_speed  = std::uniform_int_distribution<>(20,100);
    _random_pos_w  = std::uniform_int_distribution<>(0,_width);
    _random_meteor  = std::uniform_int_distribution<>(0,10);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    _sdl_window.reset(SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        SDL_WINDOW_BORDERLESS
    ));

    if (!_sdl_window.get()) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    _sdl_renderer.reset(SDL_CreateRenderer(_sdl_window.get(), -1, 0));

    if (!_sdl_renderer.get()) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    loadGame();

    return;
}

void Game::Run(std::size_t target_frame_duration) {

  while (IsRunning()) {
        processInput();
        update(target_frame_duration);
        render();
  }

  std::cout << "Falling Objects : " << _fallingObjects << std::endl;   
}

int Game::GetScore() const {
  return _score;
}

void Game::loadGame() {
    int w,h;

    //Retrieve Renderer Size
    SDL_GetRendererOutputSize(_sdl_renderer.get(), &w, &h);

    //Init Score and Level
    _score = 0;
    _currLevel = 1;

    //Load Textures
    GetAssetManager().AddTexture("skybox1","../assets/skybox-level1.png");
    GetAssetManager().AddTexture("skybox2","../assets/skybox-level2.png");
    GetAssetManager().AddTexture("skybox3","../assets/skybox-level3.png");
    GetAssetManager().AddTexture("skybox4","../assets/skybox-level4.png");
    GetAssetManager().AddTexture("skybox5","../assets/skybox-level5.png");
    GetAssetManager().AddTexture("skybox6","../assets/skybox-level6.png");
    GetAssetManager().AddTexture("canon","../assets/canon.png");
    GetAssetManager().AddTexture("bullet","../assets/bullet.png");
    GetAssetManager().AddTexture("canon-life","../assets/canon-life.png");
    GetAssetManager().AddTexture("bigrock0","../assets/bigrock0.png");
    GetAssetManager().AddTexture("bigrock1","../assets/bigrock1.png");
    GetAssetManager().AddTexture("bigrock2","../assets/bigrock2.png");
    GetAssetManager().AddTexture("bigrock3","../assets/bigrock3.png");
    GetAssetManager().AddTexture("bigrock4","../assets/bigrock4.png");
    GetAssetManager().AddTexture("bigrock5","../assets/bigrock5.png");
    GetAssetManager().AddTexture("smallrock0","../assets/smallrock0.png");
    GetAssetManager().AddTexture("smallrock1","../assets/smallrock1.png");
    GetAssetManager().AddTexture("smallrock2","../assets/smallrock2.png");
    GetAssetManager().AddTexture("smallrock3","../assets/smallrock3.png");
    GetAssetManager().AddTexture("smallrock4","../assets/smallrock4.png");
    GetAssetManager().AddTexture("smallrock5","../assets/smallrock5.png");
    GetAssetManager().AddTexture("explosion0","../assets/explosion0.png");
    GetAssetManager().AddTexture("explosion1","../assets/explosion1.png");
    GetAssetManager().AddTexture("small-spinner","../assets/small-spinner.png");
    GetAssetManager().AddTexture("big-spinner","../assets/big-spinner.png");



    //Load Font
    GetAssetManager().AddFont("charriot-font","../assets/fonts/charriot.ttf",64);

    //Load Map
    _map = std::make_unique<Map>("skybox1",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
    _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);

    //Init Player
    restartPlayer();

    std::shared_ptr<Entity> entity = GetEntityManager().AddEntity("left_boundary",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    entity->AddComponent<TransformComponent>(-500,0,0,0,500,getHeight(),1.0);
    entity->AddComponent<ColliderComponent>("LEFT_BOUNDARY",-500,0,500,getHeight());

    entity = GetEntityManager().AddEntity("right_boundary",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    entity->AddComponent<TransformComponent>(getWidth(),0,0,0,500,getHeight(),1.0);
    entity->AddComponent<ColliderComponent>("RIGHT_BOUNDARY",getWidth(),0,500,getHeight());

    entity= GetEntityManager().AddEntity("ground",GameConstants::COLLIDE_ENTITY,GameConstants::PLAYER_LAYER);
    entity->AddComponent<TransformComponent>(0,getHeight()-64-12,0,0,getWidth(),2000,1.0);
    entity->AddComponent<ColliderComponent>("GROUND",0,getHeight()-64-12,getWidth(),2000);

    _scoreLabel = GetEntityManager().AddEntity("scoreLabel",GameConstants::UI_ENTITY,GameConstants::UI_LAYER);
    _scoreLabel->AddComponent<TextLabelComponent>(32,h-64,"0", "charriot-font", GameConstants::WHITE_COLOR);

    _levelLabel = GetEntityManager().AddEntity("levelLabel",GameConstants::UI_ENTITY,GameConstants::UI_LAYER);
    _levelLabel->AddComponent<TextLabelComponent>(w-128,h-64,"1x","charriot-font", GameConstants::WHITE_COLOR);

    _lifeLabel = GetEntityManager().AddEntity("lifeLabel",GameConstants::UI_ENTITY, GameConstants::UI_LAYER);
    _lifeLabel->AddComponent<TextLabelComponent>(w/2, h - 64,"3X","charriot-font", GameConstants::WHITE_COLOR);
    _lifeSprite = GetEntityManager().AddEntity("lifeSprite",GameConstants::UI_ENTITY, GameConstants::UI_LAYER);
    _lifeSprite->AddComponent<TransformComponent>(w/2+64, h - 56,0,0,32,32,1.5);   
    _lifeSprite->AddComponent<SpriteComponent>("canon-life"); 
}

void Game::processInput() {

    SDL_PollEvent(&_event);
    switch (GetEvent().type) {
        case SDL_QUIT: {
            _runningStatus = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (GetEvent().key.keysym.sym == SDLK_ESCAPE) {
                _runningStatus = false;
            }
        }
        default: {
            break;
        }
    }  

}

void Game::update(std::size_t target_frame_duration ) {

    static int gameState = GameConstants::GAME_STATE_RUNNING;
    
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

    GetEntityManager().Update(deltaTime * GetSpeedFactor());

    handleCameraMovement();

    if (gameState == GameConstants::GAME_STATE_RUNNING) {

        //Check Collisions
        checkCollisions();

        //Update Level
        updateLevel();

        //Update Lifes
        updateLife();

        //Generate Meteor
        generateMeteor(deltaTime);

        //Generate Spinner
        generateSpinner(deltaTime);

        //Player Restart
        if (_ticksLastKill != 0)
        {
                float killDeltaTime = (_ticksLastFrame - _ticksLastKill)/1000.0f;

                _speedFactor = GameConstants::SPEED_FACTOR_KILL_TRANSITION;

                if (killDeltaTime > GameConstants::TIME_RESTART_PLAYER) {
                    _ticksLastKill = 0;
                    restartPlayer();
                }
        }
    }

    //Update UI
    updateUI();

    if ((_ticksLastKill == 0) && (_lifes == 0) && (gameState == GameConstants::GAME_STATE_RUNNING)) //GameOver
    {
        std::cout << "Game Over!\n";
        gameState = GameConstants::GAME_STATE_GAMEOVER;

        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL1;

        std::shared_ptr<Entity> gameOverLabel = GetEntityManager().AddEntity("gameOverLabel", GameConstants::UI_ENTITY, GameConstants::UI_LAYER);
        gameOverLabel->AddComponent<TextLabelComponent>(getWidth()/3,getHeight()/3,"GAME OVER!","charriot-font", GameConstants::WHITE_COLOR);
    }

}

void Game::updateLevel() {
    if (_score >= GameConstants::LEVEL_6) {  
        
        if (_currLevel !=6) {
            //Change Map
            _map = std::make_unique<Map>("skybox6",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);
        }

        _currLevel = 6;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL6;
    }
    else if (_score >= GameConstants::LEVEL_5) {
        if (_currLevel != 5) {
            //Change Map
            _map = std::make_unique<Map>("skybox5",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);
        }

        _currLevel = 5;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL5;
    }
    else if (_score >= GameConstants::LEVEL_4) {
        if (_currLevel != 4) {
            //Change Map
            _map = std::make_unique<Map>("skybox4",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);
        }

        _currLevel = 4;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL4;
    }
    else if (_score >= GameConstants::LEVEL_3) {
        if (_currLevel != 3) {
            //Change Map
            _map = std::make_unique<Map>("skybox3",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);
        }
        _currLevel = 3;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL3;
    }
    else if (_score >= GameConstants::LEVEL_2) {        
        if (_currLevel != 2) {
            //Change Map
            _map = std::make_unique<Map>("skybox2",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH);
        }

        _currLevel = 2;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL2;
    }
    else {
        if (_currLevel != 1) {
            //Change Map
            _map = std::make_unique<Map>("skybox1",GameConstants::SCALE_GAME,GameConstants::MAP_TILE_SIZE);
            _map->LoadMap("../assets/skybox1.map",GameConstants::MAP_WIDTH,GameConstants::MAP_HEIGTH
            );
        }

        _currLevel = 1;
        _speedFactor = GameConstants::SPEED_FACTOR_LEVEL1;
    }
}

void Game::updateUI() {
  std::shared_ptr<TextLabelComponent> scoreLabel = _scoreLabel->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> levelLabel = _levelLabel->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> lifeLabel = _lifeLabel->GetComponent<TextLabelComponent>();

  string score = std::to_string(_score);
  scoreLabel->SetLabelText(score,"charriot-font");

  string level = std::to_string(_currLevel) + "x";
  levelLabel->SetLabelText(level,"charriot-font");

  string life = std::to_string(_lifes) + "x";
  lifeLabel->SetLabelText(life,"charriot-font");
}

void Game::handleCameraMovement() {
    _camera.x = 0;
    _camera.y = 0;
    _camera.x = _camera.x < 0 ? 0 : _camera.x;
    _camera.y = _camera.y < 0 ? 0 : _camera.y;
    _camera.x = _camera.x > _camera.w ? _camera.w : _camera.x;
    _camera.y = _camera.y > _camera.h ? _camera.h : _camera.y;
}

void Game::checkCollisions() {
    vector<EntityManager::CollisionData> collisions =  GetEntityManager().CheckCollisions();
    
    if (collisions.size() > 0) {

        for (int i = 0; i < collisions.size(); ++i) {
            EntityManager::CollisionData collisionData = collisions[i];

            switch(collisionData.collisionType) {
              case GameConstants::PLAYER_METEOR_COLLISION:
                std::cout << "METEOR COLLISION" << std::endl;
                std::cout << collisionData.entityTwo->GetName() << std::endl;
                generateExplosion(_player->GetComponent<TransformComponent>()->GetPosition(),true);
                collisionData.entityOne->Destroy();
                collisionData.entityTwo->Destroy();
                killPlayer();
                _fallingObjects--;
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

                    generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition());
                    
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

                    generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition());

                    collisionData.entityOne->Destroy();
                }

                _fallingObjects--;

                break;
              case GameConstants::ENEMY_BULLET_COLLISION:
                std::cout << "METEOR DESTROYED\n";
                if (collisionData.entityOne->GetEntityType() == GameConstants::BULLET_ENTITY)
                {
                    switch(collisionData.entityTwo->GetEntityType()) {
                        case GameConstants::BIG_METEOR_ENTITY:
                        case GameConstants::SMALL_METEOR_ENTITY:
                            processMeteorExplode(collisionData.entityTwo);
                            break;
                        case GameConstants::SMALL_SPINNER_ENTITY:
                            _score += GameConstants::SCORE_SMALL_SPINNER * _currLevel;
                            generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition());
                            _fallingObjects--;
                            break;
                        case GameConstants::BIG_SPINNER_ENTITY:
                            _score += GameConstants::SCORE_BIG_SPINNER * _currLevel;
                            generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition());
                            _fallingObjects--;
                            break;
                        default:
                            break;
                    };
                }
                else
                {
                    switch(collisionData.entityOne->GetEntityType()) {
                        case GameConstants::BIG_METEOR_ENTITY:
                        case GameConstants::SMALL_METEOR_ENTITY:
                            processMeteorExplode(collisionData.entityOne);
                            break;
                        case GameConstants::SMALL_SPINNER_ENTITY:
                            _score += GameConstants::SCORE_SMALL_SPINNER;
                            generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition());
                            _fallingObjects--;
                            break;
                        case GameConstants::BIG_SPINNER_ENTITY:
                            _score += GameConstants::SCORE_BIG_SPINNER;
                            generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition());
                            _fallingObjects--;
                            break;
                        default:
                            break;
                    };
                }
                collisionData.entityOne->Destroy();
                collisionData.entityTwo->Destroy();
                break;
              case GameConstants::FALLING_OBJECT_BOUNDARY_COLLISION:
                if (collisionData.entityOne->GetEntityType() == GameConstants::COLLIDE_ENTITY) {
                    collisionData.entityTwo->Destroy();
                    std::cout << "OBJECT OUT OF BOUNDARY\n";
                }
                else if (collisionData.entityTwo->GetEntityType() == GameConstants::COLLIDE_ENTITY) {
                    collisionData.entityOne->Destroy();
                    std::cout << "OBJECT OUT OF BOUNDARY\n";
                }
                _fallingObjects--;
                break;
              case GameConstants::PLAYER_SPINNER_COLLISION:
              case GameConstants::SPINNER_GROUND_COLLISION:
                    _score += GameConstants::SCORE_LOST_BASE;
                    generateExplosion(_player->GetComponent<TransformComponent>()->GetPosition(),true);
                    if ((collisionData.entityOne->GetEntityType() == GameConstants::SMALL_SPINNER_ENTITY) || (collisionData.entityOne->GetEntityType() == GameConstants::BIG_SPINNER_ENTITY)) {
                        generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition());
                        collisionData.entityOne->Destroy(); //Spinner
                    }
                    else {
                        generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition());
                        collisionData.entityTwo->Destroy(); //Spinner
                    }
                    _fallingObjects--;
                    _player->Destroy();
                    killPlayer();
                    break;
                default:
                    break;
            }
        }
    } 
}

void Game::processMeteorExplode (std::shared_ptr<Entity> meteor)
{
    int w,h;

    if (meteor->GetEntityType() == GameConstants::SMALL_METEOR_ENTITY)
    {
        generateExplosion(meteor->GetComponent<TransformComponent>()->GetPosition());
        _score += GameConstants::SCORE_SMALL_ROCK * _currLevel;
        _fallingObjects--;
    }    
    else if (rand() % 2)
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
        std::shared_ptr<Entity> smallRock1 = GetEntityManager().AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
        smallRock1->AddComponent<TransformComponent>(posSmall1.x,posSmall1.y,speed1.x,speed1.y,30,18,GameConstants::SCALE_GAME);
        smallRock1->AddComponent<SpriteComponent>(smallRockSpriteName);
        w = 30; 
        h = 18;
        smallRock1->AddComponent<ColliderComponent>("METEOR",posSmall1.x,posSmall1.y,w,h);

        std::shared_ptr<Entity> smallRock2 = GetEntityManager().AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
        smallRock2->AddComponent<TransformComponent>(posSmall2.x,posSmall2.y,speed2.x,speed2.y,30,18,GameConstants::SCALE_GAME);
        smallRock2->AddComponent<SpriteComponent>(smallRockSpriteName);
        w = 30;
        h = 18;
        smallRock2->AddComponent<ColliderComponent>("METEOR",posSmall2.x,posSmall2.y,w,h);

        _fallingObjects++;
    }
    else {
        generateExplosion(meteor->GetComponent<TransformComponent>()->GetPosition());
        _score += GameConstants::SCORE_BIG_ROCK * _currLevel;
        _fallingObjects--;
    }
}

void Game::GenerateBullet(const TransformComponent & transformComp) {
    std::shared_ptr<Entity>  bulletEntity = GetEntityManager().AddEntity("bullet",GameConstants::BULLET_ENTITY,GameConstants::BULLET_LAYER);
    bulletEntity->AddComponent<TransformComponent>(transformComp.GetPosition().x+13,transformComp.GetPosition().y-40, 0, -GameConstants::BULLET_SPEED, 7, 39, GameConstants::SCALE_GAME);
    bulletEntity->AddComponent<SpriteComponent>("bullet");
    bulletEntity->AddComponent<ColliderComponent>("BULLET",transformComp.GetPosition().x+13,transformComp.GetPosition().y-40,transformComp.GetWidth(),transformComp.GetHeight());
    bulletEntity->AddComponent<BulletEmitterComponent>(GameConstants::BULLET_SPEED,270,getHeight()-40,false);
}


void Game::render() {
    SDL_SetRenderDrawColor(_sdl_renderer.get(), 21, 21, 21, 255);
    SDL_RenderClear(_sdl_renderer.get());

    if (GetEntityManager().HasNoEntities()) {
        std::cout << "NO ENTITIES RENDER\n" << std::endl;
        return;
    }

    GetEntityManager().Render(); 

    SDL_RenderPresent(_sdl_renderer.get());  
}

void Game::destroy() {

}

void Game::adjustCanonPosition(GameConstants::CollisionType collisionType) {
    std::shared_ptr<TransformComponent> transform = _player->GetComponent<TransformComponent>();

    if (collisionType == GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION) {
        transform->SetPosition(0,transform->GetPosition().y);
    }
    else if (collisionType == GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION)
    {
        transform->SetPosition(_width - transform->GetWidth(), transform->GetPosition().y);
    }   
}

void Game::generateMeteor(float deltaTime) {

    //Each TIME_FREQ_METEOR seconds generate a new Meteor
    static float generateTime{0};

    generateTime += deltaTime * GetSpeedFactor();

    if ((generateTime > (GameConstants::TIME_FREQ_METEOR)) || (_fallingObjects < 2)) {
      int posX = _random_pos_w(_gen);
      int speed = _random_speed(_gen);
      float angle{0.0f};

      if (posX <= (getWidth()/2))
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
          std::shared_ptr<Entity> asteroidEntity = GetEntityManager().AddEntity(GameConstants::SMALL_METEOR_ENTITY_NAME,GameConstants::SMALL_METEOR_ENTITY,GameConstants::NPC_LAYER);
          asteroidEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,30,18,GameConstants::SCALE_GAME);
          asteroidEntity->AddComponent<SpriteComponent>("smallrock" + std::to_string(type));
          w = 30;
          h = 18;
          asteroidEntity->AddComponent<ColliderComponent>("METEOR",posX,0,w,h);
      }
      else //Big Rock
      {
          std::shared_ptr<Entity> asteroidEntity = GetEntityManager().AddEntity(GameConstants::BIG_METEOR_ENTITY_NAME+std::to_string(type),GameConstants::BIG_METEOR_ENTITY,GameConstants::NPC_LAYER);
          asteroidEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,60,35,GameConstants::SCALE_GAME);
          asteroidEntity->AddComponent<SpriteComponent>("bigrock" + std::to_string(type));
          w = 60;
          h = 35;      
          asteroidEntity->AddComponent<ColliderComponent>("METEOR",posX,0,w,h);
      }

      _fallingObjects++;
      

      generateTime = 0;
    }
}

void Game::generateSpinner(float deltaTime)
{
    static float generateTime{0};

    generateTime += deltaTime * GetSpeedFactor();

    if ((generateTime > GameConstants::TIME_FREQ_SPINNER) || (_fallingObjects < 2)) {
            int posX = _random_pos_w(_gen);
            int speed = _random_speed(_gen);
            float angle{0.0f};

            if (posX <= (getWidth()/2))
            {
                angle = glm::radians(static_cast<float>(_random_degree_right(_gen)));
            }
            else {
                angle = glm::radians(static_cast<float>(_random_degree_left(_gen)));
            }

            //Determine if it'a a big spinner or small one (Random)
            int spinnerType = rand() % 2;

            if (spinnerType) {

                std::shared_ptr<Entity> spinnerEntity = GetEntityManager().AddEntity(GameConstants::SMALL_SPINNER_ENTITY_NAME, GameConstants::SMALL_SPINNER_ENTITY, GameConstants::NPC_LAYER);
                spinnerEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,32,32,GameConstants::SCALE_GAME);
                spinnerEntity->AddComponent<SpriteComponent>("small-spinner",4,90);
                spinnerEntity->AddComponent<ColliderComponent>("SPINNER",posX,0,32,32);
            }
            else
            {
                std::shared_ptr<Entity>spinnerEntity = GetEntityManager().AddEntity(GameConstants::BIG_SPINNER_ENTITY_NAME, GameConstants::BIG_SPINNER_ENTITY, GameConstants::NPC_LAYER);
                spinnerEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,64,64,GameConstants::SCALE_GAME);
                spinnerEntity->AddComponent<SpriteComponent>("big-spinner",4,90);
                spinnerEntity->AddComponent<ColliderComponent>("SPINNER",posX,0,64,64);
            }
            

            _fallingObjects++;
            generateTime = 0;
    }        
}

void Game::generateExplosion(glm::vec2 pos, bool death) {
  

        if (death) {
            pos.x = pos.x + 16; //TODO
            pos.y = pos.y; //TODO

            std::shared_ptr<Entity> explosion1 = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            std::shared_ptr<Entity> explosion2 = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            std::shared_ptr<Entity> explosion3 = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            std::shared_ptr<Entity> explosion4 = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            std::shared_ptr<Entity> explosion5 = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            
            explosion1->AddComponent<TransformComponent>(pos.x,pos.y,GameConstants::CANON_EXPLOSION_HIGH_SPEED,0,43,43,GameConstants::SCALE_GAME);
            explosion1->AddComponent<SpriteComponent>("explosion0");
            explosion1->SetLifeTime(GameConstants::TIME_RESTART_PLAYER);
            
            explosion2->AddComponent<TransformComponent>(pos.x,pos.y,GameConstants::CANON_EXPLOSION_LOW_SPEED,-GameConstants::CANON_EXPLOSION_LOW_SPEED,43,43,GameConstants::SCALE_GAME);
            explosion2->AddComponent<SpriteComponent>("explosion0");
            explosion2->SetLifeTime(GameConstants::TIME_RESTART_PLAYER);
            
            explosion3->AddComponent<TransformComponent>(pos.x,pos.y,0,-GameConstants::CANON_EXPLOSION_HIGH_SPEED,43,43,GameConstants::SCALE_GAME);
            explosion3->AddComponent<SpriteComponent>("explosion0");
            explosion3->SetLifeTime(GameConstants::TIME_RESTART_PLAYER);
            
            explosion4->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CANON_EXPLOSION_LOW_SPEED,-GameConstants::CANON_EXPLOSION_LOW_SPEED,43,43,GameConstants::SCALE_GAME);
            explosion4->AddComponent<SpriteComponent>("explosion0");
            explosion4->SetLifeTime(GameConstants::TIME_RESTART_PLAYER);

            explosion5->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CANON_EXPLOSION_HIGH_SPEED,0,43,43,GameConstants::SCALE_GAME);
            explosion5->AddComponent<SpriteComponent>("explosion0");
            explosion5->SetLifeTime(GameConstants::TIME_RESTART_PLAYER);
        }
        else {
            //Create Entity Explosion
            std::shared_ptr<Entity> explosionEntity = GetEntityManager().AddEntity("explosion",GameConstants::EXPLOSION_ENTITY, GameConstants::NPC_LAYER);
            explosionEntity->AddComponent<TransformComponent>(pos.x,pos.y,0,0,43,43,GameConstants::SCALE_GAME);
            explosionEntity->AddComponent<SpriteComponent>("explosion1");
            explosionEntity->SetLifeTime(0.10);
        }
}

void Game::killPlayer() {
    _lifes -= 1;
    _ticksLastKill = SDL_GetTicks();
}

void Game::restartPlayer() {
    _player = GetEntityManager().AddEntity("canon",GameConstants::PLAYER_ENTITY,GameConstants::PLAYER_LAYER);
    _player->AddComponent<TransformComponent>(getWidth()/2,getHeight()-12-32-64, 0, 0, 32, 32, GameConstants::SCALE_GAME);
    _player->AddComponent<SpriteComponent>("canon");
    _player->AddComponent<KeyboardControlComponent>("left","right","space");
    _player->AddComponent<ColliderComponent>("PLAYER",getWidth()/2,getHeight()-12-32-64,32,32);
}

void Game::updateLife() {
        if (_score > 0 && (_score > _maxScore)) {
                int oldScore = _maxScore;
                _maxScore = _score;

                bool newLife = ((_maxScore/GameConstants::SCORE_LIFE) - (oldScore/GameConstants::SCORE_LIFE) > 0 ? true : false);

                if (newLife) 
                {
                    _lifes++;
                }
        }
}