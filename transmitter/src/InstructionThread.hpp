#pragma once

#include "FTDI.hpp"
#include "Guarded.hpp"
#include "common.hpp"

#include <list>
#include <thread>
#include <utility>

namespace transmitter {
enum class ThreadStatus { Running, Quitting, Finishing };

class InstructionThread {
public:
  InstructionThread(InstructionThread const&) = delete;
  InstructionThread(InstructionThread&&) = default;
  InstructionThread& operator=(InstructionThread const&) = delete;
  InstructionThread& operator=(InstructionThread&&) = default;

  InstructionThread(FTDI& ftdi);
  virtual ~InstructionThread();

  void queue(Image const& img, double durationSeconds);
  void clearQueue();
  void quit();
  void finish();

private:
  FTDI& m_ftdi;
  Guarded<std::list<std::pair<Image, double>>> m_instructionQueue;
  Guarded<ThreadStatus> m_status = ThreadStatus::Running;
  std::thread m_thread = std::thread(run, this);

  static void run(InstructionThread* thread);

  void exit(ThreadStatus exitStatus);
};
} // namespace transmitter