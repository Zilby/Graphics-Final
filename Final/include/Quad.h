/** @file Quad.h
 *  @brief Draw a simple quad primitive.
 *
 *  Draws a simple quad primitive, that is derived
 *  from the Object class.
 *
 *  @bug No known bugs.
 */
#include "Object.h"

class Quad : public Object{
public:
    // Constructor for the Quad
    Quad();
    // The intialization routine for this object.
    void init();
};
