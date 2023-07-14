#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <engine/engine.h>

#include <GLFW/glfw3.h>

class UI
{
public:
    Engine &VoxelEngine;
    Vector2 ViewPortPos;
    Vector2 ViewPortSize;

    UI(Engine &voxelEngine);
    ~UI();

    void OnCreate(const char* openglVersion, GLFWwindow* window);
    void OnDestroy();
    void OnBegin();
    void Render();
    void ToolBar();
    void ControlBar();
    void ColorPalette();
    void ColorSelector();
    void ViewPort(unsigned int imageId);
    void Debug(GLFWwindow* window);
};