#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>

#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Initialization routine
    void init();
    // Updates and transformatinos applied to object
    void update(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void render();
    // Returns an objects transform
    Transform& getTransform();
private:
    // For now we have one shader per object.
    Shader myShader;
    // For now we have one buffer per object.
    Buffer myBuffer;
    // For now we have one texture per object
    Texture myTexture;
    // Store the objects transformations
    Transform transform; 
    // Store the 'camera'
    glm::mat4 projectionMatrix;
};


#endif
