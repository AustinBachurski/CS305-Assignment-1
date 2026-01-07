#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstdint>
#include <format>
#include <span>


namespace MemoryManager
{

enum class JobState : uint8_t
{
    end,
    waiting,
    running,
    sleeping,
};

struct Page
{
    std::size_t allocatedIndex{};
    uint8_t jobID{};
    uint8_t startTime{};
    uint8_t runTime{};
    JobState currentState{};
    JobState endState{};
};

void allocate(std::span<Page, 20> memory, std::size_t const index, Page const& page);
void displayMemoryState(std::span<Page const, 20> memory, uint8_t const currentTime);
void updateState(std::span<Page, 20> memory, uint8_t const currentTime);

} // namespace MemoryManager

template<>
struct std::formatter<MemoryManager::JobState>
{
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    auto format(MemoryManager::JobState const& state, std::format_context& context) const
    {
        switch (state)
        {
            case MemoryManager::JobState::end:
                return std::format_to(context.out(), "End");

            case MemoryManager::JobState::waiting:
                return std::format_to(context.out(), "Waiting");

            case MemoryManager::JobState::running:
                return std::format_to(context.out(), "Running");

            case MemoryManager::JobState::sleeping:
                return std::format_to(context.out(), "Sleeping");
        }
        return std::format_to(context.out(), "Undefined case in formatter!");
    }
};

template<>
struct std::formatter<MemoryManager::Page>
{
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    auto format(MemoryManager::Page const& page, std::format_context& context) const
    {
        return std::format_to(context.out(), "Job {} - {}", page.jobID, page.currentState);
    }
};

#endif

