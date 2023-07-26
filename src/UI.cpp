#include <engine/UI.h>
#include <utility/resource_manager.h>
#include <iostream>
#include <string>

using namespace std;

unsigned int pencilID;
unsigned int eraserID;
unsigned int paintID;
unsigned int gridID;
unsigned int lightID;
unsigned int rectID;

ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
bool loadPalette = true;
vector<ImVec4> imPalette;

void UI::OnCreate(const char* openglVersion, GLFWwindow* window)
{
    //Load Icons 
    ResourceManager::LoadTexture("icons/pencil.png", true, "pencil");
    ResourceManager::LoadTexture("icons/eraser.png", true, "eraser");
    ResourceManager::LoadTexture("icons/paint.png", true, "paint");
    ResourceManager::LoadTexture("icons/grid.png", true, "grid");
    ResourceManager::LoadTexture("icons/light-bulb.png", true, "light");
    ResourceManager::LoadTexture("icons/rectangle.png", true, "rect");

    pencilID = ResourceManager::GetTexture("pencil").ID;
    eraserID = ResourceManager::GetTexture("eraser").ID;
    paintID = ResourceManager::GetTexture("paint").ID;
    gridID = ResourceManager::GetTexture("grid").ID;
    lightID = ResourceManager::GetTexture("light").ID;
    rectID  = ResourceManager::GetTexture("rect").ID;

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


    if(ImGui::ImageButton((ImTextureID)pencilID, {32, 32}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0), VoxelEngine.ActiveTool == 0 ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    { 
        VoxelEngine.ActiveTool = 0;
    }
    if(ImGui::ImageButton((ImTextureID)rectID, {32, 32}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0), VoxelEngine.ActiveTool == 3 ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    {
        VoxelEngine.ActiveTool = 3;
    }
    ImGui::IsItemActive();
    if(ImGui::ImageButton((ImTextureID)eraserID, {32, 32}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0), VoxelEngine.ActiveTool == 1 ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    {
        VoxelEngine.ActiveTool = 1;
    }
    if(ImGui::ImageButton((ImTextureID)paintID, {32, 32}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0), VoxelEngine.ActiveTool == 2 ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    {
        VoxelEngine.ActiveTool = 2;
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
            VoxelEngine.ActiveColor = Vector3(imPalette[i].x, imPalette[i].y, imPalette[i].z);
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
        VoxelEngine.ActiveColor = Vector3(color.x, color.y, color.z);
    }
    ImGui::End();
}

void UI::ViewPort(unsigned int imageId)
{
    ImGui::Begin("Viewport");
   
    ImVec2 imPos = ImGui::GetWindowPos();
    ImGuiStyle& style = ImGui::GetStyle();
    float titleBarHeight = (style.FramePadding.y * 2) + ImGui::GetFontSize();
    this->ViewPortPos = Vector2(imPos.x + style.WindowPadding.x, imPos.y + style.WindowPadding.y + titleBarHeight);

    ImVec2 imSize = ImGui::GetContentRegionAvail();
    ViewPortSize = Vector2(imSize.x, imSize.y);
    VoxelEngine.ViewportSize = ViewPortSize;
    
    VoxelEngine.IsMouseInViewPort = VoxelEngine.MousePosX >= 0 && VoxelEngine.MousePosY >= 0
                                    && VoxelEngine.MousePosX <= ViewPortSize.x && VoxelEngine.MousePosY <= ViewPortSize.y;

    ImGui::Image((ImTextureID)imageId, imSize, {0, 1}, {1, 0});
    
    ImGui::End();
}

void UI::MenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) { /* Handle 'New' menu item click */ }
            if (ImGui::MenuItem("Open")) { /* Handle 'Open' menu item click */ }
            if (ImGui::MenuItem("Save")) { /* Handle 'Save' menu item click */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) { /* Handle 'New' menu item click */ }
            if (ImGui::MenuItem("Redo")) { /* Handle 'Open' menu item click */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UI::Debug(GLFWwindow *window)
{
    ImGui::Begin("Debug");
    ImGui::Text("MousePos: (%.f, %.f)", VoxelEngine.MousePosX, VoxelEngine.MousePosY);
    ImGui::Text("Viewport pos: (%.f , %.f)", this->ViewPortPos.x, this->ViewPortPos.y);
    int xPos, yPos;
    glfwGetWindowPos(window, &xPos, &yPos);
    ImGui::Text("GLFWWindow pos: (%.f , %.f)", xPos, yPos);
    ImGui::Text("Viewport Siz: %.f, %.f", ViewPortSize.x, ViewPortSize.y);
    ImGui::End();
}

int value = 1;

void UI::ControlBar()
{
    ImGui::Begin("Control bar");

    if(ImGui::ImageButton((ImTextureID)gridID, {16, 16}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0),  value == 1 ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    {
        unsigned int shaderID = VoxelEngine.ActiveShader.ID;
        value = abs(value - 1);
        VoxelEngine.ActiveShader.Use().SetInteger("borderOn", value);
    }

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

    if(ImGui::ImageButton((ImTextureID)lightID, {16, 16}, {0, 0}, {1, 1}, -1, ImVec4(0, 0, 0, 0), VoxelEngine.ActiveShader.ID == 
    ResourceManager::GetShader("cubeShaderLit").ID ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.3f)))
    {
        VoxelEngine.ActiveShader = VoxelEngine.ActiveShader.ID == ResourceManager::GetShader("cubeShaderLit").ID ?
        ResourceManager::GetShader("cubeShader") : ResourceManager::GetShader("cubeShaderLit"); 
        VoxelEngine.ActiveShader.Use().SetInteger("borderOn", value);
    }

    ImGui::End();
}

UI::UI(Engine &voxelEngine)
:VoxelEngine(voxelEngine),ViewPortPos(Vector2(0,0)), ViewPortSize(Vector2(0, 0))
{
}
