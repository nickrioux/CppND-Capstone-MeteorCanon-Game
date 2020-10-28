#include <iostream>
#include "gameconstants.h"
#include "game.h"

int main() {
  Game::GetGame().Init(GameConstants::kScreenWidth, GameConstants::kScreenHeight);

  Game::GetGame().Run(GameConstants::kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << Game::GetGame().GetScore() << "\n";
  
  return 0;
}