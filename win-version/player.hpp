#pragma once 
#include <iostream>
#include <string>
#include "vector3.hpp"
#include "vector2.hpp"
#include <cstdint>
#include "mem.hpp"
#include <math.h>

class Player {
    private:
        Vector3 cord;
        Vector2 mouse;
        int health;
        std::string name;
        uint32_t address;
    public:
        HANDLE hProcess;
        Player() : cord(0.0f, 0.0f, 0.0f), health(100), name("Default"), hProcess(NULL) {}
        
        Player(uint32_t entity_address, HANDLE _hProcess, std::string _name)
        {
            this->address = entity_address;
            this->hProcess = _hProcess; 
            this->name = _name;
            this->update();
        }
        ~Player() {}

        void set_cord(float x, float y, float z) { cord = Vector3(x, y, z); }
        void set_health(int _health) { 
            ::Write(this->address + OFFSET_HEALTH, this->hProcess, _health);
        }

        Vector3 get_cord() const { return cord; }
        int get_health() const { return health; }
        Vector2 get_mouse() const { return mouse; }
        std::string get_name() const { return name; }
        void set_address(uint32_t player_address) { this->address = player_address;}
        uint32_t get_address() const { return address; }

        void update()
        {
            this->cord   = ::Read<Vector3>(this->address + OFFSET_X, this->hProcess);
            this->health = ::Read<int>(this->address + OFFSET_HEALTH, this->hProcess);
            this->mouse  = ::Read<Vector2>(this->address + OFFSET_YAW, this->hProcess);
        }
        float distance(const Vector3& src)
        {
            return std::sqrt(std::pow(this->cord.x - src.x, 2) +
                             std::pow(this->cord.y - src.y, 2) +
                             std::pow(this->cord.z - src.z, 2));
        };

        Vector2 CalcAngle(const Vector3& src, float z_offset = 2.0f) 
        {
            Vector2 angles;
            Vector3 delta = {this->cord.x - src.x,  this->cord.y - src.y, (this->cord.z + z_offset) - src.z}; 

            float hyp = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            angles.x = (std::atan2(delta.y, delta.x) * 180.0f / M_PI) + 90.0f;
            angles.y = std::atan2(delta.z, hyp) * 180.0f / M_PI;

            return angles;
        }
};

inline std::ostream& operator<<(std::ostream& out, const Player& obj) {
    out << "Player(" << obj.get_name() << ") - HP: " << obj.get_health() << " Pos: " << obj.get_cord() << " MOUSE X/Y: " << obj.get_mouse();
    return out;
}