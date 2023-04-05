#pragma once

#include <glm/glm.hpp>

namespace glimac {

struct ShapeVertex {
    glm::fvec3 position;
    glm::fvec3 normal;
    glm::fvec2 texCoords;
};

} // namespace glimac
