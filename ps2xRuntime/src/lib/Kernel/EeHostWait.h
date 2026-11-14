#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>

enum class EeHostWaitResult : uint8_t
{
    Deadline,
    Notified,
    Failed,
};

struct EeHostWaitOutcome
{
    EeHostWaitResult result = EeHostWaitResult::Failed;
    bool precise = false;
};

class EeHostWait
{
public:
    explicit EeHostWait(bool preciseRequested);
    ~EeHostWait();

    EeHostWait(const EeHostWait &) = delete;
    EeHostWait &operator=(const EeHostWait &) = delete;

    [[nodiscard]] bool precise() const noexcept;
    void notify() noexcept;
    EeHostWaitOutcome wait(std::unique_lock<std::mutex> &lock,
                           std::condition_variable &fallback,
                           std::optional<std::chrono::steady_clock::time_point> deadline,
                           const std::atomic<uint64_t> &notificationGeneration,
                           uint64_t expectedGeneration);

private:
    EeHostWaitOutcome waitFallback(std::unique_lock<std::mutex> &lock,
                                   std::condition_variable &fallback,
                                   std::optional<std::chrono::steady_clock::time_point> deadline,
                                   const std::atomic<uint64_t> &notificationGeneration,
                                   uint64_t expectedGeneration);
    void failPreciseWait(const char *operation, unsigned long error) noexcept;

    void *m_wakeEvent = nullptr;
    void *m_timer = nullptr;
    std::atomic<bool> m_preciseFailed{false};
};
