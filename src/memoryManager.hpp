#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include "job.hpp"

#include <cstdint>
#include <deque>
#include <functional>
#include <span>


class MemoryManager
{
public:
    using AllocatorCallable = std::function<std::size_t(std::span<Job const, 20>, uint8_t const)>;
    explicit MemoryManager(AllocatorCallable allocator);

    enum class AllocationResult : uint8_t
    {
        success,
        failure,
    };

    [[nodiscard("Allocation may throw.")]]
    AllocationResult allocate(Job const& job);
    bool allSleeping();
    void displayMemoryState(uint8_t const currentTime);
    void displayStaged(uint8_t const currentTime, std::deque<Job> const& jobs);
    void updateState(uint8_t const currentTime);
    void performStagedActions(uint8_t const currentTime, std::deque<Job>& jobs);

private:
    AllocatorCallable allocatorCallable;
    std::array<Job, 20> memory{};
    void deallocate(uint8_t const jobNumber);

};

#endif

