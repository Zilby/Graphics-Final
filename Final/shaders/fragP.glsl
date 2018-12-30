// ====================================================
#version 330 core

// ======================= uniform ====================
// Our light sources
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
// Used for our specular highlights
uniform mat4 view;
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform vec4 color;


// ======================= IN =========================
in vec3 myNormal; // Import our normal data
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;


void main()
{
    // Store our final texture color
    vec4 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgba * color;
    if(diffuseColor.a < 0.1)
        discard;

    // Final color + "how dark or light to make fragment"
    if(gl_FrontFacing){
        FragColor = vec4(diffuseColor);
    }else{
        // Additionally color the back side the same color
        FragColor = vec4(diffuseColor);
    }
}
// ==================================================================
