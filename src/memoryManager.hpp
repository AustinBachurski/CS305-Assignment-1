#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include "job.hpp"

#include <cstdint>
#include <deque>


// Class for managing the pseudo-memory for the program.
class MemoryManager
{
public:
    // Return type for the `allocate` method, expresses intent better than a bool.
    enum class AllocationResult : uint8_t
    {
        success,
        failure,
    };

    // Return type for the `cullSleepingJobsFor` method, expresses intent better than a bool.
    enum class CullingResult : uint8_t
    {
        success,
        failure,
    };

    // Attempts to allocate memory for a job, returns an AllocationResult of success or failure.
    // Marked [[nodiscard]] as not checking for allocation failure is an error.
    [[nodiscard("Allocation may fail.")]] AllocationResult allocate(Job const& job, uint8_t const currentTime);

    // Checks to see if all jobs are sleeping for program termination.
    bool allSleeping();

    // Prints the current state of each job in the memory array.
    void displayMemoryState(uint8_t const currentTime);

    // Prints the actions that will be taken when current time becomes current time + 1.
    void displayStaged(uint8_t const currentTime, std::deque<Job> const& jobs);

    // Updates the state of each job in the memory array given the current time.
    void updateState(uint8_t const currentTime);

    // Performs the actions previously described in staging.
    void performStagedActions(uint8_t const currentTime, std::deque<Job>& jobs);

private:
    // This is the pseudo-memory that is managed by the program.
    std::array<Job, 20> memory{};
    
    // Attempts to cull sleeping jobs to free memory equal to or greater than the `pagesToFree` parameter.
    // Marked [[nodiscard]] as not checking whether memory was successfully freed is an error.
    [[nodiscard("Culling may fail.")]] CullingResult cullSleepingJobsFor(uint8_t pagesToFree);

    // Frees memory occupied by the `jobNumber` parameter.
    void deallocate(uint8_t const jobNumber);

    // Allocator algorithm used by the `allocate` method.
    std::size_t firstFitAllocator(uint8_t const requestedSize);

};

#endif

