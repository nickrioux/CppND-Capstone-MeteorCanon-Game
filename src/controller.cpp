#include "controller.h"
#include <iostream>
#include "SDL.h"

Controller::Direction Controller::HandleInput(bool & running) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
      return(Controller::Direction::kNone);
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          return(Controller::Direction::kLeft);
          break;
        case SDLK_RIGHT:
          return(Controller::Direction::kRight);
          break;
        case SDLK_UP:
          return(Controller::Direction::kUp);
          break;
        default:
          return(Controller::Direction::kNone);
          break;
      }
    }
  }
  return(Controller::Direction::kNone);
}