#version 330 core

in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPos;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normVec = normalize(normalVec);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(0.0, dot(normVec, lightDir));
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normVec);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 objectColor = vec3(texture(ourTexture, texCoord));
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}
