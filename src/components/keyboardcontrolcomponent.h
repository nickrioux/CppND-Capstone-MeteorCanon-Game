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
    
        KeyboardControlComponent(const std::string & leftKey, 
                                const std::string & rightKey, 
                                const std::string & shootKey);

        const std::string GetSDLKeyStringCode(const std::string & key);

        void Init() override;

        void Update(float deltaTime) override;

    private:

        std::string leftKey_;
        std::string rightKey_;
        std::string shootKey_;
        std::shared_ptr<TransformComponent> transform_;
};

#endif