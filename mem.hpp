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
#include <sstream>

#define OFFSET_X      0x2C
#define OFFSET_Y      0x30
#define OFFSET_Z      0x34
#define OFFSET_HEALTH 0x100
#define OFFSET_YAW    0x38
#define OFFSET_PITCH  0x3C

#define OFFSET_PLAYER_COUNT 0x1a352c 
#define OFFSET_ENTITY_LIST  0x1a3520
#define ME                  0x1A3518
#define OFFSET_VIEW_MATRIX  0x1B4FCC
// #define OFFSET_VIEW_MATRIX  1B4F8C
// #define OFFSET_VIEW_MATRIX  1B4F90


struct ViewMatrix {
    float matrix[16];
};


template <typename T>
T Read(uintptr_t address, pid_t pid) {
    T buffer{};
    struct iovec local = {&buffer, sizeof(T)};
    struct iovec remote = {(void*)address, sizeof(T)};
    ssize_t result = process_vm_readv(pid, &local, 1, &remote, 1, 0);
    if (result == -1) {
        std::ostringstream ss;
        ss << "Read failed at address: 0x";
        ss << std::hex <<  address;
        throw std::runtime_error(ss.str());
    }
    return buffer;
}

template <typename T, pid_t pid>
void Write(uintptr_t address, T value) {
    struct iovec local = {&value, sizeof(T)};
    struct iovec remote = {(void*)address, sizeof(T)};
    ssize_t result = process_vm_writev(pid, &local, 1, &remote, 1, 0);
    if (result == -1) {
        std::ostringstream ss;
        ss << "write failed at address: 0x";
        ss << std::hex <<  address;
        throw std::runtime_error(ss.str());
    }
}