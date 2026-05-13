#include "cheat.hpp"

Cheat::Cheat(): pid(0), entity_address(0), BaseAdd(0) {
    std::cout << "Cheat initialize" << std::endl;
}

Cheat::~Cheat() {}

void Cheat::run()
{
}

void Cheat::GetProccesname(const std::string & name)
{
    pid_t pid = -1;
    DIR *dir = opendir("/proc");

    if (!dir)
        throw std::runtime_error("dir is not opened line 19");
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
    if(pid == -1)
        throw std::runtime_error("Could not find game process. Is it running? line 43");
    this->setpid(pid);
}

void Cheat::HealthHack(bool v)
{
    if(v)
    {

    }
};

void Cheat::GetModuleBase(pid_t pid, const std::string& moduleName)
{
    std::string path = "/proc/" + std::to_string(pid) + "/maps";
    std::ifstream maps(path);
    std::string line;

    while(std::getline(maps, line))
    {
        if(line.find(moduleName) != std::string::npos)
        {
            std::string addrHex =line.substr(0, line.find("-"));
            this->setBaseAdd(std::stoul(addrHex, nullptr, 16));
            break;
        }
    }
    if(this->BaseAdd == 0)
        throw std::runtime_error("cant find the base address");
}



void Cheat::setpid(pid_t _pid)
{
    std::cout << "THE PID IS SET :" << _pid << std::endl;
    this->pid = _pid;
}

pid_t Cheat::getpid() const
{ 
    return this->pid;
}

void Cheat::setBaseAdd(uintptr_t _BaseAdd) 
{ 
    std::cout << "THE BASE ADDRESS IS SET :" << _BaseAdd << std::endl;
    this->BaseAdd = _BaseAdd;
 }

uintptr_t Cheat::getBaseAdd() const
{ 
    return this->BaseAdd; 
}