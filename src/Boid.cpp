#include "Boid.h"


Boid::Boid(glm::vec3 pos, glm::vec3 vel)
    : vPos(pos), vVel(vel)
{
}


Boid::~Boid()
{
}


void Boid::SetColor(glm::vec3 color)
{
    m_vColor = color;
}


void Boid::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO)

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // TODO - Index buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


glm::mat4 Boid::Draw(Shader &shader, float fRotAngle)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vPos);
    shader.SetMat4("model", model);

    shader.SetVec3("vColor", m_vColor);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return model;
}
