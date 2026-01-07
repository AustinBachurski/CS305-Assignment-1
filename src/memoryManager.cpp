#include "memoryManager.hpp"
#include "job.hpp"

#include <cstdint>
#include <functional>
#include <print>
#include <ranges>
#include <span>


MemoryManager::MemoryManager(AllocatorCallable allocator)
: allocator{ allocator }
{ }

void MemoryManager::allocate(Job const& job)
{
    std::size_t index{ allocator(memory, job.memorySize) };
    std::println("{}", index);
}

void MemoryManager::displayMemoryState(uint8_t const currentTime)
{
    std::println("***********************************************************");
    std::println("*             MEMORY STATE AT TIMEPOINT {:<2}                *", currentTime);
    std::println("***********************************************************");

    for (auto const [i, page] : std::views::enumerate(memory))
    {
        std::print("  Page {:2} -> ", i);

        if (!page.jobID)
        {
            std::println();
        }
        else if (page.currentState == JobState::running)
        {
            std::println("{}{}", page, std::string(page.runTime - currentTime, '.'));
        }
        else if (page.currentState == JobState::sleeping)
        {
            std::println("{}", page);
        }
        else
        {
            std::println("{} <- Shouldn't be here!", page);
        }
    }

    std::println("***********************************************************");
    std::println("Staged Commands:");
    std::println();
}

void MemoryManager::updateState(uint8_t const currentTime)
{
    for (auto& page : memory)
    {
        if (!page.jobID || page.currentState == JobState::sleeping)
        {
            continue;
        }

        if (page.currentState == JobState::running)
        {
            if (currentTime - page.startTime == page.runTime)
            {
                page.currentState = page.endState;
            }
        }
    }
}

