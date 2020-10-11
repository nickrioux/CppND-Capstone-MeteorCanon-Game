#include <iostream>
#include "game.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{480};
  constexpr std::size_t kGridWidth{128};
  constexpr std::size_t kGridHeight{128};

  Game *game = Game::GetGame();

  game->Init(kScreenWidth, kScreenHeight);

  game->Run(kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game->GetScore() << "\n";
  
  return 0;
}