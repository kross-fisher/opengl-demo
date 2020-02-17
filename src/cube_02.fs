#version 330 core

in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPos;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normVec = normalize(normalVec);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(0.0, dot(normVec, lightDir));
    vec3 diffuse = diff * lightColor;

    vec3 objectColor = vec3(texture(ourTexture, texCoord));
    FragColor = vec4((ambient + diffuse) * objectColor, 1.0);
}
