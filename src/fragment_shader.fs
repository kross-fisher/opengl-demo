#version 330 core

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

out vec4 FragColor;

void main() {
    //FragColor = vec4(ourColor, 1.0);
    //FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(ourTexture1, texCoord),
                    texture(ourTexture2, texCoord), 0.3);
}
