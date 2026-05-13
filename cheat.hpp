#pragma once 
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <sys/uio.h>
#include "vector3.hpp"
#include "player.hpp"
#include <unistd.h>


class Cheat {
    private:
        pid_t       pid;
        uintptr_t   entity_address;
        uintptr_t   BaseAdd;
    public:
        Cheat();
        ~Cheat();

        void GetProccesname(const std::string & name);
        void GetModuleBase(pid_t pid, const std::string& moduleName);
        void HealthHack(bool v);
        void run();

        void setpid(pid_t _pid)  ;
        pid_t getpid() const;

        void setBaseAdd(uintptr_t _BaseAdd)  ;
        uintptr_t getBaseAdd() const;

   
};