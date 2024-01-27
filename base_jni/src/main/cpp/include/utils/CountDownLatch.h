
#ifndef UTILS_COUNTDOWNLATCH_H
#define UTILS_COUNTDOWNLATCH_H

#include <stdint.h>
#include <stddef.h>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <condition_variable>

namespace FOREVER {

/**
 * A count down latch is used to block one or several threads until the latch is signaled
 * a certain number of times.
 *
 * Threads entering the latch are blocked until the latch is signaled enough times.
 *
 * @see CyclicBarrier
 */
class CountDownLatch {
public:
    /**
     * Creates a count down latch with a specified count. The minimum useful value is 1.
     * @param count the latch counter initial value
     */
    explicit CountDownLatch(size_t count) noexcept;
    ~CountDownLatch() = default;

    /**
     * Blocks until latch() is called \p count times.
     * @see CountDownLatch(size_t count)
     */
    void await() noexcept;

    /**
     * Releases threads blocked in await() when called \p count times. Calling latch() more than
     * \p count times has no effect.
     * @see reset()
     */
    void latch() noexcept;

    /**
     * Resets the count-down latch to the given value.
     *
     * @param new_count New latch count. A value of zero will immediately unblock all waiting
     * threads.
     *
     * @warning Use with caution. It's only safe to reset the latch count when you're sure
     * that no threads are waiting in await(). This can be guaranteed in various ways, for
     * instance, if you have a single thread calling await(), you could call reset() from that
     * thread, or you could use a CyclicBarrier to make sure all threads using the CountDownLatch
     * are at a known place (i.e.: not in await()) when reset() is called.
     */
    void reset(size_t new_count) noexcept;

    /**
     * @return the number of times latch() has been called since construction or reset.
     * @see reset(),  CountDownLatch(size_t count)
     */
    size_t getCount() const noexcept;

    CountDownLatch() = delete;
    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch& operator=(const CountDownLatch&) = delete;

private:
    uint32_t m_initial_count;
    uint32_t m_remaining_count;
    mutable std::mutex m_lock;
    mutable std::condition_variable m_cv;
};

} // namespace FOREVER

#endif // UTILS_COUNTDOWNLATCH_H
