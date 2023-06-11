#pragma once

class FrameBuffer
{
public:
    unsigned int ID;
    unsigned int textureID;
    unsigned int rboID;
    FrameBuffer();
    ~FrameBuffer();

    void Init(unsigned width, unsigned height);
    void Bind();
    void UpdateDimensions(unsigned width, unsigned height);
};