#ifndef MEM_HPP
#define MEM_HPP

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <stdexcept>

#include "vector3.hpp"
#include "player.hpp" 
#define M_PI 3.14159265359
#define OFFSET_X      0x28
#define OFFSET_Y      0x2C
#define OFFSET_Z      0x30
#define OFFSET_HEAD_X 0x04
#define OFFSET_HEAD_Y 0x08
#define OFFSET_HEAD_Z 0x0C
#define OFFSET_HEALTH 0xEC
#define OFFSET_YAW    0x34
#define OFFSET_PITCH  0x38

#define OFFSET_PLAYER_COUNT 0x18AC0C
#define OFFSET_ENTITY_LIST  0x18AC04
#define ME                  0x17E254
#define OFFSET_VIEW_MATRIX  0x17dfd0


struct ViewMatrix {
    float matrix[16];
};


template <typename T>
T Read(uint32_t address, HANDLE hProcess) {
    T buffer{};
    SIZE_T bytesRead;
    if (!ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)address, &buffer, sizeof(T), &bytesRead)) {
        std::ostringstream ss;
        ss << "Read failed at address: 0x" << std::hex << address << " | Error: " << std::dec << GetLastError();
        throw std::runtime_error(ss.str());
    }
    return buffer;
}

template <typename T>
void Write(uint32_t address, HANDLE hProcess, T value) {
    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, (LPVOID)(uintptr_t)address, &value, sizeof(T), &bytesWritten)) {
        std::ostringstream ss;
        ss << "Write failed at address: 0x" << std::hex << address << " | Error: " << std::dec << GetLastError();
        throw std::runtime_error(ss.str());
    }
}

#endif