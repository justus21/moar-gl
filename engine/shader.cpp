#include "shader.h"

#include <cstdio>

namespace moar {

Shader::Shader() {
    program = glCreateProgram();
}

Shader::~Shader() {
    glDeleteProgram(program);
    for (GLuint shader : shaders) {
        glDeleteShader(shader);
    }
}

bool Shader::attachShader(GLenum shaderType, const char *filename) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader || !compileShader(shader, filename)) {
        glDeleteShader(shader);
        return false;
    }

    shaders.push_back(shader);
    glAttachShader(program, shader);
    return true;
}

bool Shader::compileShader(GLuint shader, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    size_t filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    GLchar* data = new char[filesize + 1];
    if (!data) {
        fclose(fp);
        return false;
    }
    fread(data, 1, filesize, fp);
    data[filesize] = '\0';
    fclose(fp);

    glShaderSource(shader, 1, const_cast<const GLchar**>(&data), NULL);
    delete[] data;

    glCompileShader(shader);
    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[4096];
        glGetShaderInfoLog(shader, 4096, NULL, buffer);
        fprintf(stderr, "%s: %s\n", filename, buffer);
        return false;
    }

    return true;
}

} // moar