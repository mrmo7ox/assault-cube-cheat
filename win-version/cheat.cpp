#include "cheat.hpp"

Cheat::Cheat(): pid(0), entity_address(0), BaseAdd(0), window(nullptr) {
    std::cout << "Cheat initialize" << std::endl;
}

Cheat::~Cheat() {
    if (this->hProcess) {
        CloseHandle(this->hProcess);
    }
    if (this->window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }
}
void Cheat::run()
{
    ImGuiIO& io = ImGui::GetIO();
    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();

        // Handle mouse passthrough depending on ImGui state
        if (!io.WantCaptureMouse) {
            glfwSetWindowAttrib(this->window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        } else {
            glfwSetWindowAttrib(this->window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(this->width, this->height));
        
        ImGui::Begin("Overlay", nullptr, 
            ImGuiWindowFlags_NoBackground | 
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoSavedSettings | 
            ImGuiWindowFlags_NoInputs);

        // Your drawing logic would go here

        ImGui::End();

        ImGui::Render();
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->window);
    }
}

void Cheat::GetProccesname(const std::string & name)
{
    DWORD p_id = -1;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if(snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if(Process32First(snapshot, &pe32))
        {
            do
            {
                if(name == pe32.szExeFile)
                {
                    p_id = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pe32));
        }
        CloseHandle(snapshot); // Important to close handles to avoid memory leaks
    }
    
   this->pid = p_id;
    if(this->pid == 0)
        throw std::runtime_error("pid not found");
    this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
    if(this->hProcess == NULL)
        throw std::runtime_error("Failed to get Process Handle (Run as Administrator!)");
}

void Cheat::HealthHack(bool v)
{
    if(v)
    {
    }
}

void Cheat::GetModuleBase(DWORD _pid, const std::string& moduleName)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _pid);
    if(snap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 entry;
        entry.dwSize = sizeof(entry);
        if(Module32First(snap, &entry))
        {
            do {
                if (moduleName == entry.szModule)
                {
                    this->setBaseAdd((uint32_t)(uintptr_t)entry.modBaseAddr);
                    break;
                }
            }
            while(Module32Next(snap, &entry)); 
        }
        CloseHandle(snap);
    }
    
    if(this->BaseAdd == 0)
        throw std::runtime_error("cant find the base address");
}

void Cheat::setpid(DWORD _pid)
{
    std::cout << "THE PID IS SET :" << _pid << std::endl;
    this->pid = _pid;
}

DWORD Cheat::getpid() const
{ 
    return this->pid;
}

void Cheat::setBaseAdd(uint32_t _BaseAdd) 
{ 
    std::cout << "THE BASE ADDRESS IS SET :" << _BaseAdd << std::endl;
    this->BaseAdd = _BaseAdd;
}

uint32_t Cheat::getBaseAdd() const
{ 
    return this->BaseAdd; 
}

bool Cheat::WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16]) 
{    
    float clipX = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    float clipY = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    float clipW = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipW < 0.1f) return false;

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    screen.x = (this->width  / 2.0f) * ndcX + (this->width  / 2.0f);
    screen.y = -(this->height / 2.0f) * ndcY + (this->height / 2.0f);

    return true;
}

void Cheat::GetWindowBounds()
{
    this->game_window = FindWindowA(NULL, "AssaultCube"); 
    if (!this->game_window) {
        throw std::runtime_error("Cannot find the game window. Is the game running?");
    }
    RECT rect;
    if(GetWindowRect(this->game_window, &rect))
    {
        this->width = rect.right - rect.left;
        this->height = rect.bottom - rect.top;
        this->x = rect.left;
        this->y = rect.top;
    } 
    else 
    {
        throw std::runtime_error("Failed to get window bounds.");
    }
}

void Cheat::start_window()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    
    this->window = glfwCreateWindow(this->width, this->height, "Overlay", nullptr, nullptr);
    if (!this->window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    
    glfwSetWindowPos(this->window, this->x, this->y);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}