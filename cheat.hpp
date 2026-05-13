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

// #define X_OFFEST 0x2b03ffc - 0x2B03FD0
// #define Y_OFFEST 0x2b04000 - 0x2B03FD0
// #define Z_OFFEST 0x2b04004 - 0x2B03FD0

// #define X_MOUSE 0x2b04008 - 0x2B03FD0
// #define Y_MOUSE 0x2b0400c - 0x2B03FD0

#define OFFSET_X      0x2C
#define OFFSET_Y      0x30
#define OFFSET_Z      0x34
#define OFFSET_HEALTH 0x100
#define OFFSET_YAW    0x38
#define OFFSET_PITCH  0x3C

#define OFFSET_PLAYER_COUNT 0x1a352c 
#define OFFSET_ENTITY_LIST   0x1a3520

class Cheat {
private:
    pid_t pid;
    uintptr_t entity_address;
    uintptr_t BaseAdd;
public:
    Cheat();
    ~Cheat();

    pid_t GetProccesname(const std::string & name);
    uintptr_t GetModuleBase(pid_t pid, const std::string& moduleName);
    void HealthHack(bool v);
    void run();

    void setpid(pid_t _pid) { this->pid = _pid; }
    pid_t getpid() { return this->pid; }

    void setBaseAdd(uintptr_t _BaseAdd) { this->BaseAdd = _BaseAdd; }
    uintptr_t getBaseAdd() { return this->BaseAdd; }

    template <typename T>
    T Read(uintptr_t address) {
        T buffer{};
        
        struct iovec local = {&buffer, sizeof(T)};
        struct iovec remote = {(void*)address, sizeof(T)};
        
        ssize_t result = process_vm_readv(this->pid, &local, 1, &remote, 1, 0);
        
        if (result == -1) {
            std::cerr << "Read failed at address: 0x" << std::hex << address << std::endl;
        }
        
        return buffer;
    }

    template <typename T>
    void Write(uintptr_t address, T value) {
        struct iovec local = {&value, sizeof(T)};
        struct iovec remote = {(void*)address, sizeof(T)};
        process_vm_writev(this->pid, &local, 1, &remote, 1, 0);
    }
};