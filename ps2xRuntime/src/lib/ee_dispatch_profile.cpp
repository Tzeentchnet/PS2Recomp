#include "runtime/ee_dispatch_profile.h"

#if defined(PS2X_ENABLE_EE_DISPATCH_PROFILE)
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <time.h>
#endif

namespace ps2x::ee_profile::detail
{
bool readSampleCpu(uint64_t &value) noexcept
{
#if defined(_WIN32)
    ULONG64 cycles = 0u;
    if (!QueryThreadCycleTime(GetCurrentThread(), &cycles))
        return false;
    value = static_cast<uint64_t>(cycles);
    return true;
#elif defined(CLOCK_THREAD_CPUTIME_ID)
    timespec now{};
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now) != 0)
        return false;
    value = uint64_t(now.tv_sec) * 1000000000ull + uint64_t(now.tv_nsec);
    return true;
#else
    (void)value;
    return false;
#endif
}

bool readThreadCpuNs(uint64_t &value) noexcept
{
#if defined(_WIN32)
    FILETIME created{}, exited{}, kernel{}, user{};
    if (!GetThreadTimes(GetCurrentThread(), &created, &exited, &kernel, &user))
        return false;
    ULARGE_INTEGER k{}, u{};
    k.LowPart = kernel.dwLowDateTime;
    k.HighPart = kernel.dwHighDateTime;
    u.LowPart = user.dwLowDateTime;
    u.HighPart = user.dwHighDateTime;
    value = (k.QuadPart + u.QuadPart) * 100u;
    return true;
#else
    return readSampleCpu(value);
#endif
}
} // namespace ps2x::ee_profile::detail
#endif
