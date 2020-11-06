#include "entity.h"

#include "bulletemittercomponent.h"

BulletEmitterComponent::BulletEmitterComponent(int speed, int angleDeg, int range, bool loop) : speed_(speed), range_(range), 
                                                                        angleRad_(glm::radians(static_cast<float>(angleDeg))),
                                                                        loop_(loop) {

}

void BulletEmitterComponent::Init()  {
    transform_ = GetOwner()->GetComponent<TransformComponent>();
    origin_ = glm::vec2(transform_->GetPosition().x, transform_->GetPosition().y);
    transform_->SetVelocity(glm::cos(angleRad_) * speed_, glm::sin(angleRad_) * speed_); 
}

void BulletEmitterComponent::Update(float deltaTime)  {
    if (glm::distance(transform_->GetPosition(), origin_) > range_) {
        if (loop_) {
            transform_->SetPosition(origin_.x, origin_.y);
        } else {
            GetOwner()->Destroy();
        }
    } 
}