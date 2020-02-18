#version 330 core

in vec2 texCoord;
in vec2 lightMapCoord;
in vec3 normalVec;
in vec3 fragPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    //vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 FragColor;

void main() {
    vec3 lightMapTex = vec3(texture(material.diffuse, lightMapCoord));
    vec3 specuMapTex = vec3(texture(material.specular, lightMapCoord));

    vec3 ambient = lightMapTex * light.ambient;

    vec3 normVec = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(0.0, dot(normVec, lightDir));
    vec3 diffuse = diff * lightMapTex * light.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normVec);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
    vec3 specular = spec * specuMapTex * light.specular;

    vec3 objectColor = vec3(texture(ourTexture, texCoord));
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}
