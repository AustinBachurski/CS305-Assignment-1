#include "memoryAllocator.hpp"
#include "memoryManager.hpp"

#include <cstdint>
#include <new>
#include <span>


namespace Allocator
{

std::size_t bestFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize)
{
    throw std::bad_alloc();
}

std::size_t firstFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize)
{
    std::size_t index{};
    uint8_t spanSize{};

    throw std::bad_alloc();
}

std::size_t worstFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize)
{
    throw std::bad_alloc();
}

}  // namespace Allocator

