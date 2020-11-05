#include "entity.h"

#include "keyboardcontrolcomponent.h"

KeyboardControlComponent::KeyboardControlComponent( const std::string & leftKey, 
                                                    const std::string & rightKey, 
                                                    const std::string & shootKey) : leftKey_{GetSDLKeyStringCode(leftKey)},  
                                                                                    rightKey_{GetSDLKeyStringCode(rightKey)},
                                                                                    shootKey_{GetSDLKeyStringCode(shootKey)},
                                                                                    transform_{nullptr} {

}

const std::string KeyboardControlComponent::GetSDLKeyStringCode(const std::string & key) {
    if (key.compare("left") == 0) return GameConstants::kLeftKey;
    if (key.compare("right") == 0) return GameConstants::kRightKey;
    if (key.compare("space") == 0) return GameConstants::kSpaceKey;
    return(std::to_string(static_cast<int>(key[0])));
}

void KeyboardControlComponent::Init() {
    transform_ = GetOwner()->GetComponent<TransformComponent>();
}

void KeyboardControlComponent::Update(float deltaTime) {
    if (Game::GetGame().GetEvent().type == SDL_KEYDOWN) {
        std::string keyCode = std::to_string(Game::GetGame().GetEvent().key.keysym.sym);

        if (keyCode.compare(leftKey_) == 0) {
            transform_->SetVelocity(-GameConstants::PlayerSpeed,0);
        }

        if (keyCode.compare(rightKey_) == 0) {
            transform_->SetVelocity(GameConstants::PlayerSpeed,0);
        }
        
        if (keyCode.compare(shootKey_) == 0) {
            Game::GetGame().BulletEvent();
        }
    }

    if (Game::GetGame().GetEvent().type == SDL_KEYUP) {
        std::string keyCode = std::to_string(Game::GetGame().GetEvent().key.keysym.sym);

        if (keyCode.compare(leftKey_) == 0) {
            transform_->SetVelocity(0,0);                }

        if (keyCode.compare(rightKey_) == 0) {
            transform_->SetVelocity(0,0);
        }
    }
}