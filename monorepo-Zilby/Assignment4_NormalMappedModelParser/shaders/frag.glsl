// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates.
in vec3 FragPos;
in vec2 v_texCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform sampler2D u_NormalMap; 

void main()
{

	// Store the texture coordinates
	vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);   

	 // Store our final texture color
    vec3 diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // (1) Compute ambient light
    vec3 ambient = 0.1 * diffuseColor;

    // (2) Compute diffuse light

    // From our lights position and the fragment, we can get
    // a vector indicating direction
    // Note it is always good to 'normalize' values.
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    // Now we can compute the diffuse light impact
    float diffImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuseLight = diffImpact * diffuseColor;

    // (3) Compute Specular lighting
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(0.5) * spec * diffuseColor;

    // Our final color is now based on the texture.
    // That is set by the diffuseColor
    vec3 Lighting = diffuseLight + ambient + specular;

    // Final color + "how dark or light to make fragment"
    if(gl_FrontFacing){
        FragColor = vec4(diffuseColor * Lighting,1.0);
    }else{
        // Additionally color the back side the same color
        FragColor = vec4(diffuseColor * Lighting,1.0);
    }

    /*
    vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
    vec3 color = texture(u_DiffuseMap, v_texCoord).rgb;

    if(v_texCoord.y > 0.5) {
        FragColor = vec4(normal, 1.0);
    } else {
        FragColor = vec4(color, 1.0);
    }
    */

    // FragColor = vec4(texture(u_NormalMap, v_texCoord).rgb, 1.0);

}
// ==================================================================
