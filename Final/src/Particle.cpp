#include "Particle.h"

Particle::Particle(Object* ob, Blending* b, Shader* s) : SceneNode(ob, b, s) { 
    paused = false;
}

// Draw simply draws the current nodes
// object and all of its children. This is done by calling directly
// the objects draw method.
void Particle::Update(){
    SceneNode::Update();
    if (!paused) {
        // Advance this particle's life.
        life--;
        // If this particle is alive
        if (life > 0.0f) {
            // Move the particle by its velocity 
            localTransform.translate(velocity.x, velocity.y, velocity.z);
            
            float lifetime = ((float)life) / ((float)lifespan);
            if (proximity != nullptr) 
            {
                // Limit life by distance
                float distance = glm::length(proximity->getPosition() - worldTransform.getPosition());
                if (distance < 1.f) {
                    // For particle in first quarter of life, jump to last quarter of life
                    if (lifetime > 0.75f) { 
                        life = lifespan - life;
                    }
                    // For particle in middle of life, reduce to last quarter of life.
                    else if (lifetime > 0.25f) {
                        life = lifespan / 4.f;
                    } 
                    else { // reduce life
                        life -= 5.f;
                    }
                }
            }

            // Adjust the particle's alpha color by life.
            if (fadeOut && fadeIn) 
            {
                if (lifetime < 0.25f) 
                {
                    color.a = lifetime * 4.f;
                } 
                else if (lifetime > 0.75f) 
                {
                    color.a = (1.f - lifetime) * 4.f;
                } 
            }
            else if (fadeOut) 
            {
                color.a = ((float)life) / ((float)lifespan);
            }
            else if (fadeIn)
            {
                color.a = 1.f - ((float)life) / ((float)lifespan);
            }
        }
    }
}

void Particle::SetUpShader(glm::mat4 projectionMatrix, Camera* camera){
    SceneNode::SetUpShader(projectionMatrix, camera);
    myShader->setUniform4f("color",color.r,color.g,color.b,color.a);
    myShader->setUniform1i("billboard", 1);
}

void Particle::Render(glm::mat4 projectionMatrix, Camera* camera) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    SceneNode::Render(projectionMatrix, camera);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int Particle::GetLife(){
    return life;
}

void Particle::init(int l, bool fadeO, bool fadeI, Transform* prox, glm::vec3 vel, glm::vec4 c) {
    life = l;
    lifespan = l;

    fadeOut = fadeO;
    fadeIn = fadeI;
    proximity = prox;
    velocity = vel;
    color = c;
}

void Particle::TogglePaused() {
    paused = !paused;
}
