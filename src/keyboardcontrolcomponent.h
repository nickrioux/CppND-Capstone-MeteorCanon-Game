#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <iostream>

#include <SDL.h>

#include "game.h"
#include "component.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "bulletemittercomponent.h"
#include "collidercomponent.h"

class KeyboardControlComponent : public Component {
    public:

        KeyboardControlComponent() {

        }

        KeyboardControlComponent(std::string leftKey, 
                                std::string rightKey, 
                                std::string shootKey) {
            _leftKey = GetSDLKeyStringCode(leftKey);
            _rightKey = GetSDLKeyStringCode(rightKey);
            _shootKey = GetSDLKeyStringCode(shootKey);

        }

        std::string GetSDLKeyStringCode(std::string key) {
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32";
            return(std::to_string(static_cast<int>(key[0])));
        }

        void Init() override {
            _transform = GetOwner()->GetComponent<TransformComponent>();
            _sprite = GetOwner()->GetComponent<SpriteComponent>();
        }

        void Update(float deltaTime) override {
            if (Game::GetGame()->GetEvent()->type == SDL_KEYDOWN) {
                std::string keyCode = std::to_string(Game::GetGame()->GetEvent()->key.keysym.sym);

                if (keyCode.compare(_leftKey) == 0) {
                    _transform->GetVelocity().x = -100;
                    _transform->GetVelocity().y = 0;
                }

                if (keyCode.compare(_rightKey) == 0) {
                    _transform->GetVelocity().x = 100;
                    _transform->GetVelocity().y = 0;
                }
                
                if (keyCode.compare(_shootKey) == 0) {
                    Entity & bulletEntity = Game::GetGame()->GetEntityManager()->AddEntity("bullet",GameConstants::BULLET_ENTITY,GameConstants::BULLET_LAYER);
                    bulletEntity.AddComponent<TransformComponent>(_transform->GetPosition().x+13,_transform->GetPosition().y-40, 0, 0, 0, 0, 1.0);
                    bulletEntity.AddComponent<SpriteComponent>("bullet");
                    bulletEntity.AddComponent<ColliderComponent>("BULLET",_transform->GetPosition().x+13,_transform->GetPosition().y-40,_transform->GetWidth(),_transform->GetHeight());
                    bulletEntity.AddComponent<BulletEmitterComponent>(200,270,Game::GetGame()->GetHeight()-40,false);
                }
            }

            if (Game::GetGame()->GetEvent()->type == SDL_KEYUP) {
                std::string keyCode = std::to_string(Game::GetGame()->GetEvent()->key.keysym.sym);

                if (keyCode.compare(_leftKey) == 0) {
                    _transform->GetVelocity().x = 0;
                    _transform->GetVelocity().y = 0;
                }

                if (keyCode.compare(_rightKey) == 0) {
                    _transform->GetVelocity().x = 0;
                    _transform->GetVelocity().y = 0;
                }
            }
        }

    private:

        std::string _leftKey;
        std::string _rightKey;
        std::string _shootKey;
        TransformComponent * _transform;
        SpriteComponent * _sprite;
};

#endif