#version 330 core

in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
    FragColor = texture(ourTexture, texCoord) * vec4(lightColor, 1.0);
}
