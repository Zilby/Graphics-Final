#include "Camera.h"

#include "glm/gtx/transform.hpp"

#include <iostream>

Camera& Camera::instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::mouseLook(int mouseX, int mouseY){
    int diffX = mouseX - oldMousePosition.x;

    // Convert to a vec4
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::vec4 view4 = glm::vec4(viewDirection.x, viewDirection.y, viewDirection.z, 1.0f);
    matrix = glm::rotate(matrix, (float) (diffX / -140.0f), upVector);
    view4 =  matrix * view4;

    // Convert back to a vec3
    viewDirection.x = view4.x;
    viewDirection.y = view4.y;
    viewDirection.z = view4.z;

    // Update the mouse position
    oldMousePosition = glm::vec2(mouseX, mouseY);
}

void Camera::moveForward(float speed){
    eyePosition.z -= speed;
}

void Camera::moveBackward(float speed){
    eyePosition.z += speed;
}

void Camera::moveLeft(float speed){
    eyePosition.x -= speed;
}

void Camera::moveRight(float speed){
    eyePosition.x += speed;
}

void Camera::moveUp(float speed){
    eyePosition.y += speed;
}

void Camera::moveDown(float speed){
    eyePosition.y -= speed;
}

float Camera::getEyeXPosition(){
    return eyePosition.x;
}

float Camera::getEyeYPosition(){
    return eyePosition.y;
}

float Camera::getEyeZPosition(){
    return eyePosition.z;
}

float Camera::getViewXDirection(){
    return viewDirection.x;
}

float Camera::getViewYDirection(){
    return viewDirection.y;
}

float Camera::getViewZDirection(){
    return viewDirection.z;
}

Camera::Camera(){
    std::cout << "Created a Single Camera!\n";
	// Position us at the origin.
    eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

}

glm::mat4 Camera::getWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( eyePosition,
                        eyePosition + viewDirection,
                        upVector);
}
