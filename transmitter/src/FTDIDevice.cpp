#include "FTDIDevice.hpp"

namespace transmitter {
FTDIDevice::FTDIDevice(struct usb_device* self) : m_self(self)
{
  for (int i = 0; i < m_self->num_children; i++) {
    m_children.push_back(FTDIDevice(m_self->children[i]));
  }
}

FTDIDevice::FTDIDevice(struct usb_device* self,
                       std::string const& manufacturer,
                       std::string const& description,
                       std::string const& serial) :
    m_self(self),
    m_manufacturer(manufacturer),
    m_description(description),
    m_serial(serial)
{
  for (int i = 0; i < m_self->num_children; i++) {
    m_children.push_back(FTDIDevice(m_self->children[i]));
  }
}

usb_device_descriptor const& FTDIDevice::descriptor() const
{
  return m_self->descriptor;
}

std::uint8_t FTDIDevice::deviceNumber() const { return m_self->devnum; }

std::uint8_t FTDIDevice::numChildren() const { return m_self->num_children; }

std::vector<FTDIDevice> const& FTDIDevice::children() const
{
  return m_children;
}

std::string const& FTDIDevice::manufacturer() const { return m_manufacturer; }

std::string const& FTDIDevice::description() const { return m_description; }

std::string const& FTDIDevice::serial() const { return m_serial; }
} // namespace transmitter