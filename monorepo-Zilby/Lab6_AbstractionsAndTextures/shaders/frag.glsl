// ==================================================================
#version 330 core
out vec4 color;

in vec2 v_texCoord; // Take in our previous texture coordinates.


// If we have texture coordinates, they are stored in this sampler.
// TODO: Add in the sampler2D
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_texCoord);
    // Our final color is now based on the texture.
    color = texColor;
}
// ==================================================================
