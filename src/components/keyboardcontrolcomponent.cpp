#include "entity.h"

#include "keyboardcontrolcomponent.h"

KeyboardControlComponent::KeyboardControlComponent(const std::string & leftKey, 
                        const std::string & rightKey, 
                        const std::string & shootKey) : _leftKey(GetSDLKeyStringCode(leftKey)),  
                                                        _rightKey(GetSDLKeyStringCode(rightKey)),
                                                        _shootKey(GetSDLKeyStringCode(shootKey)) {

}

std::string KeyboardControlComponent::GetSDLKeyStringCode(std::string key) {
    if (key.compare("left") == 0) return "1073741904";
    if (key.compare("right") == 0) return "1073741903";
    if (key.compare("space") == 0) return "32";
    return(std::to_string(static_cast<int>(key[0])));
}

void KeyboardControlComponent::Init() {
    _transform = GetOwner()->GetComponent<TransformComponent>();
}

void KeyboardControlComponent::Update(float deltaTime) {
    if (Game::GetGame().GetEvent().type == SDL_KEYDOWN) {
        std::string keyCode = std::to_string(Game::GetGame().GetEvent().key.keysym.sym);

        if (keyCode.compare(_leftKey) == 0) {
            _transform->SetVelocity(-GameConstants::PlayerSpeed,0);
        }

        if (keyCode.compare(_rightKey) == 0) {
            _transform->SetVelocity(GameConstants::PlayerSpeed,0);
        }
        
        if (keyCode.compare(_shootKey) == 0) {
            Game::GetGame().BulletEvent();
        }
    }

    if (Game::GetGame().GetEvent().type == SDL_KEYUP) {
        std::string keyCode = std::to_string(Game::GetGame().GetEvent().key.keysym.sym);

        if (keyCode.compare(_leftKey) == 0) {
            _transform->SetVelocity(0,0);                }

        if (keyCode.compare(_rightKey) == 0) {
            _transform->SetVelocity(0,0);
        }
    }
}