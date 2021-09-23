// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Our light source data structure
struct PointLight{
    vec3 lightColor;
    vec3 lightPos;
    float ambientIntensity;

    float specularStrength;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[4];


// Used for our specular highlights
uniform mat4 view;


// Import our normal data
in vec3 myNormal;
// Import our texture coordinates from vertex shader
in vec2 v_texCoord;
// Import the fragment position
in vec3 FragPos;


uniform vec3 viewPos;
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap;

vec3 calcPointLight(PointLight light); 

void main()
{
    // Store our final texture color
            vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // Our final color is now based on the texture.
    // That is set by the diffuseColor

    vec3 Lighting = calcPointLight(pointLights[0]);
    Lighting += calcPointLight(pointLights[1]);
    Lighting += calcPointLight(pointLights[2]);
    Lighting += calcPointLight(pointLights[3]);

    // Final color + "how dark or light to make fragment"
    if(gl_FrontFacing){
        FragColor = vec4(diffuseColor * Lighting,1.0);
    }else{
        // Additionally color the back side the same color
         FragColor = vec4(diffuseColor * Lighting,1.0);
    }
}

vec3 calcPointLight(PointLight light) {
    // Compute the normal direction
    vec3 normal = normalize(myNormal);

    // ambient light
    vec3 ambient = light.ambientIntensity * light.lightColor;

    // diffuse light
    vec3 lightDir = normalize(light.lightPos - FragPos);
    float diffImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffImpact * light.lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specularStrength * spec * light.lightColor;

    // attenuation
    float distance = length(light.lightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +  light.quadratic * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;


}


// ==================================================================