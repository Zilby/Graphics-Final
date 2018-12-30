/** @file Object.h
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>
#include <string>

#include "Buffer.h"
#include "Texture.h"
#include "Geometry.h"
#include "ObjParser.h"

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
    // The method can be overridden by other primitives.
    void LoadObject(std::string objPath);
    // How to draw the object
    virtual void render();
    // Loads a specific texture
    void LoadTexture(std::string fileName);
    // Gets the shaders for this object
    virtual std::vector<std::string> GetShaders();

protected:
    // Helper method for when we are ready to draw or update our object
    void Bind();
    // For now we have one buffer per object.
    Buffer myBuffer;
    // For now we have one diffuse map and one normal map per object
    Texture diffuseMap;
    // Store the objects Geometry
    Geometry geometry;
    // Reference to its ObjParser
    ObjParser* objParser;
};


#endif
