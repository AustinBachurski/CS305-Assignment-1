#include "memoryManager.hpp"

#include <array>
#include <print>


int main()
{
    std::array<MemoryManager::Page, 20> memory{};

    for (auto const& page : memory)
    {
         std::println("{}", page);
    }
}

