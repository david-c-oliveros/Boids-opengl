#ifndef BOID_H
#define BOID_H

#include <glm/glm.hpp>

class Boid
{
    public:
        glm::vec2 vPos;
        glm::vec2 vVel;

        Boid(glm::vec2 pos, glm::vec2 vel);
        ~Boid();

        void Update();
};

#endif
