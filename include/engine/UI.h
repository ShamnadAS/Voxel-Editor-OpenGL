#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <engine/engine.h>

class UI
{
public:
    Engine &VoxelEngine;

    UI(Engine &voxelEngine);
    ~UI();

    void OnCreate(const char* openglVersion, GLFWwindow* window);
    void OnDestroy();
    void OnBegin();
    void Render();
    void ToolBar();
    void ColorPalette();
    void ColorSelector();
    ImVec2 ViewPort(unsigned int imageId, unsigned width, unsigned height);
};