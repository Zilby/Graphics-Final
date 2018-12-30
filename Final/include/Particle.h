#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>
#include <string>
#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "SceneNode.h"

/**
 * Represents a particle (in a particle system).
 */
class Particle : public SceneNode 
{
public:
    // Construct a Particle with the given object, blending map, and shader. 
    Particle(Object* ob, Blending* b, Shader* s);

    // Update this Particle.
    virtual void Update() override;
    // Update the shader for this Particle.
    virtual void SetUpShader(glm::mat4 projectionMatrix, Camera* camera) override;
    // Renders this Particle.
    virtual void Render(glm::mat4 projectionMatrix, Camera* camera) override;
    
    // Get the life of this Particle.
    int GetLife();

    // Set up the given particle with the given life, fade in/out, proximity to kill, velocity, and color.
    void init(int l, bool fadeO, bool fadeI, Transform* prox, glm::vec3 vel, glm::vec4 c);

    // Pause/Unpause this Particle.
    void TogglePaused();

private:
    // The particle's current number of frames left to live.
    int life;
    // The initial value of this particle's life.
    int lifespan;
    // This particle's velocity.
    glm::vec3 velocity;
    // The color of this particle.
    glm::vec4 color;
    // If this particle is paused.
    bool paused;
    // Whether to apply a fade out to this particle.
    bool fadeOut;
    // Whether to apply a fade in to this particle.
    bool fadeIn;
    // The transform to check this particle's proximity to when checking for proximity death.
    Transform* proximity;
};

#endif