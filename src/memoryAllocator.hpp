#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "memoryManager.hpp"

#include <span>

namespace Allocator
{

std::size_t bestFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize);
std::size_t firstFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize);
std::size_t worstFit(std::span<MemoryManager::Page const, 20> memory, uint8_t const requestedSize);

}  // namespace Allocator

#endif

