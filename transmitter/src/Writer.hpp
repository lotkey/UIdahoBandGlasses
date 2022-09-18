#pragma once

#include "FTDI.hpp"
#include "Instruction.hpp"
#include "InstructionThread.hpp"

#include <thread>

namespace transmitter {
class Writer {
public:
  Writer(FTDI& ftdi);

  void write(Instruction const& instruction);
  void queue(Instruction const& instruction);
  void clearQueue();
  void finish();
  void quit();

private:
  FTDI& m_ftdi;
  std::unique_ptr<InstructionThread> m_currentThread;
  Guarded<std::list<std::unique_ptr<InstructionThread>>> m_oldThreads;
  Guarded<ThreadStatus> m_status = ThreadStatus::Running;
  std::thread m_cleanupThread = std::thread(cleanUpAfter, this);

  static void cleanUpAfter(Writer* writer);

  void exit(ThreadStatus exitStatus);
};
} // namespace transmitter