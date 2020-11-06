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

Game::Game() :  gen_(rd_()), sdl_window_(nullptr,SDL_DestroyWindow), sdl_renderer_(nullptr,SDL_DestroyRenderer),
                entityManager_(std::make_unique<EntityManager>()), assetManager_(std::make_unique<AssetManager>()) {

}

Game::~Game() {
    //Implicit call destruction for the window and renderer object before SDL_Quit
    sdl_window_.reset(nullptr);
    sdl_renderer_.reset(nullptr);
    SDL_Quit();
}

void Game::Init(int width, int height) {
    
    width_ = width;
    height_ = height;

    camera_ = {0, 0, width_, height_};

    random_degree_left_ = std::uniform_int_distribution<>(90,105);
    random_degree_right_ = std::uniform_int_distribution<>(85,90);
    random_speed_  = std::uniform_int_distribution<>(20,100);
    random_pos_w_  = std::uniform_int_distribution<>(0,width_);
    random_meteor_  = std::uniform_int_distribution<>(0,10);
    random_ = std::uniform_int_distribution<>();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    sdl_window_.reset(SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_,
        height_,
        SDL_WINDOW_BORDERLESS
    ));

    if (!sdl_window_.get()) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    sdl_renderer_.reset(SDL_CreateRenderer(sdl_window_.get(), -1, 0));

    if (!sdl_renderer_.get()) {
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
  return score_;
}

void Game::BulletEvent() {
    eventBullet_ = true;
}

void Game::loadGame() {
    //Init Score and Level
    score_ = 0;
    currLevel_ = 1;

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
    map_ = std::make_unique<Map>("skybox1",GameConstants::kScaleGame,GameConstants::kMapTileSize);
    map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);

    //Init Player
    restartPlayer();

    std::shared_ptr<Entity> entity = GetEntityManager().AddEntity("left_boundary",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(GameConstants::LeftBoundaryDeltaX,0,0,0,GameConstants::LeftBoundarySize,height_,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::LeftBoundaryTag,GameConstants::LeftBoundaryDeltaX,0,GameConstants::LeftBoundarySize,height_);

    entity = GetEntityManager().AddEntity("right_boundary",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(width_,0,0,0,GameConstants::RightBoundarySize,height_,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::RightBoundaryTag,width_,0,GameConstants::RightBoundarySize,height_);

    entity= GetEntityManager().AddEntity("ground",GameConstants::CollideEntity,GameConstants::PlayerLayer);
    entity->AddComponent<TransformComponent>(0,height_+GameConstants::GroundDeltaY,0,0,width_,GameConstants::GroundSize,GameConstants::kScaleGame);
    entity->AddComponent<ColliderComponent>(GameConstants::GroundTag,0,height_+GameConstants::GroundDeltaY,width_,GameConstants::GroundSize);

    scoreLabel_ = GetEntityManager().AddEntity("scoreLabel",GameConstants::UIEntity,GameConstants::UILayer);
    scoreLabel_->AddComponent<TextLabelComponent>(GameConstants::ScoreLabelX,height_+GameConstants::ScoreLabelDeltaY,"", "charriot-font", GameConstants::kWhiteColor);

    levelLabel_ = GetEntityManager().AddEntity("levelLabel",GameConstants::UIEntity,GameConstants::UILayer);
    levelLabel_->AddComponent<TextLabelComponent>(width_+GameConstants::LevelLabelDeltaX,height_+GameConstants::LevelLabelDeltaY,"","charriot-font", GameConstants::kWhiteColor);

    lifeLabel_ = GetEntityManager().AddEntity("lifeLabel",GameConstants::UIEntity, GameConstants::UILayer);
    lifeLabel_->AddComponent<TextLabelComponent>(width_/2, height_+GameConstants::LifeLabelDeltaY,"","charriot-font", GameConstants::kWhiteColor);
    lifeSprite_ = GetEntityManager().AddEntity("lifeSprite",GameConstants::UIEntity, GameConstants::UILayer);
    lifeSprite_->AddComponent<TransformComponent>(  width_/2+GameConstants::LifeSpriteDeltaX, height_+GameConstants::LifeSpriteDeltaY,0,0,GameConstants::LifeSpriteSize,
                                                    GameConstants::LifeSpriteSize,GameConstants::kScaleGame);   
    lifeSprite_->AddComponent<SpriteComponent>("canon-life"); 
}

void Game::processInput() {

    SDL_PollEvent(&event_);
    switch (GetEvent().type) {
        case SDL_QUIT: {
            runningStatus_ = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (GetEvent().key.keysym.sym == SDLK_ESCAPE) {
                runningStatus_ = false;
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
    int delayTime = target_frame_duration - (SDL_GetTicks() - ticksLastFrame_);
    
    if (delayTime > 0 && delayTime <= target_frame_duration) {
        SDL_Delay(delayTime);
    }

    // Delta time is the difference in ticks from last frame converted to secomds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame_) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame_ = SDL_GetTicks();

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
        if (ticksLastKill_ != 0)
        {
                float killDeltaTime = (ticksLastFrame_ - ticksLastKill_)/1000.0f;

                speedFactor_ = GameConstants::kSpeedFactorKillTransition;

                if (killDeltaTime > GameConstants::kTimeRestartPlayer) {
                    ticksLastKill_ = 0;
                    restartPlayer();
                }
        }
    }

    //Update UI
    updateUI();

    //Game Over
    if ((ticksLastKill_ == 0) && (lifes_ == 0) && (gameState == GameConstants::GameState::Running)) //GameOver
    {
        gameState = GameConstants::GameState::GameOver;
        speedFactor_ = GameConstants::kSpeedFactorLevel1;
        gameOverLabel_ = GetEntityManager().AddEntity("gameOverLabel", GameConstants::UIEntity, GameConstants::UILayer);
        gameOverLabel_->AddComponent<TextLabelComponent>(width_/3,height_/3,"GAME OVER!","charriot-font", GameConstants::kWhiteColor);
    }

}

void Game::updateLevel() {

    if (score_ >= GameConstants::Level6) {  
        
        if (currLevel_ !=6) {
            //Change Map
            map_ = std::make_unique<Map>("skybox6",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        currLevel_ = 6;
        speedFactor_ = GameConstants::kSpeedFactorLevel6;
    }
    else if (score_ >= GameConstants::Level5) {
        if (currLevel_ != 5) {
            //Change Map
            map_ = std::make_unique<Map>("skybox5",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        currLevel_ = 5;
        speedFactor_ = GameConstants::kSpeedFactorLevel5;
    }
    else if (score_ >= GameConstants::Level4) {
        if (currLevel_ != 4) {
            //Change Map
            map_ = std::make_unique<Map>("skybox4",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        currLevel_ = 4;
        speedFactor_ = GameConstants::kSpeedFactorLevel4;
    }
    else if (score_ >= GameConstants::Level3) {
        if (currLevel_ != 3) {
            //Change Map
            map_ = std::make_unique<Map>("skybox3",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }
        currLevel_ = 3;
        speedFactor_ = GameConstants::kSpeedFactorLevel3;
    }
    else if (score_ >= GameConstants::Level2) {        
        if (currLevel_ != 2) {
            //Change Map
            map_ = std::make_unique<Map>("skybox2",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        currLevel_ = 2;
        speedFactor_ = GameConstants::kSpeedFactorLevel2;
    }
    else {
        if (currLevel_ != 1) {
            //Change Map
            map_ = std::make_unique<Map>("skybox1",GameConstants::kScaleGame,GameConstants::kMapTileSize);
            map_->LoadMap("../assets/skybox1.map",GameConstants::kMapWidth,GameConstants::kMapHeight);
        }

        currLevel_ = 1;
        speedFactor_ = GameConstants::kSpeedFactorLevel1;
    }
}

void Game::updateUI() {
  std::shared_ptr<TextLabelComponent> scoreLabel = scoreLabel_->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> levelLabel = levelLabel_->GetComponent<TextLabelComponent>();
  std::shared_ptr<TextLabelComponent> lifeLabel = lifeLabel_->GetComponent<TextLabelComponent>();

  std::string score = std::to_string(score_);
  scoreLabel->SetLabelText(score,"charriot-font");

  std::string level = std::to_string(currLevel_) + "x";
  levelLabel->SetLabelText(level,"charriot-font");

  std::string life = std::to_string(lifes_) + "x";
  lifeLabel->SetLabelText(life,"charriot-font");
}

//The Camera is fixed for this game.
void Game::handleCameraMovement() {
    camera_.x = 0;
    camera_.y = 0;
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
                        generateExplosion(player_->GetComponent<TransformComponent>()->GetPosition(),player_->GetEntityType());
                        collisionData.entityOne->Destroy();
                        collisionData.entityTwo->Destroy();
                        killPlayer();
                        fallingObjects_--;
                        break;
                    case GameConstants::PlayerLeftBoundary:
                    case GameConstants::PlayerRightBoundary:
                        adjustCanonPosition(collisionData.collisionType);
                        break;
                    case GameConstants::MeteorGround: {
                        std::shared_ptr<Entity> thatEntity = (collisionData.entityOne->GetEntityType() == GameConstants::CollideEntity ? collisionData.entityTwo : collisionData.entityOne);
                        if (thatEntity->GetEntityType() == GameConstants::BigMeteorEntity)
                        {
                            score_ += GameConstants::BigRockLandingScore * currLevel_;
                        }
                        else
                        {
                            score_ += GameConstants::SmallRockLandingScore * currLevel_;
                        }
                        generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                        thatEntity->Destroy();
                        fallingObjects_--;
                        break;
                    };
                    case GameConstants::EnemyBullet: {
                        std::shared_ptr<Entity> thatEntity = (collisionData.entityOne->GetEntityType() == GameConstants::BulletEntity ? collisionData.entityTwo : collisionData.entityOne);
                        switch(thatEntity->GetEntityType()) {
                                case GameConstants::BigMeteorEntity:
                                case GameConstants::SmallMeteorEntity:
                                    processMeteorExplode(thatEntity);
                                    fallingObjects_--;
                                    break;
                                case GameConstants::SmallSpinnerEntity:
                                    score_ += GameConstants::SmallSpinnerScore * currLevel_;
                                    generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                                    fallingObjects_--;
                                    break;
                                case GameConstants::BigSpinnerEntity:
                                    score_ += GameConstants::BigSpinnerScore * currLevel_;
                                    generateExplosion(thatEntity->GetComponent<TransformComponent>()->GetPosition(),thatEntity->GetEntityType());
                                    fallingObjects_--;
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
                            fallingObjects_--;
                        break;
                    case GameConstants::PlayerSpinner:
                    case GameConstants::SpinnerGround:
                        score_ += GameConstants::LostBaseScore;
                        generateExplosion(player_->GetComponent<TransformComponent>()->GetPosition(),player_->GetEntityType());
                        
                        if ((collisionData.entityOne->GetEntityType() == GameConstants::SmallSpinnerEntity) || (collisionData.entityOne->GetEntityType() == GameConstants::BigSpinnerEntity)) {
                            generateExplosion(collisionData.entityOne->GetComponent<TransformComponent>()->GetPosition(),collisionData.entityOne->GetEntityType());
                            collisionData.entityOne->Destroy(); //Spinner
                        }
                        else {
                            generateExplosion(collisionData.entityTwo->GetComponent<TransformComponent>()->GetPosition(),collisionData.entityTwo->GetEntityType());
                            collisionData.entityTwo->Destroy(); //Spinner
                        }
                        fallingObjects_--;
                        player_->Destroy();
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
        score_ += GameConstants::SmallRockScore * currLevel_;
    }    
    else if (random_(gen_) % 2)
    {
        //Generate Two Small Meteor
        glm::vec2 pos = meteor->GetComponent<TransformComponent>()->GetPosition();
        glm::vec2 speed = meteor->GetComponent<TransformComponent>()->GetVelocity();

        glm::vec2 posSmall1 = {pos.x + GameConstants::BigMeteorSizeW * 1/4,pos.y + GameConstants::BigMeteorSizeH/2 - GameConstants::SmallMeteorSizeH/2};
        glm::vec2 posSmall2 = {pos.x + GameConstants::BigMeteorSizeW * 3/4,pos.y + GameConstants::BigMeteorSizeH/2 - GameConstants::SmallMeteorSizeH/2};
        std::uniform_int_distribution<> speedRnd(-40, -20);
        glm::vec2 speed1 = {speedRnd(gen_), speed.y};
        glm::vec2 speed2 = {-speed1.x, speed.y};

        // Rotate the speeds of the small rocks some times:
        int dir = random_(gen_) % 3;
        for (int times = random_(gen_) % 2 + 1; times > 0; times--) {
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
                
        fallingObjects_+=2;
    }
    else {
        generateExplosion(meteor->GetComponent<TransformComponent>()->GetPosition(),meteor->GetEntityType());
        score_ += GameConstants::BigRockScore * currLevel_;
    }
}

void Game::generateBullet() {
    if (eventBullet_) {
        std::shared_ptr<TransformComponent> transformComp = player_->GetComponent<TransformComponent>();

        std::shared_ptr<Entity>  bulletEntity = GetEntityManager().AddEntity("bullet",GameConstants::BulletEntity,GameConstants::BulletLayer);
        bulletEntity->AddComponent<TransformComponent>( transformComp->GetPosition().x+GameConstants::BulletDeltaX,transformComp->GetPosition().y+GameConstants::BulletDeltaY, 0, 
                                                        -GameConstants::BulletSpeed, GameConstants::BulletSizeW, GameConstants::BulletSizeH, GameConstants::kScaleGame);
        bulletEntity->AddComponent<SpriteComponent>("bullet");
        bulletEntity->AddComponent<ColliderComponent>(  GameConstants::BulletTag,transformComp->GetPosition().x+GameConstants::BulletDeltaX,transformComp->GetPosition().y+GameConstants::BulletDeltaY,
                                                        transformComp->GetWidth(),transformComp->GetHeight());
        bulletEntity->AddComponent<BulletEmitterComponent>(GameConstants::BulletSpeed,GameConstants::kUpAngle,height_+GameConstants::BulletDeltaY,false);
    
        eventBullet_ = false;
    }
}


void Game::render() {
    SDL_SetRenderDrawColor(sdl_renderer_.get(), 21, 21, 21, 255);
    SDL_RenderClear(sdl_renderer_.get());

    if (GetEntityManager().HasNoEntities()) {
        std::cout << "NO ENTITIES RENDER\n" << std::endl;
        return;
    }

    GetEntityManager().Render(); 

    SDL_RenderPresent(sdl_renderer_.get());  
}

void Game::destroy() {

}

void Game::adjustCanonPosition(GameConstants::CollisionType collisionType) {
    
    std::shared_ptr<TransformComponent> transform = player_->GetComponent<TransformComponent>();

    if (collisionType == GameConstants::PlayerLeftBoundary) {
        transform->SetPosition(0,transform->GetPosition().y);
    }
    else if (collisionType == GameConstants::PlayerRightBoundary)
    {
        transform->SetPosition(width_ - transform->GetWidth(), transform->GetPosition().y);
    }
   
}

void Game::generateMeteor(float deltaTime) {

    //Each TIME_FREQ_METEOR seconds generate a new Meteor

    generateMeteorTime_ += deltaTime * GetSpeedFactor();

    if ((generateMeteorTime_ > (GameConstants::kTimeFreqMeteor)) || (fallingObjects_ < (GameConstants::kMinFallingObjects * currLevel_))) {
      int posX = random_pos_w_(gen_);
      int speed = random_speed_(gen_);
      float angle{0.0f};

      if (posX <= (width_/2))
      {
          angle = glm::radians(static_cast<float>(random_degree_right_(gen_)));
      }
      else {
          angle = glm::radians(static_cast<float>(random_degree_left_(gen_)));
      }

      //Determine Random Type and Color of Meteor
      int type = random_meteor_(gen_);

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

      fallingObjects_++;

      generateMeteorTime_ = 0.0f;
    }
}

void Game::generateSpinner(float deltaTime)
{
    generateSpinnerTime_ += deltaTime * GetSpeedFactor();

    if ((generateSpinnerTime_ > GameConstants::kTimeFreqSpinner) || (fallingObjects_ < (GameConstants::kMinFallingObjects * currLevel_))) {
            int posX = random_pos_w_(gen_);
            int speed = random_speed_(gen_);
            float angle{0.0f};

            if (posX <= (width_/2))
            {
                angle = glm::radians(static_cast<float>(random_degree_right_(gen_)));
            }
            else {
                angle = glm::radians(static_cast<float>(random_degree_left_(gen_)));
            }

            //Determine if it'a a big spinner or small one (Random)
            int spinnerType = random_(gen_) % 2;

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
            

            fallingObjects_++;
            generateSpinnerTime_ = 0.0f;;
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
            
            explosion3->AddComponent<TransformComponent>(pos.x,pos.y,0,-GameConstants::CanonExplosionHighSpeed,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
            explosion3->AddComponent<SpriteComponent>("explosion0");
            explosion3->SetLifeTime(GameConstants::kTimeRestartPlayer);
            
            explosion4->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CanonExplosionLowSpeed,-GameConstants::CanonExplosionLowSpeed,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
            explosion4->AddComponent<SpriteComponent>("explosion0");
            explosion4->SetLifeTime(GameConstants::kTimeRestartPlayer);

            explosion5->AddComponent<TransformComponent>(pos.x,pos.y,-GameConstants::CanonExplosionHighSpeed,0,GameConstants::ExplosionSize,GameConstants::ExplosionSize,GameConstants::kScaleGame);
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
    lifes_ -= 1;
    ticksLastKill_ = SDL_GetTicks();
}

void Game::restartPlayer() {
    player_ = GetEntityManager().AddEntity("canon",GameConstants::PlayerEntity,GameConstants::PlayerLayer);
    player_->AddComponent<TransformComponent>(width_/2,height_+GameConstants::CanonDeltaY, 0, 0, GameConstants::CanonSize, GameConstants::CanonSize, GameConstants::kScaleGame);
    player_->AddComponent<SpriteComponent>("canon");
    player_->AddComponent<KeyboardControlComponent>("left","right","space");
    player_->AddComponent<ColliderComponent>(GameConstants::PlayerTag,width_/2,height_+GameConstants::CanonDeltaY,GameConstants::CanonSize,GameConstants::CanonSize);
}

void Game::updateLife() {
        if (score_ > 0 && (score_ > maxScore_)) {
                int oldScore = maxScore_;
                maxScore_ = score_;

                bool newLife = ((maxScore_/GameConstants::LifeScore) - (oldScore/GameConstants::LifeScore) > 0 ? true : false);

                if (newLife) 
                {
                    lifes_++;
                }
        }
}