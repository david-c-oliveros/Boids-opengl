#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <glm/glm.hpp>

namespace math_util
{
    float remap(float input, float low1, float high1, float low2, float high2);
    glm::vec3 remap(glm::vec3 input, float low1, float high1, float low2, float high2);

    glm::vec3 vec_plane_intersect(glm::vec3 plane_p, glm::vec3 plane_n, glm::vec3 line_start, glm::vec3 line_end);
}

#endif
