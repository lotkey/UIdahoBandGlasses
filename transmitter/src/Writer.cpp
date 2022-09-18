#include "Writer.hpp"

namespace transmitter {
Writer::Writer(FTDI& ftdi) : m_ftdi(ftdi) {}

void Writer::write(Instruction const& instruction)
{
  clearQueue();
  for (auto const& [img, length] : instruction.writes()) {
    m_currentThread->queue(img, length);
  }
}

void Writer::queue(Instruction const& instruction)
{
  for (auto const& [img, length] : instruction.writes()) {
    m_currentThread->queue(img, length);
  }
}

void Writer::clearQueue()
{
  m_currentThread->clearQueue();
  m_oldThreads.lock();
  m_oldThreads->emplace_back(std::move(m_currentThread));
  m_oldThreads.unlock();
  m_currentThread = std::make_unique<InstructionThread>();
}

void Writer::finish() { exit(ThreadStatus::Finishing); }

void Writer::quit()
{
  clearQueue();
  exit(ThreadStatus::Quitting);
}

void Writer::exit(ThreadStatus exitStatus)
{
  m_status.lock();
  if (ThreadStatus::Running == m_status) {
    m_status = exitStatus;
    m_status.unlock();
    m_cleanupThread.join();
  } else {
    m_status.unlock();
  }
}

void Writer::cleanUpAfter(Writer* writer)
{
  auto& oldThreads = writer->m_oldThreads;
  auto& status = writer->m_status;
  auto& currentThread = writer->m_currentThread;

  while (true) {
    status.lock();
    ThreadStatus statusCopy = status;
    status.unlock();

    if (ThreadStatus::Running != statusCopy) { break; }

    oldThreads.lock();
    if (!oldThreads->empty()) {
      oldThreads->front()->quit();
      oldThreads->pop_front();
      oldThreads.unlock();
      continue;
    }
    oldThreads.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  oldThreads.lock();
  status.lock();
  bool quitting = ThreadStatus::Quitting == status;
  status.unlock();

  if (quitting) {
    for (auto& thread : oldThreads.get()) { thread->quit(); }
    currentThread->quit();
  } else {
    for (auto& thread : oldThreads.get()) { thread->finish(); }
    currentThread->finish();
  }
  oldThreads->clear();
  oldThreads.unlock();
}
} // namespace transmitter