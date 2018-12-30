#include "Quad.h"

// Calls the initalization routine
Quad::Quad(){
    std::cout << "(Quad.cpp) Quad constructor called (derived from Object)\n";
    init();
}

// Algorithm for rendering a Quad
void Quad::init(){
    GLfloat quad[] = {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        1.f, 1.f, 0.f,
        -1.f, 1.f, 0.f,
    }; 

    GLfloat texture_coords[] = {
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
    }; 

    GLuint indices[] = { 0, 1, 2, 
                         2, 3, 0 };

    for (int i = 0; i < 6; ++i) {
        geometry.addVertex(quad[indices[i] * 3], quad[(indices[i] * 3) + 1], quad[(indices[i] * 3) + 2]);
        geometry.addTexture(texture_coords[indices[i] * 2], texture_coords[(indices[i] * 2) + 1]);
    }

    for (int i = 0; i < 2; ++i) {
        geometry.makeTriangle(i * 3, (i * 3) + 1, (i * 3) + 2);
    }

    geometry.gen();

    // Create a buffer and set the stride of information
    myBuffer.CreateBufferNormalMapLayout(14,
                                        geometry.getSize(),
                                        geometry.getIndicesSize(),
                                        geometry.getData(),
                                        geometry.getIndicesData());
}