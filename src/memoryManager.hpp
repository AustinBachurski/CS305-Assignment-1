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
    MemoryManager(AllocatorCallable allocator);


    void allocate(Job const& job);
    void displayMemoryState(uint8_t const currentTime);
    void updateState(uint8_t const currentTime);

private:
    AllocatorCallable allocator;
    std::array<Job, 20> memory{};

};

#endif

