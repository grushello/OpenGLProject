#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Camera2D {
public:
    Camera2D(glm::vec2 position = glm::vec2(0.0f), float zoom = 1.0f, float width = 800, float height = 600);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseScroll(float yoffset);

    void setPosition(glm::vec2 newPosition);
    glm::vec2 getPosition();
private:
    glm::vec2 Position;
    float Zoom;
    float Width;
    float Height;

    float MovementSpeed = 350.0f;
    float ZoomSpeed = 0.08f;

    void updateCameraVectors();
};
#endif