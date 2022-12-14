#include "Boid.h"


Boid::Boid(glm::vec3 pos, glm::vec3 vel)
    : vPos(pos), vVel(vel)
{
    SetupMesh();
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
    // Vertex Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glGenBuffers(1, &EBO)

    // Vertex Array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    // TODO - Index buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER)

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    glBindVertexArray(0);
}


glm::mat4 Boid::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::translate(model, glm::vec3(2.0f, 20.0f, 0.0f));
    model = glm::translate(model, vPos);
    model = glm::scale(model, vScale);
    shader.SetMat4("model", model);

    shader.SetVec3("vColor", m_vColor);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return model;
}
