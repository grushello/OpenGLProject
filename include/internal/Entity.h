#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "internal/Texture.h"
#include "internal/spriteRenderer.h"
#include  "internal/Resource_manager.h"

const float ENTITY_WIDTH = 64;
const float ENTITY_HEIGHT = 64;

enum EntityID
{
	ENTITY_NONE = 0,
	ENTITY_PLAYER = 1,
	ENTITY_BOX = 2,
	ENTITY_ENEMY = 3
};
class Entity
{
public:
	int ID = 0;
	glm::vec2   Position, Size;
	glm::vec3   Color;
	float       Rotation;

	Texture2D   Sprite;
	// constructor(s)
	Entity();
	Entity(glm::vec2 pos, glm::vec2 size, int ID, glm::vec3 color = glm::vec3(1.0f));
	// draw sprite
	virtual void Draw(SpriteRenderer& renderer);
};
#endif

