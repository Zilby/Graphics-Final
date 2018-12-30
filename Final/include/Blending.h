#ifndef BLENDING_H
#define BLENDING_H

#include <map>
#include <glad/glad.h>

#include "SceneNode.h"
#include "Camera.h"

// Forward declaration.
class SceneNode;

/**
 * This class provides support for transparency in rendering textures.
 */
class Blending{ 
public:
    // Constructor for our Blending object, taking a pointer.
	Blending(Camera* c, glm::mat4 proj);

    // Destructor.
    ~Blending();

    // Add the given SceneNode to this Blending map.
    void AddNode(SceneNode* s);

    // Clear our Blending map.
    void ResetMap();

    // Draw all of the SceneNodes in this Blending map.
    void Draw();

private:
    // A map of distances (from the Camera) to SceneNodes.  
	std::map<float, SceneNode*> nodes;
    // A reference to our Camera.
	Camera* camera;
    // Our projectionMatrix.
	glm::mat4 projectionMatrix;
};

#endif