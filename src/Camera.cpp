#include "internal/Camera.h"
#include "iostream"

Camera2D::Camera2D(glm::vec2 position, float zoom, float width, float height)
    : Position(position), Zoom(zoom), Width(width), Height(height) {}

glm::mat4 Camera2D::getViewMatrix() const
{
    return glm::ortho(Position.x - Zoom, Position.x + Zoom, Position.y - Zoom, Position.y + Zoom, -1.0f, 1.0f);
}
glm::mat4 Camera2D::getProjectionMatrix() const
{
    glm::mat4 projectionMatrix = glm::ortho(0.0f, Width, Height, 0.0f, -1.0f, 1.0f);
    return projectionMatrix;
}

void Camera2D::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == CAMERA_UP)
        Position.y -= velocity;
    if (direction == CAMERA_DOWN)
        Position.y += velocity;
    if (direction == CAMERA_LEFT)
        Position.x -= velocity;
    if (direction == CAMERA_RIGHT)
        Position.x += velocity;
}

void Camera2D::processMouseScroll(float yoffset) 
{
    Zoom -= yoffset * ZoomSpeed;
    if (Zoom < 0.5f)
        Zoom = 0.5f;
    if (Zoom > 2.0f)
        Zoom = 2.0f;
}