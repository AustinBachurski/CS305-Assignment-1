#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstdint>
#include <format>


namespace MemoryManager {

enum class JobState : uint8_t
{
    end,
    waiting,
    running,
    sleep,
};

struct Page
{
    std::size_t allocatedIndex{};
    uint8_t jobID{};
    uint8_t startTime{};
    uint8_t executionInterval{};
    JobState state{};
};


} // namespace MM

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

            case MemoryManager::JobState::sleep:
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
        return std::format_to(context.out(), "JobID: {}\nStart Time: {}\nExecutionInterval: {}\nState: {}\nBegin: {}",
              page.jobID, page.startTime, page.executionInterval, page.state, page.allocatedIndex);
    }
};

#endif

