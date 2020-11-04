#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <string>

#include <SDL.h>

namespace GameConstants { 
     enum GameState {
          Running,
          GameOver
     };

     enum CollisionType {
          PlayerMeteor,
          PlayerSpinner,
          EnemyBullet,
          PlayerLeftBoundary,
          PlayerRightBoundary,
          MeteorGround,
          FallingObjectBoundary,
          SpinnerGround
     };

     enum ColliderTag {
          PlayerTag,
          MeteorTag,
          SpinnerTag,
          BulletTag,
          GroundTag,
          RightBoundaryTag,
          LeftBoundaryTag
     };

     enum LayerType {
          BackgroundLayer = 0,
          NpcLayer = 1,
          PlayerLayer = 2,
          BulletLayer = 3,
          UILayer = 4
     };

     enum LevelsScore {
          Level1 = 0,
          Level2 = 1000,
          Level3 = 5000,
          Level4 = 20000,
          Level5 = 50000,
          Level6 = 1000000
     };

     enum Score {
          LostBaseScore = -100,
          BigRockLandingScore = -5,
          SmallRockLandingScore = -10,
          BigRockScore = 10,
          SmallRockScore = 20,
          BigSpinnerScore = 40,
          SmallSpinnerScore = 80,
          LifeScore = 1000
     };

     enum Speed {
          PlayerSpeed = 300,
          BulletSpeed = 350,
          MeteorSpeed = 100,
          SpinnerSpeed = 100,
          CanonExplosionLowSpeed = 300,
          CanonExplosionHighSpeed = 800
     };

     const unsigned int kNumLayers = 6;

     const SDL_Color kWhiteColor = {255,255,255,255};

     enum EntityType {
          PlayerEntity,
          BigMeteorEntity,
          SmallMeteorEntity,
          TileEntity,
          CollideEntity,
          BigSpinnerEntity,
          SmallSpinnerEntity,
          BulletEntity,
          UIEntity,
          ExplosionEntity
     };

     enum EntitySize {
          CanonSize = 32,
          BulletSizeW = 7,
          BulletSizeH = 39,
          BigMeteorSizeW = 60,
          BigMeteorSizeH = 35,
          SmallMeteorSizeW = 30,
          SmallMeteorSizeH = 18,
          SmallSpinnerSize = 32,
          BigSpinnerSize = 64,
          ExplosionSize = 43,
          GroundSize = 2000,
          LeftBoundarySize = 500,
          RightBoundarySize = 500,
          LifeSpriteSize = 48
     };

     enum EntityPosition {
          CanonDeltaY = - 108,
          BulletDeltaX = 13,
          BulletDeltaY = -40,
          GroundDeltaY = -76,
          LeftBoundaryDeltaX = -500,
          ScoreLabelX = 32,
          ScoreLabelDeltaY = -64,
          LevelLabelDeltaX = -128,
          LevelLabelDeltaY = -64,
          LifeLabelDeltaY = -64,
          LifeSpriteDeltaX = 64,
          LifeSpriteDeltaY = -56
     };

     //Game System Constants
     constexpr std::size_t kFramesPerSecond{60};
     constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
     constexpr std::size_t kScreenWidth{1024};
     constexpr std::size_t kScreenHeight{768};

     //Min Falling Entities
     const int kMinFallingObjects = 3;

     //Entity Names
     const std::string kBigMeteorName = {"BIG_METEOR"};
     const std::string kSmallMeteorName = {"SMALL_METEOR"};
     const std::string kSmallSpinnerName = {"SMALL_SPINNER"};
     const std::string kBingSpinnerName = {"BIG_SPINNER"};

     //Entity Lifetime
     const float kEntityLifeInfinite{-1.0f};
     const float kEntityLifeExplosion{0.10f};

     //Scale
     const float kScaleGame{1.0f};

     //Map Size
     const int kMapTileSize{64};
     const int kMapWidth{16};
     const int kMapHeight{12};

     //Times Constant
     const float kTimeRestartPlayer{2.0f};
     const float kTimeFreqMeteor{2.0f};
     const float kTimeFreqSpinner{10.0f};

     //Speed Factor
     const float kSpeedFactorKillTransition{0.10f};
     const float kSpeedFactorLevel1{1.0f};
     const float kSpeedFactorLevel2{2.0f};
     const float kSpeedFactorLevel3{3.0f};
     const float kSpeedFactorLevel4{4.0f};
     const float kSpeedFactorLevel5{5.0f};
     const float kSpeedFactorLevel6{6.0f};

     //Angle Constant (degrees)
     const int kUpAngle{270};

     //SDL Keys
     const std::string kLeftKey{"1073741904"};
     const std::string kRightKey{"1073741903"};
     const std::string kSpaceKey{"32"}; 
};

#endif