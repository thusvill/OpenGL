// FBO.cpp
#include "FBO.h"


FBO::FBO(int width, int height) : width(width), height(height) {
    // Create framebuffer object
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // Create texture attachment for color
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // Check for framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw std::runtime_error("Framebuffer is not complete.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO() {
    glDeleteFramebuffers(1, &framebufferID);
    glDeleteTextures(1, &textureID);
}

void FBO::Resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;

    // Delete the previous framebuffer and texture
    glDeleteFramebuffers(1, &framebufferID);
    glDeleteTextures(1, &textureID);

    // Create a new framebuffer and texture with the updated size
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // Check for framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw std::runtime_error("Framebuffer is not complete.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FBO::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, width, height);
}

void FBO::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Reset the viewport to the default window size
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
}

GLuint FBO::GetTexture() const {
    return textureID;
}
