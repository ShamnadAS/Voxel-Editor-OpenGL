#pragma once

class FrameBuffer
{
public:
    unsigned int ID;
    unsigned int textureID;
    FrameBuffer();
    ~FrameBuffer();

    void Init(unsigned width, unsigned height);
    void Bind();
};