#version 330 core

in vec2 texCoord;
in vec2 lightMapCoord;
in mat4 fmodel;
in vec3 fPos;

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
    vec3 fragPos = vec3(fmodel * vec4(fPos, 1.0));

    // calculating current fragment position's normal vector
    vec3 normalVec = vec3(0.0);
    normalVec.x = (abs(fPos.x) > 0.4999) ? fPos.x : 0;
    normalVec.y = (abs(fPos.y) > 0.4999) ? fPos.y : 0;
    normalVec.z = (abs(fPos.z) > 0.4999) ? fPos.z : 0;
    normalVec = vec3(fmodel * vec4(normalVec, 0.0));

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
