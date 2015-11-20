#include "depthmap_point.h"
#include "common/globals.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace moar
{

DepthMapPoint::DepthMapPoint() :
    projectionMatrix(glm::perspective(glm::radians(90.0f), 1.0f, nearClipDistance, farClipDistance)) // Todo: calculated ratio from screen width & height
{
}

DepthMapPoint::~DepthMapPoint()
{
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &depthCubeTexture);
}

bool DepthMapPoint::init(GLuint shader)
{
    if (width == 0 || height == 0) {
        std::cerr << "ERROR: Shadow map width or height not initilized." << std::endl;
    }

    this->shader = shader;

    glGenTextures(1, &depthCubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeTexture);
    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    bool status = createFramebuffer(framebuffer, depthCubeTexture, true);
    return status;
}

void DepthMapPoint::bind(const glm::vec3& lightPos, const glm::vec3& /*lightDir*/)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);

    lightSpaces[0] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    lightSpaces[1] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    lightSpaces[2] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightSpaces[3] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    lightSpaces[4] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    lightSpaces[5] = projectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(LIGHT_SPACE_VP_LOCATION, 6, GL_FALSE, glm::value_ptr(lightSpaces[0]));
    glUniform1f(FAR_CLIP_DISTANCE_LOCATION, farClipDistance);
}

void DepthMapPoint::activate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeTexture);
    glUniform1i(DEPTH_TEX_LOCATION, 0);
}

} // moar