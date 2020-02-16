#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"

class Shader {

public:
    unsigned int programID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const char* name, bool value) const;
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;
    void setMat4(const char* name, glm::mat4 value) const;
    void setVec3(const char* name, glm::vec3 value) const;
};

#endif
