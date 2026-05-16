#include "cheat.hpp"
#include "player.hpp"
#include "mem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <cstdlib>

int main( int ac, char **dc) 
{
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
                ImGui::GetWindowDrawList()->AddRect(
                    ImVec2(100, 100),
                    ImVec2(300, 300),
                    IM_COL32(0, 255, 0, 255),
                    0.0f,
                    0,
                    2.0f
                );
                uintptr_t localPlayerPtr = ::Read<uintptr_t>(hack.getBaseAdd() + ME, hack.getpid());
                
                if (localPlayerPtr) 
                {
                    Player me(localPlayerPtr, hack.getpid(), "ME");
                    uintptr_t entityList = ::Read<uintptr_t>(hack.getBaseAdd() + OFFSET_ENTITY_LIST, hack.getpid());
                    ViewMatrix vm = ::Read<ViewMatrix>(hack.getBaseAdd() + OFFSET_ENTITY_LIST , hack.getpid());
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
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}