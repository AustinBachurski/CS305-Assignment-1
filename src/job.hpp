#ifndef MEMORY_JOB_HPP
#define MEMORY_JOB_HPP

#include <cstdint>
#include <format>


enum class JobState : uint8_t
{
    end,
    queued,
    running,
    open,
    blocked,
    sleeping,
};

enum class JobType : uint8_t
{
    job,
    file,
};

struct Job
{
    uint8_t jobID{};
    uint8_t memorySize{};
    uint8_t startTime{};
    uint8_t runTime{};
    JobState currentState{};
    JobState endState{};
    JobType jobType{ JobType::job };
};

template<>
struct std::formatter<JobState>
{
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    auto format(JobState const& state, std::format_context& context) const
    {
        switch (state)
        {
            case JobState::end:
                return std::format_to(context.out(), "End");

            case JobState::queued:
                return std::format_to(context.out(), "Queued");

            case JobState::running:
            {
                return std::format_to(context.out(), "Running");
            }

            case JobState::open:
            {
                return std::format_to(context.out(), "Open");
            }

            case JobState::blocked:
                return std::format_to(context.out(), "Blocked");

            case JobState::sleeping:
                return std::format_to(context.out(), "Sleeping");
        }
        return std::format_to(context.out(), "Undefined case in formatter!");
    }
};

template<>
struct std::formatter<Job>
{
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    auto format(Job const& job, std::format_context& context) const
    {
        if (job.jobType == JobType::job)
        {
            return std::format_to(context.out(), "Job {} - {}", job.jobID, job.currentState);
        }
        else
        {
            return std::format_to(context.out(), "File {} - {}", job.jobID, job.currentState);
        }
    }
};

#endif

