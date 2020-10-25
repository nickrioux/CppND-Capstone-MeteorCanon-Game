#include "bulletemittercomponent.h"

BulletEmitterComponent::BulletEmitterComponent(int speed, int angleDeg, int range, bool loop) : _speed(speed), _range(range), 
                                                                        _angleRad(glm::radians(static_cast<float>(angleDeg))),
                                                                        _loop(loop) {

}

void BulletEmitterComponent::Init()  {
    _transform = GetOwner()->GetComponent<TransformComponent>();
    _origin = glm::vec2(_transform->GetPosition().x, _transform->GetPosition().y);
    _transform->SetVelocity(glm::cos(_angleRad) * _speed, glm::sin(_angleRad) * _speed); 
}

void BulletEmitterComponent::Update(float deltaTime)  {
    if (glm::distance(_transform->GetPosition(), _origin) > _range) {
        if (_loop) {
            _transform->SetPosition(_origin.x, _origin.y);
        } else {
            GetOwner()->Destroy();
        }
    } 
}