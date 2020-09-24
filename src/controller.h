#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  enum class Direction { kNone, kUp, kDown, kLeft, kRight };
  Direction HandleInput(bool & running) const;
 private:
};

#endif