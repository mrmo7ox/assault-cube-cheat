#include "cheat.hpp"
#include "player.hpp"
#include "mem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/keysym.h>

bool isRightMousePressed(Display* dpy) {
if (!dpy) return false;
    
    Window root_return, child_return;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    
    if (XQueryPointer(dpy, DefaultRootWindow(dpy), &root_return, &child_return, 
                      &root_x, &root_y, &win_x, &win_y, &mask)) {
        return (mask & Button1Mask) != 0; 
    }
    return false;
}

int main( int ac, char **dc) 
{
    Display* dpy = nullptr; 

    try
    {
        setenv("XDG_RUNTIME_DIR", "", 1);
        setenv("DISPLAY", ":0", 1);
        char* XDG_RUNTIME_DIR = std::getenv("XDG_RUNTIME_DIR");
        char* DISPLAY = std::getenv("DISPLAY");
        std::cout << XDG_RUNTIME_DIR << " " << DISPLAY << std::endl;
        
        Cheat hack;
        hack.GetProccesname("linux_64_client");
        hack.GetModuleBase(hack.getpid(), "linux_64_client");
        hack.GetWindowBounds(hack.getpid());
        hack.start_window();
        dpy = XOpenDisplay(NULL);
        if (!dpy) {
            std::cerr << "Failed to open X display for input reading\n";
        }
        
        ImGuiIO& io = ImGui::GetIO();
        GLFWwindow* window = hack.get_window();
        
        while(!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            if (!io.WantCaptureMouse) {
                glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
            } else {
                glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
            }
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(hack.get_width(), hack.get_height()));
            ImGui::Begin("Overlay", nullptr, 
                ImGuiWindowFlags_NoBackground | 
                ImGuiWindowFlags_NoTitleBar | 
                ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove | 
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoInputs);
            
            try
            {
                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(40, 180),
                    ImVec2(220, 245),
                    IM_COL32(0, 255, 0, 255)
                );
                ImGui::SetCursorPos(ImVec2(50.0f, 200.0f));
                ImGui::SetWindowFontScale(2.0f);
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Cheat is ON");
                
                uintptr_t localPlayerPtr = ::Read<uintptr_t>(hack.getBaseAdd() + ME, hack.getpid());  
                if (localPlayerPtr) 
                {
                    Player me(localPlayerPtr, hack.getpid(), "ME");
                    uintptr_t entityList = ::Read<uintptr_t>(hack.getBaseAdd() + OFFSET_ENTITY_LIST, hack.getpid());
                    ViewMatrix vm = ::Read<ViewMatrix>(hack.getBaseAdd() + OFFSET_VIEW_MATRIX , hack.getpid());
                    
                    if(ac == 2)
                    {
                        int value = std::stoi(dc[1], nullptr, 16);
                        vm = ::Read<ViewMatrix>(hack.getBaseAdd() +  value, hack.getpid());
                        std::cout << value << std::endl;
                        // exit(1);
                    }
                    std::cout << "Matrix: ";
                    for(int i = 0; i < 16; i++) std::cout << vm.matrix[i] << " ";
                    std::cout << std::endl;
                    
                    int player_count = ::Read<int>(hack.getBaseAdd() + OFFSET_PLAYER_COUNT, hack.getpid());
                    std::cout << "====================================" << std::endl;
                    std::cout << me << std::endl;
                    
                    bool validTarget = false;
                    float closestDist = 999999.0f;
                    Vector2 bestAimAngles;
                    
                    for(int i = 1; i < player_count; i++)
                    {
                        try 
                        {
                            uintptr_t enemyPtr = ::Read<uintptr_t>(entityList + (i * 8), hack.getpid());
                            if(!enemyPtr) continue;
                            Player enemy(enemyPtr, hack.getpid(), "Enemy " + std::to_string(i));

                            if(enemy.get_health() > 0 && enemy.get_health() <= 100)
                            {
                                std::cout << enemy << std::endl;
                                Vector3 feet = enemy.get_cord();
                                Vector3 head = feet;
                                head.z += 5.0f;
                                Vector2 screenFeet, screenHead;
                                
                                float dist = enemy.distance(me.get_cord());
                                if(dist < closestDist) {
                                    closestDist = dist;
                                    bestAimAngles = enemy.CalcAngle(me.get_cord(), 0.0f);
                                    validTarget = true;
                                }
                                
                                if(hack.WorldToScreen(feet, screenFeet, vm.matrix) &&
                                   hack.WorldToScreen(head, screenHead, vm.matrix))
                                {
                                    float boxHeight = screenFeet.y - screenHead.y;
                                    float boxWidth = boxHeight * 0.4f;
                                    float topLeftX = screenHead.x - (boxWidth / 2.0f);
                                    float topLeftY = screenHead.y;
                                    std::cout << "Box -> X: " << topLeftX << " | Y: " << topLeftY << " | BottomY: " << (topLeftY + boxHeight) << std::endl;
                                    
                                    ImGui::GetWindowDrawList()->AddRect(
                                        ImVec2(topLeftX, topLeftY), 
                                        ImVec2(topLeftX + boxWidth, topLeftY + boxHeight), 
                                        IM_COL32(255, 0, 0, 255),
                                        0.0f,
                                        0,
                                        1.5f
                                    );
                                }
                            }
                        }
                        catch (...) {
                            continue;
                        }
                    }
                    
                    if (validTarget && isRightMousePressed(dpy)) 
                    {
                        ::Write<Vector2>(localPlayerPtr + OFFSET_YAW, hack.getpid(), bestAimAngles);
                    }
                }
            }
            catch (...) {
            }

            ImGui::End();

            ImGui::Render();
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
    }
    
    if (dpy) {
        XCloseDisplay(dpy);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}