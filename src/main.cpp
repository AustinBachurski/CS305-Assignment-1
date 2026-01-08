#include "memoryAllocator.hpp"
#include "job.hpp"

#include <cstdint>
#include <functional>
#include <span>

void demo(std::function<std::size_t(std::span<Job const, 20>, uint8_t const)> allocator);


int main()
{
    //demo(Allocator::firstFit);
    //demo(Allocator::bestFit);
    demo(Allocator::worstFit);
}

