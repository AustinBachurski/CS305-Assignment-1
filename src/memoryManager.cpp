#include "memoryManager.hpp"

#include <print>
#include <ranges>
#include <span>

namespace MemoryManager
{

void allocate(std::span<Page, 20> memory, std::size_t const index, Page const& page)
{
    memory[index] = page;
}

void displayMemoryState(std::span<Page const, 20> memory, uint8_t const currentTime)
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

void updateState(std::span<Page, 20> memory, uint8_t const currentTime)
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

} // namespace MemoryManager
