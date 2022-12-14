#include "math_util.h"

namespace math_util
{
    float remap(float input, float low1, float high1, float low2, float high2)
    {
        return low2 + (input - low1) * (high2 - low2) / (high1 - low1);
    }


    glm::vec3 remap(glm::vec3 input, float low1, float high1, float low2, float high2)
    {
        return glm::vec3(low2 + (input.x - low1) * (high2 - low2) / (high1 - low1),
                         low2 + (input.y - low1) * (high2 - low2) / (high1 - low1),
                         low2 + (input.z - low1) * (high2 - low2) / (high1 - low1));
    }
}
