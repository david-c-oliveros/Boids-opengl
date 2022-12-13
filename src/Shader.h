#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
    public:
        unsigned int ID;
        const char* vertexName;
        const char* fragmentName;

        Shader();
        ~Shader();

        void Create(const char* vertexPath, const char* fragmentPath);
        void OpenShaderFile(const char* vertexPath);
        void Use();
        void PrintName();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec3(const std::string &name, glm::vec3 vec) const;
        void SetMat4(const std::string &name, glm::mat4 matrix) const;
};


#endif
