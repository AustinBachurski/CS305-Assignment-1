#include "memoryManager.hpp"

#include <cstdint>


int main()
{
    MemoryManager memory;

    memory.allocate(0, MemoryManager::Page{0, 1, 1, 7, MemoryManager::JobState::running, MemoryManager::JobState::sleeping});

    uint8_t currentTime = 0u;

    while(currentTime < 10u)
    {
        memory.updateState(currentTime);
        memory.displayMemoryState(currentTime);
        ++currentTime;
    }
}

