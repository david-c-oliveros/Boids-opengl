#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"


struct Character
{
    unsigned int TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;          // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;     // Offset to advance to next glyph
};


class Text
{
    public:
        unsigned int VAO, VBO;
        std::map<char, Character> Characters;
        glm::mat4 projection;

    public:
        Text(int scrWidth, int scrHeight);
        ~Text();

        void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

    private:
        bool initialize();
        void setupBuffers();
};

#endif
