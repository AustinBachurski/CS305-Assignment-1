#include "memoryManager.hpp"
#include "job.hpp"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <new>
#include <print>
#include <ranges>
#include <set>

// The methods in this file are described with comments in `memoryManager.hpp`

MemoryManager::AllocationResult MemoryManager::allocate(Job const& job, uint8_t const currentTime)
{
    try
    {
        std::size_t index{ firstFitAllocator(job.memorySize) };

        for (auto const i : std::views::iota(index, index + job.memorySize))
        {
            memory[i] = job;
            memory[i].startTime = currentTime;

            if (job.jobType == JobType::job)
            {
                memory[i].currentState = JobState::running;
            }
            else
            {
                memory[i].currentState = JobState::open;
            }
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
    std::println();
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
        else if (job.currentState == JobState::running || job.currentState == JobState::open)
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
            if (job.jobType == JobType::job)
            {
                std::println("  - Deallocate Job {}, freeing {} pages.", job.jobID, job.memorySize);
            }
            else
            {
                std::println("  - Deallocate File {}, freeing {} pages.", job.jobID, job.memorySize);
            }
        }
    }

    for (auto const& job : jobs)
    {
        if (job.startTime <= currentTime || job.currentState == JobState::blocked)
        {
            std::println("  - Allocate {}: requesting {} pages for {} seconds.", job, job.memorySize, job.runTime);
        }
    }
}

void MemoryManager::updateState(uint8_t const currentTime)
{
    for (auto& job : memory)
    {
        if (!job.jobID || job.currentState == JobState::sleeping)
        {
            continue;
        }

        if (job.currentState == JobState::running || job.currentState == JobState::open)
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

MemoryManager::CullingResult MemoryManager::cullSleepingJobsFor(uint8_t pagesToFree)
{
    std::println("    INSUFFICIENT MEMORY FOR ALLOCATION");
    std::set<uint8_t> ids;
    uint8_t cumulativeSpace{};

    for (auto const& job : memory)
    {
        if (job.jobID && job.currentState == JobState::sleeping)
        {
            if (job.memorySize >= pagesToFree)
            {
                std::println("    - Cull sleeping job {} for {} pages.", job.jobID, job.memorySize);
                deallocate(job.jobID);
                return MemoryManager::CullingResult::success;
            }

            if (ids.insert(job.jobID).second)
            {
                cumulativeSpace += job.memorySize;

                if (cumulativeSpace >= pagesToFree)
                {
                    std::println("    - Cull sleeping jobs {} for {} pages.", ids, cumulativeSpace);

                    for (auto const id : ids)
                    {
                        deallocate(id);
                    }

                    return MemoryManager::CullingResult::success;
                }
            }
        }
    }

    std::println("    - Insufficient sleeping processes, blocking job.");
    return MemoryManager::CullingResult::failure;
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

    while (!jobs.empty() && jobs.front().startTime <= currentTime)
    {
        if (allocate(jobs.front(), currentTime) == AllocationResult::success)
        {
            jobs.pop_front();
        }
        else if (cullSleepingJobsFor(jobs.front().memorySize) == CullingResult::success)
        {
            // If culling succeeded, memory is now available; try again.
            continue;
        }
        else
        {
            jobs.front().currentState = JobState::blocked;
            break;
        }
    }
}

std::size_t MemoryManager::firstFitAllocator(uint8_t const requestedSize)
{
    std::size_t index{};
    uint8_t spanSize{};

    for (auto const [i, job] : std::views::enumerate(memory))
    {
        if (!job.jobID)    // At a free space.
        {
            if (!spanSize)  // Not currently traversing free space.
            {
                index = static_cast<std::size_t>(i);
            }

            ++spanSize;

            if (requestedSize == spanSize)
            {
                return index;
            }
        }
        else
        {
            spanSize = 0;
        }
    }

    throw std::bad_alloc();
}

