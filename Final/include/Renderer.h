#ifndef RENDERER_H
#define RENDERER_H
/** @file Renderer.h
 *  @brief Renderer is responsible for drawing.
 *
 * 	Renderer is responsible for drawing everything. It
 *	contains a scenegraph node and a camera. We could 
 *	possibly have multiple renderers (if we had multiple
 *	windows for example).
 *
 *	Each renderer thus has it's own camera.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

// The glad library helps setup OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <glad/glad.h>
#include <vector>

#include "ParticleGenerator.h"
#include "Camera.h"
#include "Blending.h"

class Renderer{
public:
	// The constructor	
	// Sets the width and height of the rendererer draws to
	Renderer(unsigned int w, unsigned int h);
	// Destructor
	~Renderer();
	// Update the scene
	void Update();
	// Render the scene
	void Render();
	// Sets the root of our renderer to some node to
	// draw an entire scene graph
	void setRoot(SceneNode* n);
	// The one camera per Renderer
	Camera* camera;
	// The blending object for transparency in this renderer
	Blending* blender;
	// Whether or not we're rendering in wireframe mode. 
	bool wireFrame;

protected:
	// Root scene node
	SceneNode* root;
    // Store the projection matrix for our camera.
    glm::mat4 projectionMatrix;

private:
	// Screen dimension constants
    int screenHeight;
    int screenWidth;
};

#endif
