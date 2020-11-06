#ifndef BULLETEMITTERCOMPONENT_H
#define BULLETEMITTERCOMPONENT_H

#include "glm/glm.hpp"
#include "entitymanager.h"
#include "transformcomponent.h"

class BulletEmitterComponent: public Component {
    public:
        BulletEmitterComponent(int speed, int angleDeg, int range, bool loop);

        void Init() override;

        void Update(float deltaTime) override;

    private:
        std::shared_ptr<TransformComponent> transform_;
        glm::vec2 origin_;
        int speed_;
        int range_;
        float angleRad_;
        bool loop_;
};

#endif