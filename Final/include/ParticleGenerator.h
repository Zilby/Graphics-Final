#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <time.h>
#include "Particle.h"
#include "Quad.h"

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator : public SceneNode 
{
public:
    // Construct a ParticleGenerator with the given blending map, diffuse maps, and range values (see fields below).
    ParticleGenerator(Blending* b, std::vector<std::string> fileNames, int num, glm::vec3 spr = glm::vec3(1.f),
                      glm::vec3 vMin = glm::vec3(0.f), glm::vec3 vMax = glm::vec3(0.f), 
                      int lMin = 10, int lMax = 30, bool fout = false, bool fin = false, Transform* pd = nullptr,
                      glm::vec4 cMin = glm::vec4(1.f), glm::vec4 cMax = glm::vec4(1.f));

    // Update all of the particles in this particle generator.
    virtual void Update() override;

    // Pause this particle system
    void TogglePaused();

private:
    // Total number of particles in this particle generator.
    int numParticles;
    // The spread for which to spawn particles in this system. 
    // For example, if a spread of (5, 2, 0) was given, particles would spawn 
    // randomly between x from -5 and 5, and y from -2 to 2 (relative to the particle system).
    glm::vec3 spread;
    // List of textures for the particles to randomly select.    
    std::vector<std::string> diffuseMaps;
    // Minium value for particle life.
    int lifeMin;
    // Maximum value of particle life.
    int lifeMax;
    // Minimum velocity for a particle 
    glm::vec3 velocityMin; 
    // Maximum velocity for a particle 
    glm::vec3 velocityMax;
    // Minium value for particle color.
    glm::vec4 colorMin;
    // Maximum value of particle color.
    glm::vec4 colorMax;
    // Whether or not the particles in this system should fade in after spawning.
    bool fadeIn;
    // Whether or not the particles in this system should fade out as they die.
    bool fadeOut;

    // The transform to compare particle position to in order to check for proximity based death (collisions).
    Transform* proximityDeath;

    // Initialize/Reset the given particle with new random values within the valid life, velocity, color ranges.
    void setUpParticle(Particle* particle);
    // Get a random value in the given range.
    float randRange(float min, float max);
};

#endif