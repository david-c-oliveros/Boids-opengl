#ifndef QUAD_H
#define QUAD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"



class Quad
{
    private:
        glm::vec3 m_vColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 m_vOffset = glm::vec3(0.25f, 0.25f, 0.0f);

    public:
        static constexpr glm::vec3 vertices[6] =
        {
            glm::vec3(  0.0f, -0.5f, 0.0f ),
            glm::vec3(  0.0f,  0.0f, 0.0f ),
            glm::vec3( -0.5f, -0.5f, 0.0f ), 

            glm::vec3(  0.0f,  0.0f, 0.0f ),
            glm::vec3( -0.5f,  0.0f, 0.0f ),
            glm::vec3( -0.5f, -0.5f, 0.0f ) 
        };

        glm::vec3 vPos;

        unsigned int VBO, VAO, EBO;

        Quad(glm::vec3 pos);
        ~Quad();

        void SetColor(glm::vec3 color);
        void SetScale(glm::vec3 scale);

        void SetupMesh();
        void Draw(Shader &shader);
};

#endif
