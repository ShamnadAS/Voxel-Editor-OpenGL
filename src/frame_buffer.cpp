#include <utility/frame_buffer.h>
#include <iostream>
#include <utility/resource_manager.h>

FrameBuffer::FrameBuffer()
:samples(8)
{
}

void FrameBuffer::Init(unsigned width, unsigned height)
{
    //Multi sampling FBO
    glGenFramebuffers(1, &this->MsFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->MsFboId);
    // generate texture
    glGenTextures(1, &this->MsTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->MsTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->MsTextureId, 0);
    glGenRenderbuffers(1, &this->MsRboId);
    glBindRenderbuffer(GL_RENDERBUFFER, this->MsRboId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->MsRboId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Intermediate FBO
    glGenFramebuffers(1, &this->InterFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->InterFboId);
    // generate texture
    glGenTextures(1, &this->InterTextureId);
    glBindTexture(GL_TEXTURE_2D, this->InterTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,  GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->InterTextureId, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->MsFboId);
}

void FrameBuffer::UpdateDimensions(unsigned width, unsigned height)
{
    //Ms Fbo
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->MsTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, this->MsRboId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //Intermediate Fbo
    glBindTexture(GL_TEXTURE_2D, this->InterTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,  GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::ResolveTexture(unsigned width, unsigned height)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MsFboId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->InterFboId);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
