/// @author @lotkey Chris McVickar
#pragma once

#include "FTDIDevice.hpp"
#include "Image.hpp"

#include <ftdi.h>

#define NUM_PACKETS 3


namespace transmitter {
/// @brief Class to represent and manage an FTDI connection.
/// @throws Throws on any FTDI error
class FTDI {
public:
  FTDI(FTDI const&) = delete;
  FTDI(FTDI&&) = default;
  FTDI& operator=(FTDI const&) = delete;
  FTDI& operator=(FTDI&&) = default;

  /// @brief Sets up the FTDI connection
  FTDI(int vendor, int product, int baudrate);
  /// @brief Calls close()
  virtual ~FTDI();

  /// @return Number of FTDI devices found on the machine
  int numDevices() const;
  /// @return List of FTDI devices found on the machine
  std::vector<FTDIDevice> const& devices() const;
  /// @return Getter for vendor passed into constructor
  int vendor() const;
  /// @return Getter for product passed into constructor
  int product() const;
  /// @return Baudrate of the FTDI connection
  int baudrate() const;

  /// @brief Transmits an image
  /// @param packetNum the packet number to be transmitted
  /// @param img Image to transmit
  /// @return The number of bytes transmitted
  int write(uint8_t packetNum, Image const& img)
  /// @brief Closes the FTDI connection and frees any associated memory.
  /// @return Return value of ftdi_usb_close
  int close();

private:
  ftdi_context* m_self = nullptr;
  std::vector<FTDIDevice> m_devices;
  ftdi_device_list* m_deviceList = nullptr;
  int m_numDevices = 0;
  int m_vendor = 0;
  int m_product = 0;

  int openDevice(int index);
  int setBaudrate(int baudrate);
  int setLineProperty(ftdi_bits_type bits,
                      ftdi_stopbits_type sbit,
                      ftdi_parity_type parity);
  void freeMemory();
  /// @brief Throws an error if errorCode is < 0
  /// @param errorCode Error code to check for errors
  /// @param errorMessage Message to print in case of an error
  /// @param successMessage Message to print in case of a success
  /// @return errorCode
  int handle(int errorCode,
             std::string const& errorMessage = {},
             std::string const& successMessage = {});
  /// @return Error string given the ftdi_context
  std::string errorString();
};
} // namespace transmitter