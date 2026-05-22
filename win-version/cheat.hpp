#pragma once 
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "vector3.hpp"
#include "vector2.hpp"
#include "player.hpp"

class Cheat {
    private:
        DWORD       pid;
        uint32_t   entity_address;
        uint32_t   BaseAdd;
        int         width;
        int         height;
        HWND        game_window;
        int         x;
        int         y;
        GLFWwindow* window;
        
    public:
        HANDLE hProcess;
        Cheat();
        ~Cheat();

        void GetProccesname(const std::string & name);
        void GetModuleBase(DWORD pid, const std::string& moduleName);
        void HealthHack(bool v);
        void run();

        void setpid(DWORD _pid);
        DWORD getpid() const;

        void setBaseAdd(uint32_t _BaseAdd);
        uint32_t getBaseAdd() const;
        
        bool WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16]);
        void GetWindowBounds();

        void start_window();
        
        GLFWwindow* get_window() const { return this->window; }
        int get_width() const { return this->width; }
        int get_height() const { return this->height; }
};