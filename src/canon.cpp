#include "game.h"
#include "canon.h"

void CanonGraphicsComponent::render(GameObject & obj, Renderer & renderer) 
{
    renderer.Render(_sprite, obj.GetX(),obj.GetY());
};

void PhysicsComponent::update(GameObject & obj, Game & game) {

}

void ControllerInputComponent::update(GameObject & obj, Game & game) {
    static float speed{0.1f};

    Controller::Direction direction = _controller.HandleInput(game.GetRunningStatus());

    if (direction != Controller::Direction::kNone) {
        switch(direction)
        {
            case Controller::Direction::kLeft :
                _currentDirection = Direction::kLeft;
            break;

            case Controller::Direction::kRight :
                _currentDirection = Direction::kRight;
            break;

            case Controller::Direction::kUp :
                game.GetSpeedFactor() *= 2.0;
            default:
            break;
        }
    }
    else
    {
        switch(_currentDirection)
        {
            case Direction::kLeft :
                obj._x -= speed*game.GetSpeedFactor();
            break;

            case Direction::kRight :
                obj._x += speed*game.GetSpeedFactor();;
            break;

            default:
            break;
        }
    }

    // Bounce the Canon around if going off of the screen.
    if ((obj.GetX()+obj.GetW()) >= (game.GetWidth())) {
      obj._x = game.GetWidth()-obj.GetW()-1;
      _currentDirection = Direction::kLeft;
    }
    else if ((obj.GetX()) <= 0) {
      obj._x = 1;
      _currentDirection = Direction::kRight;
    } 
    
}
