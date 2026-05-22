#include "cheat.hpp"
#include "player.hpp"
#include "mem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <windows.h>

bool isRightMousePressed() {
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

int main(int ac, char **dc) 
{
    try
    {
        Cheat hack;
        
        hack.GetProccesname("ac_client.exe");
        hack.GetModuleBase(hack.getpid(), "ac_client.exe");
        hack.GetWindowBounds();
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
                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(40, 180),
                    ImVec2(220, 245),
                    IM_COL32(0, 255, 0, 255)
                );
                ImGui::SetCursorPos(ImVec2(50.0f, 200.0f));
                ImGui::SetWindowFontScale(2.0f);
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Cheat is ON");
                
                uint32_t localPlayerPtr = ::Read<uint32_t>(hack.getBaseAdd() + ME, hack.hProcess);  
                
                if (localPlayerPtr) 
                {
                    Player me(localPlayerPtr, hack.hProcess, "ME");
                    // std::cout << me << std::endl;
                    uint32_t entityList = ::Read<uint32_t>(hack.getBaseAdd() + OFFSET_ENTITY_LIST, hack.hProcess);
                    ViewMatrix vm = ::Read<ViewMatrix>(hack.getBaseAdd() + OFFSET_VIEW_MATRIX , hack.hProcess);
                    if(ac == 2)
                    {
                        int value = std::stoi(dc[1], nullptr, 16);
                        vm = ::Read<ViewMatrix>(hack.getBaseAdd() +  value, hack.hProcess);
                        std::cout << std::hex << value << std::endl;
                        // exit(1);

                    }
                    int player_count = ::Read<int>(hack.getBaseAdd() + OFFSET_PLAYER_COUNT, hack.hProcess);
                    // std::cout << "player count is: " << player_count << std::endl;
                    bool validTarget = false;
                    float closestDist = 999999.0f;
                    Vector2 bestAimAngles;
                    
                    for(int i = 1; i < player_count; i++)
                    {
                        try 
                        {
                            uint32_t enemyPtr = ::Read<uint32_t>(entityList + (i * 4), hack.hProcess);
                            if(!enemyPtr) continue;
                            Player enemy(enemyPtr, hack.hProcess, "Enemy " + std::to_string(i));
                            // std::cout << enemy << std::endl;

                            if(enemy.get_health() > 0 && enemy.get_health() <= 100)
                            {
                                enemy.set_health(1);
                                Vector3 feet = enemy.get_cord();
                                Vector3 head = feet;
                                head.z += 4.5f;
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
                    
                    if (validTarget && isRightMousePressed()) 
                    {
                        ::Write<Vector2>(localPlayerPtr + OFFSET_YAW, hack.hProcess, bestAimAngles);
                    }
                }
                else
                    std::cout << "no local player found" << std::endl;
            }
            catch (std::exception &e) {
                std::cout << e.what() << std::endl;
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