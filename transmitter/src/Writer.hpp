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

  /// Write an instruction to the transmitter.
  /// This will clear any instructions in the queue and write to the transmitter
  /// immediately.
  void write(Instruction const& instruction);
  /// Queue an instruction to be written to the transmitter
  void queue(Instruction const& instruction);
  /// Clear the queue of all instructions
  void clear();
  /// Finish sending everything in the queue and then exit.
  void finish();
  /// Clear the queue, sent an Off instruction, and then exit.
  void quit();

private:
  // Reference to FTDI device to write to
  FTDI& m_ftdi;
  // Thread that writes to the FTDI device
  std::thread m_instructionThread;

  ///////////////////////////////////////////////////////
  // Members that are shared with the instruction thread
  // The Writer will queue instructions and the thread will write them.
  //////////////////////////////////////////////////////
  // Using a linked list for faster pushes/pops
  // It's never indexed
  Guarded<std::list<std::pair<Image, double>>> m_instructionQueue;
  // State of the instruction thread. Used to tell the thread to exit.
  Guarded<ThreadState> m_state = ThreadState::Running;

  /// The function that the instruction thread will execute.
  /// @param ftdiPtr Pointer to the FTDI to write with
  /// @param instructionQueuePtr Pointer to the instruction queue to read
  /// instructions from
  /// @param statePtr Pointer to the state of the thread
  static void executeInstructions(
    FTDI* ftdiPtr,
    Guarded<std::list<std::pair<Image, double>>>* instructionQueuePtr,
    Guarded<ThreadState>* statePtr);

  void exit(ThreadState exitingState);
};
} // namespace transmitter