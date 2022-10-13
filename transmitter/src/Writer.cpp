#include "Writer.hpp"

namespace transmitter {
Writer::Writer(FTDI& ftdi) :
    m_ftdi(ftdi),
    m_instructionThread(executeInstructions,
                        &ftdi,
                        &m_instructionQueue,
                        &m_state)
{}

Writer::~Writer()
{
  bool exited = false;
  m_state.lock();
  exited = ThreadState::Exited == *m_state;
  m_state.unlock();
  if (!exited) { quit(); }
}

void Writer::write(Instruction const& instruction)
{
  m_instructionQueue.lock();
  m_instructionQueue->clear();
  for (auto const& pair : instruction.writes()) {
    m_instructionQueue->push_back(pair);
  }
  m_instructionQueue.unlock();
}

void Writer::queue(Instruction const& instruction)
{
  m_instructionQueue.lock();
  for (auto const& pair : instruction.writes()) {
    m_instructionQueue->push_back(pair);
  }
  m_instructionQueue.unlock();
}

void Writer::clear() { write(Off()); }

void Writer::finish()
{
  queue(Off());
  exit(ThreadState::Finishing);
}

void Writer::quit()
{
  write(Off());
  exit(ThreadState::Quitting);
}

void Writer::exit(ThreadState exitingState)
{
  if (!(ThreadState::Finishing == exitingState ||
        ThreadState::Quitting == exitingState)) {
    exitingState = ThreadState::Quitting;
  }

  m_state.lock();
  *m_state = exitingState;
  m_state.unlock();
  m_instructionThread.join();
  *m_state = ThreadState::Exited;
}

void Writer::executeInstructions(
  FTDI* ftdiPtr,
  Guarded<std::list<std::pair<Image, double>>>* instructionQueuePtr,
  Guarded<ThreadState>* statePtr)
{
  auto& ftdi = *ftdiPtr;
  auto& instructionQueue = *instructionQueuePtr;
  auto& state = *statePtr;
  std::chrono::system_clock::time_point imageStart =
    std::chrono::system_clock::now();

  while (true) {
    instructionQueue.lock();
    state.lock();

    if (instructionQueue->empty()) {
      if ((ThreadState::Finishing == *state ||
           ThreadState::Quitting == *state)) {
        break;
      }
      std::chrono::system_clock::time_point imageStart =
        std::chrono::system_clock::now();
      instructionQueue.unlock();
      state.unlock();
      continue;
    }

    if (-1 == instructionQueue->front().second &&
        1 == instructionQueue->size()) {
      ftdi.write(instructionQueue->front().first);
    } else if (imageStart + std::chrono::duration<double>(
                              instructionQueue->front().second) >=
               std::chrono::system_clock::now()) {
      instructionQueue->pop_front();
      imageStart = std::chrono::system_clock::now();
    } else {
      ftdi.write(instructionQueue->front().first);
    }

    instructionQueue.unlock();
    state.unlock();
  }
}
} // namespace transmitter