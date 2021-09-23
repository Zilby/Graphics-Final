# Lab 6 - Abstraction and Textures

<img align="right" src="http://www.mshah.io/comp/Spring18/graphics/Lecture/6/lecture.PNG" width="400px" alt="picture">

> "What's another layer of abstraction anyway?"

**Lab materials must be pushed to your repository one week from now before the next class begins**

**Read:** *Labs are designed to be finished in class if you work diligently, but expected to take 1-2 hours outside of class. They are often more 'tutorial' in style, and sometimes the solution may even be provided in pieces for you to type in and experiment. Copying & Pasting is discouraged however--make mistakes, play, and you will further learn the material.*

## Modify this section

- How many hours did it take you to complete this lab? ~1
- Did you collaborate with any other students/TAs/Professors? Lab partners.
- Did you use any external resources? (Cite them below) Just lecture notes.
- (Optional) What was your favorite part of the lab? Spinning cats.
- (Optional) How would you improve the lab? More spinning cats.

## Logistics

You will be working on your own laptop today.

## Description

Last lab we rendered a colored triangle and a square to the screen. As a recap:

- We created a colored traingle
- We created a perspective camera
- We learned about uniforms

Today we are going to really think about what is going on in our program, and abstract our render. This will help you understand object-oriented programming in C++ as well as think about what code is executing on the CPU and the GPU.

**The good news**: For part 1 I have already done the abstraction. You will have to learn how to use it however!

## Part 1 - Code Review

During your co-ops, internships, and jobs you will often do a code review. Today you will review my code for understanding the design of our abstraction, and to help connect the concepts that we have learned so far. The slides should help, but being able to dive into the code is a valuable skill to have. Once again start with the headers to see how the code works. 

Discuss with your partner the following:

1. What is the difference between the two buffer layouts? (Buffer.h)
2. What do you think the parameters in glTexImage2D mean in Texture::LoadTexture. (Texture.cpp)
3. Browse Transform.h to see how you can move models.

## Part 2 - Textures

Note: You may use the slides for guidance on completing this task. The deliverables section describes your task with some hints.

### Vertex Shader in a glance

It is worth taking another moment to review a few parts of the vertex shader.

* The **layout** corresponds to our [glEnableVertexAttribArray](http://docs.gl/gl3/glEnableVertexAttribArray) function.
  * Currently we are taking in position data (x,y,z). You'll need to figure out how to take in texture data, and what the corresponding data type should be.
* The **out** qualifier means we are sending data to the next part of the pipeline. From the vertex shader this means we will send data to the fragment shader.
* **uniforms** are values that are passed from CPU to GPU. They are variables that can talk in a way back in forth to modify what is going on.


```c
// ==================================================================
#version 330 core

layout(location=0)in vec3 position; // We explicitly state which is the vertex information (The first 3 floats are positional data, we are putting in our vector)
// TODO:
// Take in the texture coordinates


// If we have texture coordinates we can now use this as well
out vec2 v_texCoord;

// If we are applying our camera, then we need to add some uniforms.
// Note that the syntax nicely matches glm's mat4!
uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

void main()
{
  // gl_Position is a special glsl variable that tells us what
  // position to put things in.
  // It takes in exactly 4 things.
  // Note that 'w' (the 4th dimension) should be 1.
  vec4 oldPosition = vec4(position.x, position.y, position.z, 1.0f);
  vec4 newPositon = modelTransformMatrix * oldPosition;
  vec4 projectedPosition = projectionMatrix * newPositon;

  gl_Position = projectedPosition;

  // Store the texture coordinaets which we will output to
  // the next stage in the graphics pipeline.
  v_texCoord = texCoord;
}
// ==================================================================

```

### Fragment Shader in a glance

It is worth taking another moment to review a few parts of the fragment shader.

* The **in** qualifier means we are taking in information from a previous stage of the pipeline (in this case, our vertex shader).
  * The variable should match in datatype and name.
  * Think of these variables scope as 'existing on the GPU' if that is helpful, and that is why the names should match.
* Lookup what a **sampler2D** is.
  * You can see how it is used in 'Object::update' as a hint for how to create one. Another hint is that a uniform **should** match the name in the shader exactly as it is created.

```c
// ==================================================================
#version 330 core
out vec4 color;

in vec2 v_texCoord; // Take in our previous texture coordinates.


// If we have texture coordinates, they are stored in this sampler.
// TODO: Add in the sampler2D

void main()
{
    vec4 texColor = texture(u_Texture, v_texCoord);
    // Our final color is now based on the texture.
    color = texColor;
}
// ==================================================================

```

## Helpful Resources

- [SDL API Wiki](https://wiki.libsdl.org/CategoryAPI)
- [docs.gl](http://docs.gl/) useful programming resource
- Start writing some OpenGL 3.3 from here! [learnopengl.com](https://learnopengl.com/) Use this time for graphics!
- [Mip mapping](http://www.tomshardware.co.uk/ati,review-965-2.html)
- [Full video explaining Normal mapping](https://www.youtube.com/watch?v=6_-NNKc4lrk) (A special kind of texturing)

## Deliverables

1. Fill in the texture coordinates for Object::init.
2. Fill in vert.glsl to take in the texture coordinates (Should be a 1-liner)
3. Add in the sampler2D for frag.glsl (Another 1-line code change)
4. Draw two spinning quads that are textured.
      - You will have to add in another object and modifying the transform somewhere in the code.

## Rubric

You (and any partner(s)) will receive the same grade from a scale of 0-2.

- 0 for no work completed by the deadline
- 1 for some work completed, but something is not working properly
- 2 for a completed lab (with possible 'going further' options completed)

## Going Further

What is that, you finished Early? Did you enjoy this lab? Here are some (optional) ways to further this assignment.

- Try changing vertex colors and see if it effects the texture.
- Try changing the color of the texture in the fragment shader.

## Found a bug?

If you found a mistake (big or small, including spelling mistakes) in this lab, kindly send me an e-mail. It is not seen as nitpicky, but appreciated! (Or rather, future generations of students will appreciate it!)

- Fun fact: The famous computer scientist Donald Knuth would pay folks one $2.56 for errors in his published works. [[source](https://en.wikipedia.org/wiki/Knuth_reward_check)]
- Unfortunately, there is no monetary reward in this course :)
