#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "job.hpp"

#include <span>


namespace Allocator
{

/// Best-Fit memory allocation algorithm.
std::size_t bestFit(std::span<Job const, 20> memory, uint8_t const requestedSize);

/// First-Fit memory allocation algorithm.
std::size_t firstFit(std::span<Job const, 20> memory, uint8_t const requestedSize);

/// Worst-Fit memory allocation algorithm.
std::size_t worstFit(std::span<Job const, 20> memory, uint8_t const requestedSize);

}  // namespace Allocator

#endif

