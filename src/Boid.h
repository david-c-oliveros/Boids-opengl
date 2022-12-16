#ifndef BOID_H
#define BOID_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

typedef struct Vertex
{
    glm::vec3 pos;
} Vertex;



class Boid
{
    private:
        glm::vec3 m_vColor;
        glm::vec3 vScale = glm::vec3(1.0f, 1.0f, 1.0f);

    public:
        static constexpr Vertex vertices[3] =
        {
            { { -0.5f, -0.5f, 0.0f } },
            { {  0.5f, -0.5f, 0.0f } },
            { {  0.0f,  0.5f, 0.0f } } 
        };

//        static constexpr Vertex vertices[3] =
//        {
//            { { -0.5f, -0.5f, 0.0f } },
//            { {  0.5f, -0.5f, 0.0f } },
//            { {  0.0f,  0.5f, 0.0f } } 
//        };

        static constexpr float triangle[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        static constexpr float cube[] = {
            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f, -0.5f,  0.5f, 
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f, -0.5f,  0.5f, 

            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 

             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  

            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f,  0.5f, -0.5f, 
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
        };
        glm::vec3 vPos;
        glm::vec3 vVel;

        unsigned int VBO, VAO, EBO;

        Boid(glm::vec3 pos, glm::vec3 vel);
        ~Boid();

        void SetColor(glm::vec3 color);

        void SetupMesh();
        void Draw(Shader &shader);
};

#endif
