#include "memoryManager.hpp"

#include <array>
#include <cstdint>


int main()
{
    std::array<MemoryManager::Page, 20> memory{};

    MemoryManager::allocate(memory, 0, MemoryManager::Page{0, 1, 1, 7, MemoryManager::JobState::running, MemoryManager::JobState::sleeping});

    uint8_t currentTime = 0u;

    while(currentTime < 10u)
    {
        MemoryManager::updateState(memory, currentTime);
        MemoryManager::displayMemoryState(memory, currentTime);
        ++currentTime;
    }
}

