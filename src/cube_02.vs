#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;
out vec3 normalVec;
out vec3 fragPos;

void main() {
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = proj * view * model * vec4(aPos, 1.0);

    texCoord = aTexCoord;
    texCoord.x = (texCoord.x - 0.5) * 0.46 + 0.5;

    fragPos = vec3(model * vec4(aPos, 1.0));

    // calculating current fragment position's normal vector
    normalVec.x = (aPos.x > 0.4999 || aPos.x < -0.4999) ? aPos.x : 0;
    normalVec.y = (aPos.y > 0.4999 || aPos.y < -0.4999) ? aPos.y : 0;
    normalVec.z = (aPos.z > 0.4999 || aPos.z < -0.4999) ? aPos.z : 0;
    normalVec = vec3(model * vec4(normalVec, 0.0));
}
