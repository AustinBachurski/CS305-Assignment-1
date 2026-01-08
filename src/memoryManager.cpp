#include "memoryManager.hpp"
#include "job.hpp"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <new>
#include <print>
#include <ranges>
#include <set>


MemoryManager::MemoryManager(AllocatorCallable allocator)
: allocatorCallable{ allocator }
{ }

MemoryManager::AllocationResult MemoryManager::allocate(Job const& job)
{
    try
    {
        std::size_t index{ allocatorCallable(memory, job.memorySize) };

        for (auto const i : std::views::iota(index, index + job.memorySize))
        {
            memory[i] = job;
            memory[i].currentState = JobState::running;
        }

        return AllocationResult::success;
    }
    catch (std::bad_alloc&)
    {
        return AllocationResult::failure;
    }
}

bool MemoryManager::allSleeping()
{
    return std::ranges::all_of(memory, [](auto const& job)
           {
               if (job.jobID)
               {
                   return job.currentState == JobState::sleeping;
               }
               return true;
           });
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
            std::println("{}{}", job, std::string(static_cast<uint8_t>((job.startTime + job.runTime) - currentTime), '.'));
        }
        else
        {
            std::println("{}", job);
        }
    }

    std::println("***********************************************************");
    std::println("Staged Commands:");
}

void MemoryManager::displayStaged(uint8_t const currentTime, std::deque<Job> const& jobs)
{
    std::set<uint8_t> seen;

    for (auto const& job : memory)
    {
        if (job.jobID && job.currentState == JobState::end && seen.insert(job.jobID).second)
        {
            std::println("  - Deallocate Job {}, freeing {} pages.", job.jobID, job.memorySize);
        }
    }

    for (auto const& job : blocked)
    {
        std::println("  - Allocate {}: requesting {} pages for {} seconds.", job, job.memorySize, job.runTime);
    }

    for (auto const& job : jobs)
    {
        if (job.startTime <= currentTime || job.currentState == JobState::blocked)
        {
            std::println("  - Allocate {}: requesting {} pages for {} seconds.", job, job.memorySize, job.runTime);
        }
    }
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

void MemoryManager::performStagedActions(uint8_t const currentTime, std::deque<Job>& jobs)
{
    for (auto const& job : memory)
    {
        if (job.jobID && job.currentState == JobState::end)
        {
            deallocate(job.jobID);
        }
    }

    // No difference in output using this method.
    //std::erase_if(blocked, [this](auto const& job)
    //              { return allocate(job) == AllocationResult::success; });

    while (!jobs.empty() && jobs.front().startTime <= currentTime)
    {
        if (allocate(jobs.front()) == AllocationResult::success)
        {
            jobs.pop_front();
        }
        else
        {
            jobs.front().currentState = JobState::blocked;
            blocked.push_back(jobs.front());
            jobs.pop_front();
        }
    }
}

