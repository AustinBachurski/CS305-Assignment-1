#include "memoryAllocator.hpp"
#include "job.hpp"

#include <cstdint>
#include <new>
#include <ranges>
#include <span>


namespace Allocator
{

//std::size_t bestFit(std::span<Job const, 20> memory, uint8_t const requestedSize)
//{
//    throw std::bad_alloc();
//}

std::size_t firstFit(std::span<Job const, 20> memory, uint8_t const requestedSize)
{
    std::size_t index{};
    uint8_t spanSize{};

    for (auto const [i, page] : std::views::enumerate(memory))
    {
        if (!page.jobID)    // At a free space.
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

//std::size_t worstFit(std::span<Job const, 20> memory, uint8_t const [[maybe_unused]]requestedSize)
//{
//    throw std::bad_alloc();
//}

}  // namespace Allocator

