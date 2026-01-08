#include "memoryAllocator.hpp"
#include "job.hpp"

#include <cstdint>
#include <limits>
#include <new>
#include <ranges>
#include <span>


namespace Allocator
{

std::size_t bestFit(std::span<Job const, 20> memory, uint8_t const requestedSize)
{
    std::size_t currentIndex{};
    uint8_t currentSize{};
    std::size_t smallestIndex{};
    uint8_t smallestSize{ std::numeric_limits<uint8_t>::max() };

    for (auto const [i, job] : std::views::enumerate(memory))
    {
        if (!job.jobID)
        {
            if (!currentSize)
            {
                currentIndex = static_cast<std::size_t>(i);
            }

            ++currentSize;
        }
        else
        {
            if (currentSize && currentSize < smallestSize && currentSize >= requestedSize)
            {
                smallestSize = currentSize;
                smallestIndex = currentIndex;
            }
            currentSize = 0;
        }
    }

    if (currentSize > 0 && currentSize < smallestSize && currentSize >= requestedSize)
    {
        smallestSize = currentSize;
        smallestIndex = currentIndex;
    }

    if (smallestSize <= memory.size() && smallestSize >= requestedSize)
    {
        return smallestIndex;
    }

    throw std::bad_alloc();
}

std::size_t firstFit(std::span<Job const, 20> memory, uint8_t const requestedSize)
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

std::size_t worstFit(std::span<Job const, 20> memory, uint8_t const requestedSize)
{
    std::size_t currentIndex{};
    std::size_t currentSize{};
    std::size_t largestIndex{};
    std::size_t largestSize{};

    for (auto const [i, job] : std::views::enumerate(memory))
    {
        if (!job.jobID)
        {
            if (!currentSize)
            {
                currentIndex = static_cast<std::size_t>(i);
            }
            
            ++ currentSize;
        }
        else
        {
            if (currentSize && currentSize > largestSize && currentSize >= requestedSize)
            {
                largestSize = currentSize;
                largestIndex = currentIndex;
            }
            currentSize = 0;
        }
    }

    if (currentSize && currentSize > largestSize && currentSize >= requestedSize)
    {
        largestSize = currentSize;
        largestIndex = currentIndex;
    }

    if (largestSize >= requestedSize)
    {
        return largestIndex;
    }

    throw std::bad_alloc();
}

}  // namespace Allocator

