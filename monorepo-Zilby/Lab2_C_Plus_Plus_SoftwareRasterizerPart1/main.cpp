/** @file main.cpp
 *  @brief Entry point into our program.
 *  
 *  Welcome to the Great Looking Software Render 
 *  code base (Yes, I needed something with a gl prefix).
 *
 *  This is where you will implement your graphics API.
 *
 *  Compile on the terminal with: 
 *
 *  clang++ -std=c++11 main.cpp -o main
 *
 *  @author Mike Shah
 *  @bug No known bugs.
 */

// Load our libraries
#include <iostream>

// Some define values
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 320

// C++ Standard Libraries
#include <iostream>

// User libraries
#include "GL.h"
#include "Color.h"
#include "TGA.h"
#include "Maths.h"

// Create a canvas to draw on.
TGA canvas(WINDOW_WIDTH,WINDOW_HEIGHT);


// Implementation of Bresenham's Line Algorithm
// The input to this algorithm is two points and a color
// This algorithm will then modify a canvas (i.e. image)
// filling in the appropriate colors.
void drawLine(Vec2 v0, Vec2 v1, TGA& image, ColorRGB c){
    bool steep = false;
    if(std::abs(v0.x-v1.x)<std::abs(v0.y-v1.y)){
        // If the line is steep we want to transpose the image.
        std::swap(v0.x,v0.y);
        std::swap(v1.x,v1.y);
        steep = true;
    }
    if(v0.x>v1.x){  // make it left-to-right
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }
    for(int x = v0.x; x <= v1.x; ++x){
        float t = (x-v0.x)/(float)(v1.x-v0.x);
        int y = v0.y*(1.0f-t) + v1.y*t;
        if(steep){
            canvas.setPixelColor(y,x,c);
        }else{
            canvas.setPixelColor(x,y,c);
        }
    }
}


// Takes cross product of two vectors
float crossProduct(Vec2 v1, Vec2 v2) {
    return v1.x * v2.y - v1.y * v2.x;
}


// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2,TGA& image, ColorRGB c){
    glFillMode = LINE;
    if(glFillMode==LINE){
        drawLine(v0,v1,image,c);
        drawLine(v1,v2,image,c);
        drawLine(v2,v0,image,c);
    }
    int maxX = std::max(v0.x, std::max(v1.x, v2.x));
    int minX = std::min(v0.x, std::min(v1.x, v2.x));
    int maxY = std::max(v0.y, std::max(v1.y, v2.y));
    int minY = std::min(v0.y, std::min(v1.y, v2.y));


    Vec2 vs1 = Vec2(v1.x - v0.x, v1.y - v0.y);
    Vec2 vs2 = Vec2(v2.x - v0.x, v2.y - v0.y);

    for (int x = minX; x <= maxX; x++)
    {
      for (int y = minY; y <= maxY; y++)
      {
        Vec2 q = Vec2(x - v0.x, y - v0.y);

        float s = crossProduct(q, vs2) / crossProduct(vs1, vs2);
        float t = crossProduct(vs1, q) / crossProduct(vs1, vs2);

        if ( (s >= 0) && (t >= 0) && (s + t <= 1))
        {
          canvas.setPixelColor(x,y,c);
        }
      }
    }
}


// Main
int main(){

    // A sample of color(s) to play with
    ColorRGB red;
    red.r = 255; red.g = 0; red.b = 0;

    ColorRGB blue;
    blue.r = 0; blue.g = 0; blue.b = 255;

    ColorRGB green;
    green.r = 0; green.g = 255; green.b = 0;
        
    
    // Points for our Line
    Vec2 line[2] = {Vec2(0,0), Vec2(100,100)};

    // Set the fill mode
    glPolygonMode(FILL);

    // Draw a line
    drawLine(line[0],line[1],canvas,red);

    // Data for our triangle
    Vec2 tri[3] = {Vec2(180,60),Vec2(150,10),Vec2(75,190)};
    Vec2 tri2[3] = {Vec2(210,20),Vec2(250,50),Vec2(205,90)};
    Vec2 tri3[3] = {Vec2(180,140),Vec2(220,130),Vec2(290,190)};

    // Draw a triangle
    triangle(tri[0],tri[1],tri[2],canvas,red);
    triangle(tri2[0],tri2[1],tri2[2],canvas,blue);
    triangle(tri3[0],tri3[1],tri3[2],canvas,green);

    // Output the final image
    canvas.outputTGAImage("graphics_lab2.ppm");

    return 0;
}
