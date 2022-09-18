#include "InstructionThread.hpp"

namespace transmitter {
InstructionThread::InstructionThread(FTDI& ftdi) : m_ftdi(ftdi) {}

InstructionThread::~InstructionThread() { quit(); }

void InstructionThread::queue(Image const& img, double durationSeconds)
{
  m_instructionQueue.lock();
  m_instructionQueue->push_back({img, durationSeconds});
  m_instructionQueue.unlock();
}

void InstructionThread::clearQueue()
{
  m_instructionQueue.lock();
  m_instructionQueue->clear();
  m_instructionQueue.unlock();
}

void InstructionThread::quit() { exit(ThreadStatus::Quitting); }

void InstructionThread::finish() { exit(ThreadStatus::Finishing); }

void InstructionThread::exit(ThreadStatus exitStatus)
{
  m_status.lock();
  if (ThreadStatus::Running == m_status) {
    m_status = exitStatus;
    m_status.unlock();
    m_thread.join();
  } else {
    m_status.unlock();
  }
}

void InstructionThread::run(InstructionThread* thread)
{
  auto& instructionQueue = thread->m_instructionQueue;
  auto& status = thread->m_status;
  auto& ftdi = thread->m_ftdi;

  while (true) {
    status.lock();
    ThreadStatus statusCopy = status;
    status.unlock();

    if (ThreadStatus::Quitting == statusCopy) { return; }

    int sleepMs = 100;
    instructionQueue.lock();
    if (instructionQueue->empty()) {
      if (ThreadStatus::Running != statusCopy) {
        instructionQueue.unlock();
        return;
      }
    } else {
      sleepMs = instructionQueue->front().second * 1000;
      ftdi.write(instructionQueue->front().first);
      instructionQueue->pop_front();
    }
    instructionQueue.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
  }
}
} // namespace transmitter