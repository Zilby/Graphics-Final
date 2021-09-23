#include "Camera.h"

#include "glm/gtx/transform.hpp"

#include <iostream>

Camera& Camera::instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::mouseLook(int mouseX, int mouseY){
    // TODO: Which vector are you modifying?
	// *hint* not the eye...
    float scalar = 0.01f;
    glm::mat4 rotation = glm::rotate((oldMousePosition.x - mouseX) * scalar, upVector);    
    viewDirection = glm::mat3(rotation)* viewDirection;
    // rotation = glm::rotate((oldMousePosition.y - mouseY) * scalar, glm::vec3(1,0,0));
    // viewDirection = glm::mat3(rotation)* viewDirection;
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
