#include "Blending.h"

Blending::Blending(Camera* c, glm::mat4 proj){
	camera = c;
	projectionMatrix = proj;
}

Blending::~Blending(){}

void Blending::AddNode(SceneNode* s){
	float distance = glm::length(camera->getEyePosition() - s->getWorldTransform().getPosition());
	std::map<float,SceneNode*>::iterator it = nodes.find(distance);
	while(it != nodes.end()) {
		distance += 0.001f;
		it = nodes.find(distance);
	}
	nodes[distance] = s;
}

void Blending::ResetMap(){
	nodes.clear();
}

void Blending::Draw(){
	for (std::map<float, SceneNode*>::reverse_iterator it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        it->second->Render(projectionMatrix, camera);
    }
}