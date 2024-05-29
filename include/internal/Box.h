#ifndef BOX_H
#define BOX_H

#include "internal/Entity.h"

class Box : public Entity
{
public:
    Box(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));

    void loadTexture() override;
};

#endif

