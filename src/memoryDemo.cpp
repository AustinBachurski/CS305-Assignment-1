#include "memoryManager.hpp"
#include "job.hpp"

#include <cstdint>
#include <functional>
#include <print>
#include <span>


void demo(std::function<std::size_t(std::span<Job const, 20>, uint8_t const)> allocator)
{
    MemoryManager memory(allocator);

    Job page{1, 2, 1, 7, JobState::running, JobState::end};

    if (memory.allocate(page) == MemoryManager::AllocationResult::failure)
    {
        std::println("allocation failed!");
    }

    uint8_t currentTime = 0u;

    while(currentTime < 10u)
    {
        memory.updateState(currentTime);
        memory.displayMemoryState(currentTime);
        ++currentTime;
    }
}

