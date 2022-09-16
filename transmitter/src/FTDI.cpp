#include "FTDI.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace transmitter {
FTDI::FTDI(int vendor, int product, int baudrate) :
    m_self(ftdi_new()), m_vendor(vendor), m_product(product)
{
  if (!m_self) { throw std::runtime_error("ftdi_new failed."); }
  std::cerr << "Created new FTDI connection." << std::endl;

  m_numDevices =
    handle(ftdi_usb_find_all(m_self, &m_deviceList, m_vendor, m_product),
           "No FTDI devices found.");
  std::cerr << m_numDevices << " FTDI devices found." << std::endl;

  static constexpr int strlen = 512;
  char manufacturer[strlen];
  char description[strlen];
  char serial[strlen];

  auto it = m_deviceList;
  for (int i = 0; it != nullptr; it = it->next, i++) {
    std::cerr << "Checking device: " << i << std::endl;

    handle(ftdi_usb_get_strings(m_self,
                                it->dev,
                                manufacturer,
                                strlen,
                                description,
                                strlen,
                                serial,
                                strlen),
           "ftdi_usb_get_strings failed.");
    m_devices.push_back(FTDIDevice(it->dev, manufacturer, description, serial));
    std::cerr << "Manufacturer: " << m_devices.back().manufacturer()
              << ", Description: " << m_devices.back().description()
              << std::endl;
  }
  handle(openDevice(0), "Unable to open FTDI.", "ftdi_open successful.");
  handle(setBaudrate(baudrate),
         "Unable to set baudrate.",
         "Baudrate set to " + std::to_string(baudrate) + ".");
  handle(setLineProperty(static_cast<ftdi_bits_type>(8), STOP_BIT_1, NONE),
         "Unable to set line parameters.",
         "Line parameters set.");
}

FTDI::~FTDI() { close(); }

int FTDI::numDevices() const { return m_devices.size(); }

std::vector<FTDIDevice> const& FTDI::devices() const { return m_devices; }

int FTDI::vendor() const { return m_vendor; }

int FTDI::product() const { return m_product; }

int FTDI::baudrate() const { return m_self->baudrate; }

int FTDI::write(Image const& img)
{
  auto encoded = img.encode();
  return handle(ftdi_write_data(m_self, encoded.data(), encoded.size()));
}

int FTDI::close()
{
  int i = handle(ftdi_usb_close(m_self));
  freeMemory();
  m_numDevices = 0;
  return i;
}

int FTDI::openDevice(int index)
{
  return handle(ftdi_usb_open_dev(m_self, m_deviceList[index].dev));
}

int FTDI::setBaudrate(int baudrate)
{
  return handle(ftdi_set_baudrate(m_self, baudrate));
}

int FTDI::setLineProperty(ftdi_bits_type bits,
                          ftdi_stopbits_type sbit,
                          ftdi_parity_type parity)
{
  return handle(ftdi_set_line_property(m_self, bits, sbit, parity));
}

void FTDI::freeMemory()
{
  if (m_self) {
    ftdi_free(m_self);
    m_self = nullptr;
  }

  if (m_deviceList) {
    ftdi_list_free(&m_deviceList);
    m_deviceList = nullptr;
  }
}

int FTDI::handle(int errorCode,
                 std::string const& errorMessage,
                 std::string const& successMessage)
{
  if (errorCode < 0) {
    freeMemory();
    if (!errorMessage.empty()) { std::cerr << errorMessage << std::endl; }
    auto str = errorString();
    if (str.empty()) {
      throw std::runtime_error("FTDI error: " + std::to_string(errorCode));
    } else {
      throw std::runtime_error(std::string("FTDI error: \"") + str + "\" (" +
                               std::to_string(errorCode) + ")");
    }
  }
  if (!successMessage.empty()) { std::cerr << successMessage << std::endl; }
  return errorCode;
}

std::string FTDI::errorString()
{
  return std::string(ftdi_get_error_string(m_self));
}
} // namespace transmitter