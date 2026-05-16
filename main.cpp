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
            try
            {
                hack.GetWindowSize(hack.getpid());
                
                // 1. Read the Local Player pointer FIRST
                uintptr_t localPlayerPtr = ::Read<uintptr_t>(hack.getBaseAdd() + ME, hack.getpid());
                if (!localPlayerPtr) continue;

                // 2. Pass it safely to the new constructor
                Player me(localPlayerPtr, hack.getpid(), "ME");
                
                uintptr_t entityList = ::Read<uintptr_t>(hack.getBaseAdd() + OFFSET_ENTITY_LIST, hack.getpid());
                ViewMatrix vm = ::Read<ViewMatrix>(hack.getBaseAdd() + OFFSET_VIEW_MATRIX, hack.getpid());
                int player_count = ::Read<int>(hack.getBaseAdd() + OFFSET_PLAYER_COUNT, hack.getpid());
                
                std::cout << "=========================="<< std::endl;
                std::cout << me << std::endl;
                
                for(int i = 1; i < player_count; i++)
                {
                    try 
                    {
                        uintptr_t enemyPtr = ::Read<uintptr_t>(entityList + (i * 8), hack.getpid());
                        if(!enemyPtr) continue;
                        
                        // 3. Name the enemies so you can tell them apart!
                        Player enemy(enemyPtr, hack.getpid(), "Enemy " + std::to_string(i));
                        
                        std::cout << enemy << std::endl;

                        if(enemy.get_health() > 0 && enemy.get_health() <= 100)
                        {
                            Vector3 feet = enemy.get_cord();
                            Vector3 head = feet;
                            head.z += 5.0f;
                            Vector2 screenFeet, screenHead;
                            
                            if(hack.WorldToScreen(feet, screenFeet, vm.matrix) &&
                               hack.WorldToScreen(head, screenHead, vm.matrix))
                            {
                                float boxHeight = screenFeet.y - screenHead.y;
                                float boxWidth = boxHeight / 2.0f;
                                float topLeftX = screenHead.x - (boxWidth / 2.0f);
                                float topLeftY = screenHead.y;
                                
                                std::cout << "[SUCCESS] DRAW BOX AT X:" << (int)topLeftX 
                                          << " Y:" << (int)topLeftY << std::endl;
                            }
                        }
                    }
                    catch (const std::exception &e)
                    {
                        continue;
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            usleep(100000);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}