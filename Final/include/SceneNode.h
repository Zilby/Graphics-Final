#ifndef SCENENODE_H
#define SCENENODE_H
/** @file SceneNode.h
 *  @brief SceneNode helps organize a large 3D graphics scene.
 *  
 *  SceneNode helps organize a large 3D graphics scene.
 *  The traversal of the tree takes place starting from
 *  a single SceneNode (typically called root).
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#include <vector>

#include "Object.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"
#include "Blending.h"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Blending;

class SceneNode{
public:
    // A SceneNode is created by taking
    // a pointer to an object.
    SceneNode(Object* ob, Blending* b, Shader* s = nullptr);
    // Our destructor takes care of destroying
    // all of the children within the node.
    // Now we do not have to manage deleting
    // each individual object.
    ~SceneNode();
    // Adds a child node to our current node.
    void AddChild(SceneNode* n);
    // Draws the current SceneNode
    virtual void Draw();
    // Set up the given shader (passes required values to shader)
    virtual void SetUpShader(glm::mat4 projectionMatrix, Camera* camera);
    // Renders the current SceneNode
    virtual void Render(glm::mat4 projectionMatrix, Camera* camera);
    // Updates the current SceneNode
    virtual void Update();
    // Returns the local transformation transform
    // Remember that local is local to an object, where it's center is the origin.
    Transform& getLocalTransform();
    // Returns a SceneNode's world transform
    Transform& getWorldTransform();
    // For now we have one shader per object.
    Shader* myShader;
protected:
    // Parent of this SceneNode.
    SceneNode* parent;
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    std::vector<SceneNode*> children;
    // The object stored in the scene graph
    Object* object;
    // Used for blending transparent objects
    Blending* blender;
    // Each SceneNode nodes locals transform.
    Transform localTransform;
    // We additionally can store the world transform
    Transform worldTransform;
};

#endif
