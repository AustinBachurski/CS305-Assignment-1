#include "memoryAllocator.hpp"
#include "memoryManager.hpp"
#include "job.hpp"

#include <cstdint>


int main()
{
    MemoryManager memory(Allocator::firstFit);

    Job page{1, 2, 1, 7, JobState::running, JobState::sleeping};

    memory.allocate(page);
    return 0;

    uint8_t currentTime = 0u;

    while(currentTime < 10u)
    {
        memory.updateState(currentTime);
        memory.displayMemoryState(currentTime);
        ++currentTime;
    }
}

