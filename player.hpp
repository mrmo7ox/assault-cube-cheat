#pragma once 
#include <iostream>
#include <string>
#include "vector3.hpp"
#include "vector2.hpp"
#include <cstdint>
#include "mem.hpp"

class Player {
    private:
        Vector3 cord;
        Vector2 mouse;
        int health;
        std::string name;
        uintptr_t address;
        pid_t pid;
    public:
        Player() : cord(0.0f, 0.0f, 0.0f), health(100), name("Default") {}
        Player(uintptr_t playerAdd, pid_t _pid)
        {
            this->pid = _pid;
            this->address = ::Read<uintptr_t>(playerAdd, this->pid);
            this->cord = ::Read<Vector3>(this->address + OFFSET_X, this->pid);
            this->health = ::Read<int>(this->address + OFFSET_HEALTH, this->pid);
            this->mouse = ::Read<Vector2>(this->address + OFFSET_YAW, this->pid);
            this->name = "ME";
        }
        ~Player() {}

        void set_cord(float x, float y, float z) { cord = Vector3(x, y, z); }
        void set_health(int _health) { health = _health; }

        Vector3 get_cord() const { return cord; }
        int get_health() const { return health; }
        Vector2 get_mouse() const { return mouse; }
        std::string get_name() const { return name; }
        void set_address(uintptr_t player_address) { this->address = player_address;}
        uintptr_t get_address() const { return address; }

        void update()
        {
            this->cord = ::Read<Vector3>(this->address + OFFSET_X, this->pid);
            this->health = ::Read<int>(this->address + OFFSET_HEALTH, this->pid);
            this->name = "ME";
        }
};

inline std::ostream& operator<<(std::ostream& out, const Player& obj) {
    out << "Player(" << obj.get_name() << ") - HP: " << obj.get_health() << " Pos: " << obj.get_cord() << " MOUSE X/Y: " << obj.get_mouse();
    return out;
}