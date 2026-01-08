#include "memoryManager.hpp"
#include "job.hpp"

#include <cstdint>
#include <new>
#include <print>
#include <ranges>


MemoryManager::MemoryManager(AllocatorCallable allocator)
: allocatorCallable{ allocator }
{ }

MemoryManager::AllocationResult MemoryManager::allocate(Job const& job)
{
    try
    {
        std::size_t index{ allocatorCallable(memory, job.memorySize) };

        for (auto const i : std::views::iota(0uz, index + 1))
        {
            memory[i] = job;
        }

        return AllocationResult::success;
    }
    catch (std::bad_alloc&)
    {
        return AllocationResult::failure;
    }
}

void MemoryManager::displayMemoryState(uint8_t const currentTime)
{
    std::println("***********************************************************");
    std::println("*             MEMORY STATE AT TIMEPOINT {:<2}                *", currentTime);
    std::println("***********************************************************");

    for (auto const [i, job] : std::views::enumerate(memory))
    {
        std::print("  Page {:2} -> ", i);

        if (!job.jobID)
        {
            std::println();
        }
        else if (job.currentState == JobState::running)
        {
            std::println("{}{}", job, std::string(job.runTime - currentTime, '.'));
        }
        else
        {
            std::println("{}", job);
        }
    }

    std::println("***********************************************************");
    std::println("Staged Commands:");
    std::println();
}

void MemoryManager::updateState(uint8_t const currentTime)
{
    for (auto& job : memory)
    {
        if (!job.jobID || job.currentState == JobState::sleeping)
        {
            continue;
        }

        if (job.currentState == JobState::running)
        {
            if (currentTime - job.startTime == job.runTime)
            {
                job.currentState = job.endState;
            }
        }
        else if (job.currentState == JobState::end)
        {
            deallocate(job.jobID);
        }
    }
}


void MemoryManager::deallocate(uint8_t const jobNumber)
{
    for (auto& job : memory)
    {
        if (jobNumber == job.jobID)
        {
            job = {};
        }
    }
}

