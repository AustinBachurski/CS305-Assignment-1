#include "memoryAllocator.hpp"
#include "job.hpp"

#include <cstdint>
#include <cstring>
#include <functional>
#include <print>
#include <span>


void demo(std::function<std::size_t(std::span<Job const, 20>, uint8_t const)> allocator);


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::println("Usage: assignment1 <--algorithm> (i.e. assignment1 --First-Fit)\nAvailable algorithms:\n\t\tFirst-Fit\n\t\tBest-Fit\n\t\tWorst-Fit");
    }
    else if (std::strcmp(argv[1], "--First-Fit") == 0)
    {
        demo(Allocator::firstFit);
    }
    else if (std::strcmp(argv[1], "--Best-Fit") == 0)
    {
        demo(Allocator::bestFit);
    }
    else if (std::strcmp(argv[1], "--Worst-Fit") == 0)
    {
        demo(Allocator::worstFit);
    }
}

