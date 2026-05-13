#include "cheat.hpp"

Cheat::Cheat() {
    std::cout << "Cheat initialize" << std::endl;
}

Cheat::~Cheat() {}

void Cheat::run()
{
}

pid_t Cheat::GetProccesname(const std::string & name)
{
    pid_t pid = -1;
    DIR *dir = opendir("/proc");

    if (!dir)
        return pid;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            std::string path = "/proc/";
            path += entry->d_name;
            path += "/comm";

            std::ifstream commFILE(path);
            std::string _name;
            if (std::getline(commFILE, _name))
            {
                if (name == _name)
                {
                    pid = std::atoi(entry->d_name);
                    break;
                }
            }
        }
    }
    closedir(dir);
    this->setpid(pid);
    return pid;
}

void Cheat::HealthHack(bool v)
{
    if(v)
    {

    }
};

uintptr_t Cheat::GetModuleBase(pid_t pid, const std::string& moduleName)
{
    std::string path = "/proc/" + std::to_string(pid) + "/maps";
    std::ifstream maps(path);
    std::string line;

    while(std::getline(maps, line))
    {
        if(line.find(moduleName) != std::string::npos)
        {
            std::string addrHex =line.substr(0, line.find("-"));
            setBaseAdd(std::stoul(addrHex, nullptr, 16));
            return std::stoul(addrHex, nullptr, 16);
        }
    }
    return 0;
}

#include "cheat.hpp"
#include <unistd.h>

int main() {
    Cheat hack;
    
    pid_t pid = hack.GetProccesname("linux_64_client");
    if (pid == -1) {
        std::cerr << "[-] Could not find game process. Is it running?" << std::endl;
        return 1;
    }
    hack.setpid(pid);

    uintptr_t base = hack.GetModuleBase(pid, "linux_64_client");
    if (base == 0) {
        std::cerr << "[-] Could not find module base. Check permissions!" << std::endl;
        return 1;
    }
    std::cout << "[+] Game Found! PID: " << pid << std::endl;
    std::cout << "[+] Module Base: 0x" << std::hex << base << std::dec << std::endl;
    // exit(1);

    while (true) {
        int playerCount = hack.Read<int>(base + OFFSET_PLAYER_COUNT);
        
        if (playerCount > 0 && playerCount < 32) { 
            std::cout << "\n--- Players in game: " << playerCount << " ---" << std::endl;
            uintptr_t entityList = hack.Read<uintptr_t>(base + OFFSET_ENTITY_LIST);

            for (int i = 1; i < playerCount; i++) {
                uintptr_t enemyPtr = hack.Read<uintptr_t>(entityList + (i * 8));
                if (enemyPtr != 0) { 
                    int health = hack.Read<int>(enemyPtr + OFFSET_HEALTH);
                    hack.Write(enemyPtr + OFFSET_HEALTH, 1);
                    Vector3 pos = hack.Read<Vector3>(enemyPtr + OFFSET_X);
                    Vector3 my_pos = hack.Read<Vector3>(base + 0x261B0FD0 + OFFSET_X);
                    hack.Write(enemyPtr + OFFSET_X, my_pos);

                    std::cout << "[Enemy " << i << "] Health: " << health 
                              << " | Pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
                }
            }
        } else {
            std::cout << "Waiting for game to start..." << std::endl;
        }

        usleep(10000000);
    }

    return 0;
}