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
        std::shared_ptr<TransformComponent> _transform;
        glm::vec2 _origin;
        int _speed;
        int _range;
        float _angleRad;
        bool _loop;
};

#endif