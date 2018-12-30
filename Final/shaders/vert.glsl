// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

// If we are applying our camera, then we need to add some uniforms.
// Note that the syntax nicely matches glm's mat4!
uniform mat4 model; // Object space
uniform mat4 view; // Object space
uniform mat4 projection; // Object space

uniform int billboard;

// Export our normal data, and read it into our frag shader
out vec3 myNormal;
// Export our Fragment Position computed in world space
out vec3 FragPos;
// If we have texture coordinates we can now use this as well
out vec2 v_texCoord;


void main()
{
	mat4 mvp = projection * view * model;
  if (billboard == 1)
  {
      vec3 scale = vec3(length(mvp[0]), length(mvp[1]), length(mvp[2]));
      gl_Position = mvp * vec4(vec3(0.0), 1.0) + vec4(scale * position, 0.0);
  } 
  else 
  {
    gl_Position = mvp * vec4(position, 1.0f);
  }

  myNormal = normals;
  // Transform normal into world space
  FragPos = vec3(model* vec4(position,1.0f));

  // Store the texture coordinaets which we will output to
  // the next stage in the graphics pipeline.
  v_texCoord = texCoord;
}
// ==================================================================
