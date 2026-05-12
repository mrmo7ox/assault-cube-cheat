#pragma once 
#include <iostream>
#include <string>
#include "vector3.hpp"
#include <cstdint>

class Player {
private:
    Vector3 cord;
    int health;
    std::string name;
    uintptr_t address;
public:
    Player() : cord(0.0f, 0.0f, 0.0f), health(100), name("Default") {}
    Player(float _x, float _y, float _z, int _health, std::string _name)
        : cord(_x, _y, _z), health(_health), name(_name) {}
    ~Player() {}

    void set_cord(float x, float y, float z) { cord = Vector3(x, y, z); }
    void set_health(int _health) { health = _health; }

    Vector3 get_cord() const { return cord; }
    int get_health() const { return health; }
    std::string get_name() const { return name; }
    void set_address(uintptr_t player_address) { this->address = player_address;}
    uintptr_t get_address() const { return address; }
};

inline std::ostream& operator<<(std::ostream& out, const Player& obj) {
    out << "Player(" << obj.get_name() << ") - HP: " << obj.get_health() << " Pos: " << obj.get_cord();
    return out;
}