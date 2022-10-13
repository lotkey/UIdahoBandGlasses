#pragma once

#include "FTDI.hpp"
#include "Guarded.hpp"
#include "Instruction.hpp"

#include <chrono>
#include <list>
#include <thread>

namespace transmitter {
enum class ThreadState { Running, Finishing, Quitting, Exited };

class Writer final {
public:
  Writer(FTDI& ftdi);
  ~Writer();

  void write(Instruction const& instruction);
  void queue(Instruction const& instruction);
  void clear();
  void finish();
  void quit();

private:
  std::thread m_instructionThread;
  Guarded<std::list<std::pair<Image, double>>> m_instructionQueue;
  Guarded<ThreadState> m_state = ThreadState::Running;
  FTDI& m_ftdi;

  static void executeInstructions(
    FTDI* ftdiPtr,
    Guarded<std::list<std::pair<Image, double>>>* instructionQueuePtr,
    Guarded<ThreadState>* statePtr);

  void exit(ThreadState exitingState);
};
} // namespace transmitter