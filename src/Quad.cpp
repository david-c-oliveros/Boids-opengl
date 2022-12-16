#include "Quad.h"


Quad::Quad(glm::vec3 pos)
    : vPos(pos)
{
    SetupMesh();
}


Quad::~Quad()
{
}


void Quad::SetColor(glm::vec3 color)
{
    m_vColor = color;
}


void Quad::SetScale(glm::vec3 scale)
{
    m_vScale = scale;
}


void Quad::SetupMesh()
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}


void Quad::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::translate(model, glm::vec3(2.0f, 20.0f, 0.0f));
    model = glm::translate(model, vPos);
    model = glm::scale(model, m_vScale);
    model = glm::translate(model, m_vOffset);
    shader.SetMat4("model", model);

    shader.SetVec3("vColor", m_vColor);

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
