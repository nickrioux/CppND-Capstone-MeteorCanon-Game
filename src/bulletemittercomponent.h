#ifndef BULLETEMITTERCOMPONENT_H
#define BULLETEMITTERCOMPONENT_H

#include "glm/glm.hpp"
#include "entitymanager.h"
#include "transformcomponent.h"

class BulletEmitterComponent: public Component {
    public:
        BulletEmitterComponent(int speed, int angleDeg, int range, bool loop) : _speed(speed), _range(range), _loop(loop) {
            _angleRad = glm::radians(static_cast<float>(angleDeg));
        }

        void Init() override {
            _transform = GetOwner()->GetComponent<TransformComponent>();
            _origin = glm::vec2(_transform->GetPosition().x, _transform->GetPosition().y);
            _transform->GetVelocity() = glm::vec2(glm::cos(_angleRad) * _speed, glm::sin(_angleRad) * _speed); 
        }

        void Update(float deltaTime) override {
            if (glm::distance(_transform->GetPosition(), _origin) > _range) {
                if (_loop) {
                    _transform->GetPosition().x = _origin.x;
                    _transform->GetPosition().y = _origin.y;
                } else {
                    GetOwner()->Destroy();
                }
            } 
        }

    private:
        TransformComponent *_transform;
        glm::vec2 _origin;
        int _speed;
        int _range;
        float _angleRad;
        bool _loop;
};

#endif