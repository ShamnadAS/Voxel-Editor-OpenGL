#include <engine/UI.h>
#include <utility/resource_manager.h>

unsigned int pencilID;
unsigned int eraserID;
unsigned int paintID;

// bool loadPalette = true;
// Texture2D loadedPalette;

void UI::OnCreate(const char* openglVersion, GLFWwindow* window)
{
    //Load Icons 
    ResourceManager::LoadTexture("icons/pencil.png", true, "pencil");
    ResourceManager::LoadTexture("icons/eraser.png", true, "eraser");
    ResourceManager::LoadTexture("icons/paint.png", true, "paint");
    //Load Palettes
    //ResourceManager::LoadTexture("palette/palette1.png", true, "palette1");
    //loadedPalette = ResourceManager::GetTexture("palette1");

    pencilID = ResourceManager::GetTexture("pencil").ID;
    eraserID = ResourceManager::GetTexture("eraser").ID;
    paintID = ResourceManager::GetTexture("paint").ID;

    //Load shader


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
        VoxelEngine.ToolState = 0;
    }
    ImGui::IsItemActive();
    if(ImGui::ImageButton((ImTextureID)eraserID, {32, 32}))
    {
        VoxelEngine.ToolState = 1;
    }
    if(ImGui::ImageButton((ImTextureID)paintID, {32, 32}))
    {
        VoxelEngine.ToolState = 2;
    }

    ImGui::End();
}

// void UI::ColorPalette()
// {
//     if(loadPalette)
//     {
//         //load the palette
//     }

//     ImGui::Begin("Color Palette");
//     ImGui::Text("Color palette");
//     ImGui::End();
// }

UI::UI(Engine &voxelEngine)
:VoxelEngine(voxelEngine)
{
}
