
#ifndef UTILS_CYCLIC_BARRIER_H
#define UTILS_CYCLIC_BARRIER_H

#include <stddef.h>

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

namespace FOREVER {

/**
 * A cyclic barrier is used to synchronize several threads to a particular
 * execution point.
 *
 * Threads entering the barrier are halted until all threads reach the barrier.
 *
 * @see CountDownLatch
 */
class CyclicBarrier {
 public:
  /**
   * Creates a cyclic barrier with a specified number of threads to synchronize.
   * The minimum useful value is 2. A value of 0 is invalid and is silently
   * changed to 1.
   * @param num_threads Number of threads to synchronize.
   */
  explicit CyclicBarrier(size_t num_threads) noexcept;

  /**
   * @return The number of thread that are synchronized.
   */
  size_t getThreadCount() const noexcept;

  /**
   * @return Number of threads currently waiting on the barrier.
   */
  size_t getWaitingThreadCount() const noexcept;

  /**
   * Blocks until getThreadCount()-1 other threads reach await().
   */
  void await() noexcept;

  /**
   * Resets the cyclic barrier to its original state and releases all waiting
   * threads.
   */
  void reset() noexcept;

  CyclicBarrier() = delete;
  CyclicBarrier(const CyclicBarrier&) = delete;
  CyclicBarrier& operator=(const CyclicBarrier&) = delete;

 private:
  enum class State { TRAP, RELEASE };

  const size_t m_num_threads;
  mutable std::mutex m_lock;
  mutable std::condition_variable m_cv;

  State m_state = State::TRAP;
  size_t m_trapped_threads = 0;
  size_t m_released_threads = 0;
};

}  // namespace FOREVER

#endif  // UTILS_CYCLIC_BARRIER_H
