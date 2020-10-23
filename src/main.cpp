#include <iostream>
#include "game.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{1024};
  constexpr std::size_t kScreenHeight{768};


  Game::GetGame().Init(kScreenWidth, kScreenHeight);

  Game::GetGame().Run(kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << Game::GetGame().GetScore() << "\n";
  
  return 0;
}