#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Blending* b, std::vector<std::string> fileNames, int num, glm::vec3 spr,
                                     glm::vec3 vMin, glm::vec3 vMax, int lMin, int lMax, bool fout, bool fin, Transform* pd,
                                     glm::vec4 cMin, glm::vec4 cMax) : SceneNode(nullptr, b) {
    diffuseMaps = fileNames;
    numParticles = num;
    spread = spr;

    lifeMin = lMin;
    lifeMax = lMax;
    velocityMin = vMin;
    velocityMax = vMax;
    colorMin = cMin;
    colorMax = cMax;
    
    fadeOut = fout;
    fadeIn = fin;
    proximityDeath = pd;
    std::vector<Quad*> quads; 
    
    myShader = new Shader();
    std::string vertexShader = myShader->LoadShader("./shaders/vertP.glsl");
    std::string fragmentShader = myShader->LoadShader("./shaders/fragP.glsl");
    myShader->CreateShader(vertexShader,fragmentShader);     
    for (int i = 0; i < diffuseMaps.size(); ++i) 
    {
        Quad* q = new Quad();
        q->LoadTexture(diffuseMaps[i]);
        quads.push_back(q);
    }
    srand (time(NULL));
    for (int i = 0; i < numParticles; ++i) {
        unsigned int randQuad = rand() % (int)quads.size();
        Particle* p = new Particle(quads[randQuad], b, myShader);
        setUpParticle(p);
        AddChild(p);
    }
}

void ParticleGenerator::Update(){
    for (int i = 0; i < numParticles; i++) {
        Particle* p = (Particle*)children[i];
        if (p->GetLife() <= 0) {
            setUpParticle(p);
        }
    }
    SceneNode::Update();
}

float ParticleGenerator::randRange(float min, float max) 
{
    if (min == max) 
    {
        return min;
    }
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max - min)));
}

void ParticleGenerator::setUpParticle(Particle* particle)
{
    glm::vec3 vel = glm::vec3(randRange(velocityMin.x, velocityMax.x),
                              randRange(velocityMin.y, velocityMax.y),
                              randRange(velocityMin.z, velocityMax.z));
    glm::vec4 col = glm::vec4(randRange(colorMin.r, colorMax.r),
                              randRange(colorMin.g, colorMax.g),
                              randRange(colorMin.b, colorMax.b),
                              randRange(colorMin.a, colorMax.a));

    float life = randRange(lifeMin, lifeMax);
    particle->init(life, fadeOut, fadeIn, proximityDeath, vel, col);

    float randomx = ((rand() % 100) - 50) / 50.0f;
    float randomy = ((rand() % 100) - 50) / 50.0f;
    float randomz = ((rand() % 100) - 50) / 50.0f;
    particle->getLocalTransform().loadIdentity();
    particle->getLocalTransform().translate(randomx * spread.x, randomy * spread.y, randomz * spread.z);
}

void ParticleGenerator::TogglePaused() 
{
    for (int i = 0; i < numParticles; i++) {
        Particle* p = (Particle*)children[i];
        p->TogglePaused();
    }
}
