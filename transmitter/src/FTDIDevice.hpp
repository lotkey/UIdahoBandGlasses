#pragma once

#include <ftdi.h>

#include <string>
#include <vector>

namespace transmitter {
// Forward declarations
class FTDI;

/// @brief Class to represent an FTDI USB device
class FTDIDevice {
public:
  virtual ~FTDIDevice() = default;

  /// @return Descriptor of the USB device
  usb_device_descriptor const& descriptor() const;
  /// @return The number associated with the USB device
  std::uint8_t deviceNumber() const;
  /// @return The number of children of the USB device
  std::uint8_t numChildren() const;
  /// @return The children of the USB device
  std::vector<FTDIDevice> const& children() const;
  /// @return Manufacturer of the USB device
  std::string const& manufacturer() const;
  /// @return Description of the USB device
  std::string const& description() const;
  /// @return Serial string retrieved from ftdi_usb_get_strings
  std::string const& serial() const;

private:
  struct usb_device* m_self = nullptr;
  std::string m_manufacturer;
  std::string m_description;
  std::string m_serial;
  std::vector<FTDIDevice> m_children;

  // FTDIDevices should not be constructable by any user. They should only be
  // constructable by the FTDI class.
  FTDIDevice(struct usb_device* self);
  FTDIDevice(struct usb_device* self,
             std::string const& manufacturer,
             std::string const& description,
             std::string const& serial);

  friend class FTDI; // Give the FTDI access to the private constructors
};
} // namespace transmitter