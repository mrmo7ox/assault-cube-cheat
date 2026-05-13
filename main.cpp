#include "cheat.hpp"
#include "player.hpp"
#include "mem.hpp"

int main() 
{
    try
    {
        Cheat hack;
        hack.GetProccesname("linux_64_client");
        hack.GetModuleBase(hack.getpid(), "linux_64_client");
        while(true)
        {
            Player me(hack.getBaseAdd() + ME, hack.getpid());
            std::cout << me << std::endl; 
            me.update();
            usleep(100000); 

        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // int playerCount = hack.Read<int>(base + OFFSET_PLAYER_COUNT);
    // while (true) {
    //     if (playerCount > 0 && playerCount < 32) { 
    //         std::cout << "\n--- Players in game: " << playerCount << " ---" << std::endl;
    //         uintptr_t localPlayer = hack.Read<uintptr_t>(base + 0x1A3518);
    //         if (localPlayer != 0) {
    //             int health_me = hack.Read<int>(localPlayer + OFFSET_HEALTH);
    //             Vector3 my_pos = hack.Read<Vector3>(localPlayer + OFFSET_X);
    //             std::cout << "Pos: [" << my_pos.x << ", " << my_pos.y << ", " << my_pos.z << "] " 
    //                       << "Health: " << health_me 
    //                       << " | CE Value (0x100): " << ce_value << std::endl;
    //         } else {
    //             std::cout << "LocalPlayer pointer is null! Are you in a match?" << std::endl;
    //         }
            
    //     } else {
    //         std::cout << "Waiting for game to start or invalid player count (" << playerCount << ")" << std::endl;
    //     }

    //     usleep(100000); 
    // }

    return 0;
}