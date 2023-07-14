#pragma once

class FrameBuffer
{
public:
    unsigned int InterFboId;
    unsigned int InterTextureId;

    unsigned int MsFboId;
    unsigned int MsTextureId;
    unsigned int MsRboId;

    unsigned int samples;

    FrameBuffer();
    ~FrameBuffer();

    void Init(unsigned width, unsigned height);
    void Bind();
    void UpdateDimensions(unsigned width, unsigned height);
    void ResolveTexture(unsigned width, unsigned height);
};