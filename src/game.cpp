#include <iostream>
#include <SDL.h>

#include "glm/glm.hpp"

#include "entitymanager.h"
#include "entity.h"
#include "map.h"
#include "components/transformcomponent.h"
#include "components/spritecomponent.h"
#include "components/keyboardcontrolcomponent.h"
#include "components/collidercomponent.h"
#include "components/textlabelcomponent.h"
#include "components/bulletemittercomponent.h"

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
    _random = std::uniform_int_distribution<>();

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

    //Start Collisions Detection Thread
    GetEntityManager().SimulateCollisions();

    return;
}

void Game::Run(std::size_t target_frame_duration) {

  while (IsRunning()) {
        processInput();
        update(target_frame_duration);
        render();
  }
}

int Game::GetScore() const {
  return _score;
}

void Game::BulletEvent() {
    _eventBullet = true;
}

void Game::loadGame() {
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



    //Load Fonts
    GetAssetManager().AddFont("charriot-font","../assets/fonts/charriot.ttf",64);

    //Load Map
    _map = std::make_unique<Map>("skybox1",GameConstants::kScaleGame,GameConstants::kMapTileSize);
    _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);

    //Init Player
    restartPlayer();

    std::shared_ptr<Entity> entity = GetEntityManager().AddEntity("left_boundary",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(GameConstants::LeftBoundaryDeltaX,0,0,0,GameConstants::LeftBoundarySize,_height,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::LeftBoundaryTag,GameConstants::LeftBoundaryDeltaX,0,GameConstants::LeftBoundarySize,_height);

    entity = GetEntityManager().AddEntity("right_boundary",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(_width,0,0,0,GameConstants::RightBoundarySize,_height,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::RightBoundaryTag,_width,0,GameConstants::RightBoundarySize,_height);

    entity= GetEntityManager().AddEntity("ground",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(0,_height+GameConstants::GroundDeltaY,0,0,_width,GameConstants::GroundSize,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::GroundTag,0,_height+GameConstants::GroundDeltaY,_width,GameConstants::GroundSize);

    _scoreLabel = GetEntityManager().AddEntity("scoreLabel",GameConstants::UIEntity,GameConstants::UILayer);
    _scoreLabel->AddComponent<TextLabelComponent>(GameConstants::ScoreLabelX,_height+GameConstants::ScoreLabelDeltaY,"", "charriot-font", GameConstants::kWhiteColor);

    _levelLabel = GetEntityManager().AddEntity("levelLabel",GameConstants::UIEntity,GameConstants::UILayer);
    _levelLabel->AddComponent<TextLabelComponent>(_width+GameConstants::LevelLabelDeltaX,_height+GameConstants::LevelLabelDeltaY,"","charriot-font", GameConstants::kWhiteColor);

    _lifeLabel = GetEntityManager().AddEntity("lifeLabel",GameConstants::UIEntity, GameConstants::UILayer);
    _lifeLabel->AddComponent<TextLabelComponent>(_width/2, _height+GameConstants::LifeLabelDeltaY,"","charriot-font", GameConstants::kWhiteColor);
    _lifeSprite = GetEntityManager().AddEntity("lifeSprite",GameConstants::UIEntity, GameConstants::UILayer);
    _lifeSprite->AddComponent<TransformComponent>(  _width/2+GameConstants::LifeSpriteDeltaX, _height+GameConstants::LifeSpriteDeltaY,0,0,GameConstants::LifeSpriteSize,
                                                    GameConstants::LifeSpriteSize,GameConstants::kScaleGame);   
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
    static int gameState = GameConstants::GameState::Running;
    
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

    //Update Entities
    GetEntityManager().Update(deltaTime * GetSpeedFactor());

    handleCameraMovement();

    if (gameState == GameConstants::GameState::Running) {

        //Check Collisions
        checkCollisions();

        //Update Level
        updateLevel();

        //Update Lifes
        updateLife();

        //Generate Bullet - Check if bullet event is triggered and generate a bullet
        generateBullet();

        //Generate Meteor
        generateMeteor(deltaTime);

        //Generate Spinner
        generateSpinner(deltaTime);

        //Player Restart
        if (_ticksLastKill != 0)
        {
                float killDeltaTime = (_ticksLastFrame - _ticksLastKill)/1000.0f;

                _speedFactor = GameConstants::kSpeedFactorKillTransition;

                if (killDeltaTime > GameConstants::kTimeRestartPlayer) {
                    _ticksLastKill = 0;
                    restartPlayer();
                }
        }
    }

    //Update UI
    updateUI();

    //Game Over
    if ((_ticksLastKill == 0) && (_lifes == 0) && (gameState == GameConstants::GameState::Running)) //GameOver
    {
        gameState = GameConstants::GameState::GameOver;
        _speedFactor = GameConstants::kSpeedFactorLevel1;
        std::shared_ptr<Entity> gameOverLabel = GetEntityManager().AddEntity("gameOverLabel", GameConstants::UIEntity, GameConstants::UILayer);
        gameOverLabel->AddComponent<TextLabelComponent>(_width/3,_height/3,"GAME OVER!","charriot-font", GameConstants::kWhiteColor);
    }

}

void Game::updateLevel() {

    if (_score >= GameConstants::Level6) {  
        
        if (_currLevel !=6) {
            //Change Map
            _map = std::make_unique<Map>("skybox6",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        _currLevel = 6;
        _speedFactor = GameConstants::kSpeedFactorLevel6;
    }
    else if (_score >= GameConstants::Level5) {
        if (_currLevel != 5) {
            //Change Map
            _map = std::make_unique<Map>("skybox5",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        _currLevel = 5;
        _speedFactor = GameConstants::kSpeedFactorLevel5;
    }
    else if (_score >= GameConstants::Level4) {
        if (_currLevel != 4) {
            //Change Map
            _map = std::make_unique<Map>("skybox4",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        _currLevel = 4;
        _speedFactor = GameConstants::kSpeedFactorLevel4;
    }
    else if (_score >= GameConstants::Level3) {
        if (_currLevel != 3) {
            //Change Map
            _map = std::make_unique<Map>("skybox3",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }
        _currLevel = 3;
        _speedFactor = GameConstants::kSpeedFactorLevel3;
    }
    else if (_score >= GameConstants::Level2) {        
        if (_currLevel != 2) {
            //Change Map
            _map = std::make_unique<Map>("skybox2",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        _currLevel = 2;
        _speedFactor = GameConstants::kSpeedFactorLevel2;
    }
    else {
        if (_currLevel != 1) {
            //Change Map
            _map = std::make_unique<Map>("skybox1",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            _map->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight
            );
        }

        _currLevel = 1;
        _speedFactor = GameConstants::kSpeedFactorLevel1;
    }
}

void Game::updateUI() {
  std::shared_ptr<TextLabelComponent> scoreLabel = _scoreLabel->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> levelLabel = _levelLabel->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> lifeLabel = _lifeLabel->GetComponent<TextLabelComponent>();

  std::string score = std::to_string(_score);
  scoreLabel->SetLabelText(score,"charriot-font");

  std::string level = std::to_string(_currLevel) + "x";
  levelLabel->SetLabelText(level,"charriot-font");

  std::string life = std::to_string(_lifes) + "x";
  lifeLabel->SetLabelText(life,"charriot-font");
}

//The Camera is fixed for this game.
void Game::handleCameraMovement() {
    _camera.x = 0;
    _camera.y = 0;
    _camera.x = _camera.x < 0 ? 0 : _camera.x;
    _camera.y = _camera.y < 0 ? 0 : _camera.y;
    _camera.x = _camera.x > _camera.w ? _camera.w : _camera.x;
    _camera.y = _camera.y > _camera.h ? _camera.h : _camera.y;
}

void Game::checkCollisions() {
    
    std::vector<EntityManager::CollisionData> collisions;
    
    collisions =  GetEntityManager().GetCollisions();
    
    if (collisions.size() > 0) {

        for (int i = 0; i < collisions.size(); ++i) {
            EntityManager::CollisionData collisionData = collisions[i];

            //Check if the Entities are still Active
            if (collisionData.entityOne->IsActive() && collisionData.entityTwo->IsActive()) {

                switch(collisionData.collisionType) {
                    case GameConstants::PlayerMeteor:
                        generateExplosion(_player->GetComponent<TransformComponent>()->GetPosition(),_player->GetEntityType());
                        collisionData.entityOne->Destroy();
                        collisionData.entityTwo->Destroy();
                        killPlayer();
                        _fallingObjects--;
                        break;
                    case GameConstants::PlayerLeftBoundary:
                    case GameConstants::PlayerRightBoundary:
                        adjustCanonPosition(collisionData.collisionType);
                        break;
                    case GameConstants::MeteorGround: {
                        std::shared_ptr<Entity> thatEntity = (collisionData.entityOne->GetEntityType() == GameConstants::CollideEntity ? collisionData.entityTwo : collisionData.entityOne);
                        if (thatEntity->GetEntityType() == GameConstants::BigMeteorEntity)
                        {
                            _score += GameConstants::BigRockLandingScore * _currLevel;
                        }
                        else
                        {
                            _score += GameConstants::SmallRockLandingScore * _currLevel;
                        }
                        generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                        thatEntity->Destroy();
                        _fallingObjects--;
                        break;
                    };
                    case GameConstants::EnemyBullet: {
                        std::shared_ptr<Entity> thatEntity = (collisionData.entityOne->GetEntityType() == GameConstants::BulletEntity ? collisionData.entityTwo : collisionData.entityOne);
                        switch(thatEntity->GetEntityType()) {
                                case GameConstants::BigMeteorEntity:
                                case GameConstants::SmallMeteorEntity:
                                    processMeteorExplode(thatEntity);
                                    _fallingObjects--;
                                    break;
                                case GameConstants::SmallSpinnerEntity:
                                    _score += GameConstants::SmallSpinnerScore * _currLevel;
                                    generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                                    _fallingObjects--;
                                    break;
                                case GameConstants::BigSpinnerEntity:
                                    _score += GameConstants::BigSpinnerScore * _currLevel;
                                    generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                                    _fallingObjects--;
                                    break;
                                default:
                                    break;
                        };
                        collisionData.entityOne->Destroy();
                        collisionData.entityTwo->Destroy();
                        break;
                    };
                    case GameConstants::FallingObjectBoundary:
                            if (collisionData.entityOne->GetEntityType() == GameConstants::CollideEntity) {
                                collisionData.entityTwo->Destroy();
                            }
                            else if (collisionData.entityTwo->GetEntityType() == GameConstants::CollideEntity) {
                                collisionData.entityOne->Destroy();
                            }
                            _fallingObjects--;
                        break;
                    case GameConstants::PlayerSpinner:
                    case GameConstants::SpinnerGround:
                        _score += GameConstants::LostBaseScore;
                        generateExplosion(_player->GetComponent<TransformComponent>()->GetPosition(),_player->GetEntityType());
                        
                        if ((collisionData.entityOne->GetEntityType() == GameConstants::SmallSpinnerEntity) || (collisionData.entityOne->GetEntityType() == GameConstants::BigSpinnerEntity)) {
                            generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition(),collisionData.entityOne->GetEntityType());
                            collisionData.entityOne->Destroy(); //Spinner
                        }
                        else {
                            generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition(),collisionData.entityTwo->GetEntityType());
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
}

void Game::processMeteorExplode (std::shared_ptr<Entity> meteor)
{
    if (meteor->GetEntityType() == GameConstants::SmallMeteorEntity)
    {
        generateExplosion(meteor->GetComponent<TransformComponent>()->GetPosition(),meteor->GetEntityType());
        _score += GameConstants::SmallRockScore * _currLevel;
    }    
    else if (_random(_gen) % 2)
    {
        //Generate Two Small Meteor
        glm::vec2 pos = meteor->GetComponent<TransformComponent>()->GetPosition();
        glm::vec2 speed = meteor->GetComponent<TransformComponent>()->GetVelocity();

        glm::vec2 posSmall1 = {pos.x + GameConstants::BigMeteorSizeW * 1/4,pos.y + GameConstants::BigMeteorSizeH/2 - GameConstants::SmallMeteorSizeH/2};
        glm::vec2 posSmall2 = {pos.x + GameConstants::BigMeteorSizeW * 3/4,pos.y + GameConstants::BigMeteorSizeH/2 - GameConstants::SmallMeteorSizeH/2};
        std::uniform_int_distribution<> speedRnd(-40, -20);
        glm::vec2 speed1 = {speedRnd(_gen), speed.y};
        glm::vec2 speed2 = {-speed1.x, speed.y};

        // Rotate the speeds of the small rocks some times:
        int dir = _random(_gen) % 3;
        for (int times = _random(_gen) % 2 + 1; times > 0; times--) {
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
        std::string smallRockSpriteName = "smallrock";
        smallRockSpriteName += meteor->GetName().back();
        
        std::shared_ptr<Entity> smallRock1 = GetEntityManager().AddEntity(GameConstants::kSmallMeteorName,GameConstants::SmallMeteorEntity,GameConstants::NpcLayer);
        smallRock1->AddComponent<TransformComponent>(posSmall1.x,posSmall1.y,speed1.x,speed1.y,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH,GameConstants::kScaleGame);
        smallRock1->AddComponent<SpriteComponent>(smallRockSpriteName);
        smallRock1->AddComponent<ColliderComponent>(GameConstants::MeteorTag,posSmall1.x,posSmall1.y,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH);

        std::shared_ptr<Entity> smallRock2 = GetEntityManager().AddEntity(GameConstants::kSmallMeteorName,GameConstants::SmallMeteorEntity,GameConstants::NpcLayer);
        smallRock2->AddComponent<TransformComponent>(posSmall2.x,posSmall2.y,speed2.x,speed2.y,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH,GameConstants::kScaleGame);
        smallRock2->AddComponent<SpriteComponent>(smallRockSpriteName);
        smallRock2->AddComponent<ColliderComponent>(GameConstants::MeteorTag,posSmall2.x,posSmall2.y,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH);
                
        _fallingObjects+=2;
    }
    else {
        generateExplosion(meteor->GetComponent<TransformComponent>()->GetPosition(),meteor->GetEntityType());
        _score += GameConstants::BigRockScore * _currLevel;
    }
}

void Game::generateBullet() {
    if (_eventBullet) {
        std::shared_ptr<TransformComponent> transformComp = _player->GetComponent<TransformComponent>();

        std::shared_ptr<Entity>  bulletEntity = GetEntityManager().AddEntity("bullet",GameConstants::BulletEntity,GameConstants::BulletLayer);
        bulletEntity->AddComponent<TransformComponent>( transformComp->GetPosition().x+GameConstants::BulletDeltaX,transformComp->GetPosition().y+GameConstants::BulletDeltaY, 0, 
                                                        -GameConstants::BulletSpeed, GameConstants::BulletSizeW, GameConstants::BulletSizeH, GameConstants::kScaleGame);
        bulletEntity->AddComponent<SpriteComponent>("bullet");
        bulletEntity->AddComponent<ColliderComponent>(  GameConstants::BulletTag,transformComp->GetPosition().x+GameConstants::BulletDeltaX,transformComp->GetPosition().y+GameConstants::BulletDeltaY,
                                                        transformComp->GetWidth(),transformComp->GetHeight());
        bulletEntity->AddComponent<BulletEmitterComponent>(GameConstants::BulletSpeed,GameConstants::kUpAngle,_height+GameConstants::BulletDeltaY,false);
    
        _eventBullet = false;
    }
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

    if (collisionType == GameConstants::PlayerLeftBoundary) {
        transform->SetPosition(0,transform->GetPosition().y);
    }
    else if (collisionType == GameConstants::PlayerRightBoundary)
    {
        transform->SetPosition(_width - transform->GetWidth(), transform->GetPosition().y);
    }
   
}

void Game::generateMeteor(float deltaTime) {

    //Each TIME_FREQ_METEOR seconds generate a new Meteor

    _generateMeteorTime += deltaTime * GetSpeedFactor();

    if ((_generateMeteorTime > (GameConstants::kTimeFreqMeteor)) || (_fallingObjects < (GameConstants::kMinFallingObjects * _currLevel))) {
      int posX = _random_pos_w(_gen);
      int speed = _random_speed(_gen);
      float angle{0.0f};

      if (posX <= (_width/2))
      {
          angle = glm::radians(static_cast<float>(_random_degree_right(_gen)));
      }
      else {
          angle = glm::radians(static_cast<float>(_random_degree_left(_gen)));
      }

      //Determine Random Type and Color of Meteor
      int type = _random_meteor(_gen);

      if (type > 5) //Small Rock
      {
          type = (type - 5);
          std::shared_ptr<Entity> asteroidEntity = GetEntityManager().AddEntity(GameConstants::kSmallMeteorName,GameConstants::SmallMeteorEntity,GameConstants::NpcLayer);
          asteroidEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH,GameConstants::kScaleGame);
          asteroidEntity->AddComponent<SpriteComponent>("smallrock" + std::to_string(type));
          asteroidEntity->AddComponent<ColliderComponent>(GameConstants::MeteorTag,posX,0,GameConstants::SmallMeteorSizeW,GameConstants::SmallMeteorSizeH);
      }
      else //Big Rock
      {
          std::shared_ptr<Entity> asteroidEntity = GetEntityManager().AddEntity(GameConstants::kBigMeteorName+std::to_string(type),GameConstants::BigMeteorEntity,GameConstants::NpcLayer);
          asteroidEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,60,35,GameConstants::kScaleGame);
          asteroidEntity->AddComponent<SpriteComponent>("bigrock" + std::to_string(type));      
          asteroidEntity->AddComponent<ColliderComponent>(GameConstants::MeteorTag,posX,0,GameConstants::BigMeteorSizeW,GameConstants::BigMeteorSizeH);
      }

      _fallingObjects++;

      _generateMeteorTime = 0.0f;
    }
}

void Game::generateSpinner(float deltaTime)
{
    _generateSpinnerTime += deltaTime * GetSpeedFactor();

    if ((_generateSpinnerTime > GameConstants::kTimeFreqSpinner) || (_fallingObjects < (GameConstants::kMinFallingObjects * _currLevel))) {
            int posX = _random_pos_w(_gen);
            int speed = _random_speed(_gen);
            float angle{0.0f};

            if (posX <= (_width/2))
            {
                angle = glm::radians(static_cast<float>(_random_degree_right(_gen)));
            }
            else {
                angle = glm::radians(static_cast<float>(_random_degree_left(_gen)));
            }

            //Determine if it'a a big spinner or small one (Random)
            int spinnerType = _random(_gen) % 2;

            if (spinnerType) {
                std::shared_ptr<Entity> spinnerEntity = GetEntityManager().AddEntity(GameConstants::kSmallSpinnerName, GameConstants::SmallSpinnerEntity, GameConstants::NpcLayer);
                spinnerEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,GameConstants::SmallSpinnerSize,GameConstants::SmallSpinnerSize,GameConstants::kScaleGame);
                spinnerEntity->AddComponent<SpriteComponent>("small-spinner",4,90);
                spinnerEntity->AddComponent<ColliderComponent>(GameConstants::SpinnerTag,posX,0,GameConstants::SmallSpinnerSize,GameConstants::SmallSpinnerSize);
            }
            else
            {
                std::shared_ptr<Entity>spinnerEntity = GetEntityManager().AddEntity(GameConstants::kBingSpinnerName, GameConstants::BigSpinnerEntity, GameConstants::NpcLayer);
                spinnerEntity->AddComponent<TransformComponent>(posX,0,glm::cos(angle)*speed,glm::sin(angle)*speed,GameConstants::BigSpinnerSize,GameConstants::BigSpinnerSize,GameConstants::kScaleGame);
                spinnerEntity->AddComponent<SpriteComponent>("big-spinner",4,90);
                spinnerEntity->AddComponent<ColliderComponent>(GameConstants::SpinnerTag,posX,0,GameConstants::BigSpinnerSize,GameConstants::BigSpinnerSize);
            }
            

            _fallingObjects++;
            _generateSpinnerTime = 0.0f;;
    }        
}

void Game::generateExplosion(glm::vec2 pos, const GameConstants::EntityType & entityType) {
  

        if (entityType == GameConstants::PlayerEntity) {
            pos.x = pos.x + 16; //TODO
            pos.y = pos.y; //TODO

            std::shared_ptr<Entity> explosion1 = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            std::shared_ptr<Entity> explosion2 = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            std::shared_ptr<Entity> explosion3 = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            std::shared_ptr<Entity> explosion4 = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            std::shared_ptr<Entity> explosion5 = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            
            explosion1->AddComponent<TransformComponent>(pos.x,pos.y,GameConstants::CanonExplosionHighSpeed,0,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
            explosion1->AddComponent<SpriteComponent>("explosion0");
            explosion1->SetLifeTime(GameConstants::kTimeRestartPlayer);
            
            explosion2->AddComponent<TransformComponent>(pos.x,pos.y,GameConstants::CanonExplosionLowSpeed,-GameConstants::CanonExplosionLowSpeed,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
            explosion2->AddComponent<SpriteComponent>("explosion0");
            explosion2->SetLifeTime(GameConstants::kTimeRestartPlayer);
            
            explosion3->AddComponent<TransformComponent>(pos.x,pos.y,0,-GameConstants::CanonExplosionHighSpeed,43,43,GameConstants::kScaleGame);
            explosion3->AddComponent<SpriteComponent>("explosion0");
            explosion3->SetLifeTime(GameConstants::kTimeRestartPlayer);
            
            explosion4->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CanonExplosionLowSpeed,-GameConstants::CanonExplosionLowSpeed,43,43,GameConstants::kScaleGame);
            explosion4->AddComponent<SpriteComponent>("explosion0");
            explosion4->SetLifeTime(GameConstants::kTimeRestartPlayer);

            explosion5->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CanonExplosionHighSpeed,0,43,43,GameConstants::kScaleGame);
            explosion5->AddComponent<SpriteComponent>("explosion0");
            explosion5->SetLifeTime(GameConstants::kTimeRestartPlayer);
        }
        else {
            //Create Entity Explosion
            std::shared_ptr<Entity> explosionEntity = GetEntityManager().AddEntity("explosion",GameConstants::ExplosionEntity, GameConstants::NpcLayer);
            explosionEntity->AddComponent<TransformComponent>(pos.x,pos.y,0,0,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
            explosionEntity->AddComponent<SpriteComponent>("explosion1");
            explosionEntity->SetLifeTime(GameConstants::kEntityLifeExplosion);
        }
}

void Game::killPlayer() {
    _lifes -= 1;
    _ticksLastKill = SDL_GetTicks();
}

void Game::restartPlayer() {
    _player = GetEntityManager().AddEntity("canon",GameConstants::PlayerEntity,GameConstants::PlayerLayer);
    _player->AddComponent<TransformComponent>(_width/2,_height+GameConstants::CanonDeltaY, 0, 0, GameConstants::CanonSize, GameConstants::CanonSize, GameConstants::kScaleGame);
    _player->AddComponent<SpriteComponent>("canon");
    _player->AddComponent<KeyboardControlComponent>("left","right","space");
    _player->AddComponent<ColliderComponent>(GameConstants::PlayerTag,_width/2,_height-12-32-64,32,32);
}

void Game::updateLife() {
        if (_score > 0 && (_score > _maxScore)) {
                int oldScore = _maxScore;
                _maxScore = _score;

                bool newLife = ((_maxScore/GameConstants::LifeScore) - (oldScore/GameConstants::LifeScore) > 0 ? true : false);

                if (newLife) 
                {
                    _lifes++;
                }
        }
}