#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include "job.hpp"

#include <cstdint>
#include <deque>
#include <unordered_map>


class MemoryManager
{
public:
    enum class AllocationResult : uint8_t
    {
        success,
        failure,
    };

    enum class CullingResult : uint8_t
    {
        success,
        failure,
    };

    [[nodiscard("Allocation may fail.")]]
    AllocationResult allocate(Job const& job, uint8_t const currentTime);
    bool allSleeping();
    void displayMemoryState(uint8_t const currentTime);
    void displayStaged(uint8_t const currentTime, std::deque<Job> const& jobs);
    void updateState(uint8_t const currentTime);
    void performStagedActions(uint8_t const currentTime, std::deque<Job>& jobs);

private:
    std::array<Job, 20> memory{};
    
    [[nodiscard("Culling may fail.")]]
    CullingResult cullSleepingJobsFor(uint8_t pagesToFree);
    void deallocate(uint8_t const jobNumber);
    std::unordered_map<uint8_t, uint8_t> getSleepingJobs();
    std::size_t firstFitAllocator(uint8_t const requestedSize);

};

#endif

