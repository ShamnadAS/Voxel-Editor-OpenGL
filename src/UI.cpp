#include <engine/UI.h>
#include <utility/resource_manager.h>
#include <iostream>
#include <string>
using namespace std;

unsigned int pencilID;
unsigned int eraserID;
unsigned int paintID;

ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
bool loadPalette = true;
vector<ImVec4> imPalette;

void UI::OnCreate(const char* openglVersion, GLFWwindow* window)
{
    //Load Icons 
    ResourceManager::LoadTexture("icons/pencil.png", true, "pencil");
    ResourceManager::LoadTexture("icons/eraser.png", true, "eraser");
    ResourceManager::LoadTexture("icons/paint.png", true, "paint");

    pencilID = ResourceManager::GetTexture("pencil").ID;
    eraserID = ResourceManager::GetTexture("eraser").ID;
    paintID = ResourceManager::GetTexture("paint").ID;

    //Load Palettes
    vector<Vector3> palette = ResourceManager::LoadColorPalette("palette/palette 2.png", "palette1");
    for(auto &color : palette)
    {
        ImVec4 imColor = ImVec4(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, 1.0f);
        imPalette.push_back(imColor);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(openglVersion);
}

void UI::OnDestroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::OnBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::Render()
{
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();(void)io;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void UI::ToolBar()
{
    ImGui::Begin("Toolbar");

    if(ImGui::ImageButton((ImTextureID)pencilID, {32, 32}))
    { 
        VoxelEngine.activeTool = 0;
    }
    ImGui::IsItemActive();
    if(ImGui::ImageButton((ImTextureID)eraserID, {32, 32}))
    {
        VoxelEngine.activeTool = 1;
    }
    if(ImGui::ImageButton((ImTextureID)paintID, {32, 32}))
    {
        VoxelEngine.activeTool = 2;
    }

    ImGui::End();
}

void UI::ColorPalette()
{
    ImGui::Begin("Color Palette");
    
    for (int i = 0; i < imPalette.size(); i++)
    {
        ImGui::PushID(i);

        if ((i % 8) != 0)
        {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
        }
        if(ImGui::ColorButton("color", imPalette[i]))
        {
            color = imPalette[i];
            VoxelEngine.activeColor = Vector3(imPalette[i].x, imPalette[i].y, imPalette[i].z);
        }

        ImGui::PopID();
    }

    ImGui::End();
}

void UI::ColorSelector()
{
    ImGui::Begin("Color Selector");
    if(ImGui::ColorPicker3("Current", (float*)&color))
    {
        VoxelEngine.activeColor = Vector3(color.x, color.y, color.z);
    }
    ImGui::End();
}

ImVec2 UI::ViewPort(unsigned int imageId)
{
    ImGui::Begin("Viewport");
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((ImTextureID)imageId, windowSize, {0, 1}, {1, 0});
    ImGui::End();

    return windowSize;
}

UI::UI(Engine &voxelEngine)
:VoxelEngine(voxelEngine)
{
}
