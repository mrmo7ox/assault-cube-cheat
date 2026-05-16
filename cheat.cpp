#include "cheat.hpp"

Cheat::Cheat(): pid(0), entity_address(0), BaseAdd(0), window(nullptr) {
    std::cout << "Cheat initialize" << std::endl;
}

Cheat::~Cheat() {
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
    pid_t p_id = -1;
    DIR *dir = opendir("/proc");

    if (!dir)
        throw std::runtime_error("dir is not opened line 19");
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            std::string path = "/proc/";
            path += entry->d_name;
            path += "/comm";

            std::ifstream commFILE(path);
            std::string _name;
            if (std::getline(commFILE, _name))
            {
                if (name == _name)
                {
                    p_id = std::atoi(entry->d_name);
                    break;
                }
            }
        }
    }
    closedir(dir);
    if(p_id == -1)
        throw std::runtime_error("Could not find game process. Is it running? line 43");
    this->setpid(p_id);
}

void Cheat::HealthHack(bool v)
{
    if(v)
    {

    }
}

void Cheat::GetModuleBase(pid_t _pid, const std::string& moduleName)
{
    std::string path = "/proc/" + std::to_string(_pid) + "/maps";
    std::ifstream maps(path);
    std::string line;

    while(std::getline(maps, line))
    {
        if(line.find(moduleName) != std::string::npos)
        {
            std::string addrHex =line.substr(0, line.find("-"));
            this->setBaseAdd(std::stoul(addrHex, nullptr, 16));
            break;
        }
    }
    if(this->BaseAdd == 0)
        throw std::runtime_error("cant find the base address");
}

void Cheat::setpid(pid_t _pid)
{
    std::cout << "THE PID IS SET :" << _pid << std::endl;
    this->pid = _pid;
}

pid_t Cheat::getpid() const
{ 
    return this->pid;
}

void Cheat::setBaseAdd(uintptr_t _BaseAdd) 
{ 
    std::cout << "THE BASE ADDRESS IS SET :" << _BaseAdd << std::endl;
    this->BaseAdd = _BaseAdd;
}

uintptr_t Cheat::getBaseAdd() const
{ 
    return this->BaseAdd; 
}

bool Cheat::WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16]) 
{
    float clipX = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8]  + matrix[12];
    float clipY = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9]  + matrix[13];
    // float clipZ = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    float clipW = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipW < 0.1f) {
        return false; 
    }

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    screen.x = (this->width / 2.0f * ndcX) + (this->width / 2.0f);
    screen.y = -(this->height / 2.0f * ndcY) + (this->height / 2.0f);
    
    return true; 
}

Window Cheat::FindWindowByPID(Display* dpy, Window top, unsigned long target_pid)
{
    Atom prop = XInternAtom(dpy, "_NET_WM_PID", True);
    Atom type;
    int format;
    unsigned long nItems;
    unsigned long bytesAfter;
    unsigned char* data = 0;

    if (XGetWindowProperty(dpy, top, prop, 0, 1, False, AnyPropertyType, &type, &format, &nItems, &bytesAfter, &data) == Success)
    {
        if (data != nullptr)
        {
            unsigned long window_pid = *((unsigned long*)data);
            XFree(data);
            if(window_pid == target_pid) {
                return top;
            }
        }
    }

    Window  root;
    Window  parent;
    Window  *children;
    unsigned int num;
    
    if(XQueryTree(dpy, top, &root, &parent, &children, &num))
    {
        for(unsigned int i = 0; i < num; i++)
        {
            Window w = FindWindowByPID(dpy, children[i], target_pid);
            if(w)
            {
                XFree(children);
                return w;
            }
        }
        if(children)
            XFree(children);
    }
    
    return 0;
}

void Cheat::GetWindowBounds(int _pid)
{
    Display* dpy = XOpenDisplay(NULL);
    if(!dpy)
        throw std::runtime_error("cant create a dpy line 113");
        
    this->game_window = FindWindowByPID(dpy, DefaultRootWindow(dpy), _pid);
    if(!this->game_window)
    {
        XCloseDisplay(dpy);
        throw std::runtime_error("cant get game window line 119");
    }
    
    XWindowAttributes attr;
    XGetWindowAttributes(dpy, this->game_window, &attr);
    this->width = attr.width;
    this->height = attr.height;

    int dest_x, dest_y;
    Window child;
    XTranslateCoordinates(dpy, this->game_window, DefaultRootWindow(dpy), 0, 0, &dest_x, &dest_y, &child);
    
    this->x = dest_x;
    this->y = dest_y;

    XCloseDisplay(dpy);
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