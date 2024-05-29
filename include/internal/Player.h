#ifndef PLAYER_H
#define PLAYER_H

#include "internal/Entity.h"

enum PlayerActions
{
    PLAYER_UP,
    PLAYER_LEFT,
    PLAYER_DOWN,
    PLAYER_RIGHT
};

class Player : public Entity
{
public:
    Player(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));

    void loadTexture() override;
};

#endif