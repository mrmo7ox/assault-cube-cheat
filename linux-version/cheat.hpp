#pragma once 
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <sys/uio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "vector3.hpp"
#include "player.hpp"

class Cheat {
    private:
        pid_t       pid;
        uintptr_t   entity_address;
        uintptr_t   BaseAdd;
        int         width;
        int         height;
        Window      game_window;
        int         x;
        int         y;
        GLFWwindow* window;
    public:
        Cheat();
        ~Cheat();

        void GetProccesname(const std::string & name);
        void GetModuleBase(pid_t pid, const std::string& moduleName);
        void HealthHack(bool v);
        void run();

        void setpid(pid_t _pid);
        pid_t getpid() const;

        void setBaseAdd(uintptr_t _BaseAdd);
        uintptr_t getBaseAdd() const;
        bool WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16]);
        Window FindWindowByPID(Display* dpy, Window top, unsigned long target_pid);
        void GetWindowBounds(int pid);

        void start_window();
        GLFWwindow* get_window() const { return this->window; }
        int get_width() const { return this->width; }
        int get_height() const { return this->height; }
};