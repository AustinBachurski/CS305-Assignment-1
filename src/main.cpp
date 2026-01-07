#include "memoryAllocator.hpp"
#include "memoryManager.hpp"

#include <cstdint>


int main()
{
    MemoryManager memory;

    MemoryManager::Page page{1, 2, 1, 7, MemoryManager::JobState::running, MemoryManager::JobState::sleeping};

    memory.allocate(page, Allocator::firstFit);
    return 0;

    uint8_t currentTime = 0u;

    while(currentTime < 10u)
    {
        memory.updateState(currentTime);
        memory.displayMemoryState(currentTime);
        ++currentTime;
    }
}

