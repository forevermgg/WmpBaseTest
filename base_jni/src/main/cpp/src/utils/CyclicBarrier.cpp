
#include <utils/CyclicBarrier.h>

#include <algorithm>

namespace FOREVER {

CyclicBarrier::CyclicBarrier(size_t num_threads) noexcept
    : m_num_threads(std::max(size_t(1), num_threads)) {}

size_t CyclicBarrier::getThreadCount() const noexcept { return m_num_threads; }

size_t CyclicBarrier::getWaitingThreadCount() const noexcept {
  std::lock_guard<std::mutex> guard(m_lock);
  return m_trapped_threads;
}

void CyclicBarrier::reset() noexcept {
  std::lock_guard<std::mutex> guard(m_lock);
  m_state = State::TRAP;
  m_trapped_threads = 0;
  m_released_threads = 0;
  m_cv.notify_all();
}

void CyclicBarrier::await() noexcept {
  std::unique_lock<std::mutex> guard(m_lock);

  // we're releasing old threads, wait until we're done with that
  m_cv.wait(guard, [this] { return m_state != State::RELEASE; });

  // This is the last thread that will be trapped in the barrier
  if (m_trapped_threads == m_num_threads - 1) {
    std::swap(m_released_threads, m_trapped_threads);
    // release currently trapped threads
    m_state = State::RELEASE;
    m_cv.notify_all();

    // wait for all previously trapped threads to be released
    m_cv.wait(guard, [this] { return m_released_threads == 0; });
    m_state = State::TRAP;
    m_cv.notify_all();
  } else {
    ++m_trapped_threads;
    m_cv.wait(guard, [this] { return m_state == State::RELEASE; });
    if (--m_released_threads == 0) {
      // no more threads to be released, we need to notify the last one which is
      // waiting for the m_released_threads queue to become empty and will
      // switch the state back to State::TRAP.
      m_cv.notify_all();
    }
  }
}

}  // namespace FOREVER
