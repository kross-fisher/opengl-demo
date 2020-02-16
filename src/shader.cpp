#include "shader.h"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    programID = 0;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_READ_FAILED" << std::endl;
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    int success;
    char infoLog[512];
    unsigned int vShaderID, fShaderID;

    vShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderID, 1, &vShaderCode, NULL);
    glCompileShader(vShaderID);

    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl
            << infoLog << std::endl;
        return;
    }

    fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderID, 1, &fShaderCode, NULL);
    glCompileShader(fShaderID);

    glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(fShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl
            << infoLog << std::endl;
        return;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vShaderID);
    glAttachShader(programID, fShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (! success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED" << std::endl
            << infoLog << std::endl;
        return;
    }

    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);
}

void Shader::use() {
    glUseProgram(programID);
}

void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(programID, name), (int)value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name), value);
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name), value);
}

void Shader::setMat4(const char* name, glm::mat4 value) const {
    unsigned int mat4Loc = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const char* name, glm::vec3 value) const {
    unsigned int location = glGetUniformLocation(programID, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

/* for test ...
int main() {
    Shader shader("src/vertex_shader.vs", "src/fragment_shader.fs");
    return 0;
}
 */
