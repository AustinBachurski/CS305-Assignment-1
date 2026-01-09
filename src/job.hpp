#ifndef MEMORY_JOB_HPP
#define MEMORY_JOB_HPP

#include <cstdint>
#include <format>


// Enum for setting the different states of a job.
enum class JobState : uint8_t
{
    end,       // Job has ended and may be freed.
    queued,    // Job is queued for allocation.
    running,   // Job is currently running.
    open,      // File is currently open.
    blocked,   // Job is blocked waiting for memory.
    sleeping,  // Job is sleeping.
};

// Enum class for setting the type of a job.
enum class JobType : uint8_t
{
    job,   // Running application.
    file,  // File from disk.
};

// Object representing the application or file that is occupying memory.
struct Job
{
    uint8_t jobID{};                  // The ID of the job.
    uint8_t memorySize{};             // The number of pages the job occupies.
    uint8_t startTime{};              // Point in time when the job should be allocated.
    uint8_t runTime{};                // Duration of time the job should occupy memory.
    JobState currentState{};          // The current state of the job.
    JobState endState{};              // The state the job will be left in at startTime + runTime.
    JobType jobType{ JobType::job };  // Indicates the type of job.
};

// The std::formatter object is required by the std::print and std::println functions ot the 
// C++ Standard Template Library.  It defines how the object will be printed.
// In this case, it is the JobState object.
template<>
struct std::formatter<JobState>
{
    // `parse` defines behavior for format strings, unnecessary for this object.
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    // `format` defines the output that will be printed by std::print/std::println
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

// The std::formatter object is required by the std::print and std::println functions ot the 
// C++ Standard Template Library.  It defines how the object will be printed.
// In this case, it is the Job object.
template<>
struct std::formatter<Job>
{
    // `parse` defines behavior for format strings, unnecessary for this object.
    constexpr auto parse(std::format_parse_context& context)
    {
        return context.begin();
    }

    // `format` defines the output that will be printed by std::print/std::println
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

