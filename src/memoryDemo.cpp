#include "memoryManager.hpp"
#include "job.hpp"

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>


namespace
{

[[noreturn]]
void invalidInput(std::string_view const line)
{
    std::println("Invalid input encountered!");
    std::println("Found: {}", line);
    std::exit(1);
}

uint8_t parseField(std::string_view const field, std::string_view const line)
{
    uint8_t value{};

    if (!(std::from_chars(field.data(), field.data() + field.size(), value).ec == std::errc{}))
    {
        invalidInput(line);
    }

    return value;
}

Job makeJob(std::string_view const line)
{
    auto fields = line | std::views::split(',')
                       | std::views::transform([](auto&& split_view)
                             { return std::string_view{ split_view }; });

    Job job;

    auto fieldsIter{ fields.begin() };

    auto next{ [&fieldsIter, &fields, line]()
    {
        if (fieldsIter == fields.end()) { invalidInput(line); }
        return *fieldsIter++;
    }};

    job.jobID = parseField(next(), line);
    job.startTime = parseField(next(), line);
    job.memorySize = parseField(next(), line);
    job.runTime = parseField(next(), line);

    auto lastField{ next() };

    if (lastField == "End")
    {
        job.endState = JobState::end;
    }
    else if (lastField == "Sleep")
    {
        job.endState = JobState::sleeping;
    }
    else
    {
        invalidInput(line);
    }

    job.currentState = JobState::queued;
    return job;
}

std::deque<Job> getJobsFromFile(std::filesystem::path filePath)
{
    std::deque<Job> jobs;

    if (!std::filesystem::exists(filePath))
    {
        std::println("Input file not found: {}!", filePath.string());
        std::exit(1);
    }

    std::ifstream file{ filePath };

    if (!file.is_open())
    {
        std::println("Failed to open input file: {}!", filePath.string());
        std::exit(1);
    }

    std::string line;

    std::getline(file, line);  // Skip csv header.

    while (std::getline(file, line))
    {
        jobs.push_back(makeJob(line));
    }

    return jobs;
}

Job makeFile(std::string_view const line)
{
    auto fields = line | std::views::split(',')
                       | std::views::transform([](auto&& split_view)
                             { return std::string_view{ split_view }; });

    Job job;

    auto fieldsIter{ fields.begin() };

    auto next{ [&fieldsIter, &fields, line]()
    {
        if (fieldsIter == fields.end()) { invalidInput(line); }
        return *fieldsIter++;
    }};

    job.jobID = parseField(next(), line);
    job.startTime = parseField(next(), line);
    job.runTime = parseField(next(), line);

    // Calculate file size; since the instructions define the size to be equal
    // to 1, just sum the number of blocks.
    while (fieldsIter++ != fields.end())
    {
        ++job.memorySize;
    }

    job.jobType = JobType::file;
    job.currentState = JobState::queued;
    return job;
}

void getFilesFromFile(std::deque<Job>& jobs, std::filesystem::path filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        std::println("Input file not found: {}!", filePath.string());
        std::exit(1);
    }

    std::ifstream file{ filePath };

    if (!file.is_open())
    {
        std::println("Failed to open input file: {}!", filePath.string());
        std::exit(1);
    }

    std::string line;

    std::getline(file, line);  // Skip csv header.

    while (std::getline(file, line))
    {
        jobs.push_back(makeFile(line));
    }
}

}  // Anonymous Namespace

void demo()
{
    MemoryManager memory;

    std::deque<Job> jobs = getJobsFromFile("data/inputJobs.csv");
    getFilesFromFile(jobs, "data/inputFiles.csv");

    uint8_t currentTime = 0;

    while(true)
    {
        memory.updateState(currentTime);
        memory.displayMemoryState(currentTime);
        ++currentTime;
        memory.displayStaged(currentTime, jobs);
        memory.performStagedActions(currentTime, jobs);

        if (jobs.empty() && memory.allSleeping())
        {
            memory.displayMemoryState(currentTime);
            memory.displayStaged(currentTime, jobs);
            std::println("Nothing more to do...\n");
            return;
        }
    }
}

