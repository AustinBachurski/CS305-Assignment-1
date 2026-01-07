#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstdint>
#include <format>
#include <functional>
#include <span>


class MemoryManager
{
public:
    enum class JobState : uint8_t
    {
        end,
        waiting,
        running,
        sleeping,
    };

    struct Page
    {
        uint8_t jobID{};
        uint8_t memorySize{};
        uint8_t startTime{};
        uint8_t runTime{};
        JobState currentState{};
        JobState endState{};
    };

    using AllocatorCallable = std::function<std::size_t(std::span<Page const, 20>, uint8_t const)>;

    void allocate(Page const& page, AllocatorCallable allocator);
    void displayMemoryState(uint8_t const currentTime);
    void updateState(uint8_t const currentTime);

private:
    std::array<MemoryManager::Page, 20> memory{};

};

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

