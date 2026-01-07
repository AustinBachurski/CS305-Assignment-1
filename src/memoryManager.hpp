#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include "job.hpp"

#include <cstdint>
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
    void displayMemoryState(uint8_t const currentTime);
    void updateState(uint8_t const currentTime);

private:
    AllocatorCallable allocatorCallable;
    std::array<Job, 20> memory{};
    void deallocate(uint8_t const jobNumber);

};

#endif

