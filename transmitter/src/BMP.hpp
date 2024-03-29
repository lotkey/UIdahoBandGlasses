/// @author @lotkey Chris McVickar
#pragma once
#include "Image.hpp"
#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace transmitter {
class BMP {
public:
  BMP(BMP const&) = default;
  BMP(BMP&&) = default;
  BMP& operator=(BMP const&) = default;
  BMP& operator=(BMP&&) = default;
  virtual ~BMP() = default;

  // Construct a BMP from the provided path to a .bmp file
  BMP(std::filesystem::path const& img_path);
  // Construct a BMP from an Image
  BMP(Image const& img);
  // Convert to an Image
  Image toImage() const;
  // Save to the provided .bmp path
  void save(std::filesystem::path const& img_path) const;

  // Set the pixel at row, col to the provided color
  void set(int row, int col, common::Color const&);
  // Returns the color at row, col
  common::Color at(int row, int col) const;
  // Returns the color at row, col, channel
  std::uint8_t at(int row, int col, int channel) const;
  // Returns a reference to the color at row, col, channel
  std::uint8_t& at(int row, int col, int channel);

private:
#pragma pack(push, 1)
  struct FileHeader {
    std::uint16_t file_type =
      0x4D42;                      // File type always BM which is 0x4D42
                                   // (stored as hex uint16_t in little endian)
    std::uint32_t file_size = 0;   // Size of the file (in bytes)
    std::uint16_t reserved1 = 0;   // Reserved, always 0
    std::uint16_t reserved2 = 0;   // Reserved, always 0
    std::uint32_t offset_data = 0; // Start position of pixel data (bytes from
                                   // the beginning of the file)
  };

  struct InfoHeader {
    std::uint32_t size = 0;  // Size of this header (in bytes)
    std::int32_t width = 0;  // width of bitmap in pixels
    std::int32_t height = 0; // width of bitmap in pixels
                             //       (if positive, bottom-up, with origin in
                             //       lower left corner) (if negative, top-down,
                             //       with origin in upper left corner)
    std::uint16_t planes =
      1; // No. of planes for the target device, this is always 1
    std::uint16_t bit_count = 0;   // No. of bits per pixel
    std::uint32_t compression = 0; // 0 or 3 - uncompressed. THIS PROGRAM
                                   // CONSIDERS ONLY UNCOMPRESSED BMP images
    std::uint32_t size_image = 0;  // 0 - for uncompressed images
    std::int32_t x_pixels_per_meter = 0;
    std::int32_t y_pixels_per_meter = 0;
    std::uint32_t colors_used =
      0; // No. color indexes in the color table. Use 0 for
         // the max number of colors allowed by bit_count
    std::uint32_t colors_important = 0; // No. of colors used for displaying the
                                        // bitmap. If 0 all colors are required
  };

  struct ColorHeader {
    std::uint32_t red_mask = 0x00ff0000;   // Bit mask for the red channel
    std::uint32_t green_mask = 0x0000ff00; // Bit mask for the green channel
    std::uint32_t blue_mask = 0x000000ff;  // Bit mask for the blue channel
    std::uint32_t alpha_mask = 0xff000000; // Bit mask for the alpha channel
    std::uint32_t color_space_type = 0x73524742; // Default "sRGB" (0x73524742)
    std::uint32_t unused[16] =
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Unused data for
                                                        // sRGB color space
  };
#pragma pack(pop)

  FileHeader m_fileHeader;
  InfoHeader m_infoHeader;
  ColorHeader m_colorHeader;
  std::uint32_t m_rowStride = 0;
  std::vector<std::uint8_t> m_data;

  void load(std::filesystem::path const&);
  void writeHeaders(std::ofstream& outfile) const;
  void writeData(std::ofstream& outfile) const;
  std::uint32_t makeStrideAligned(std::uint32_t align_stride) const;
  void checkColorHeader(ColorHeader& bmp_color_header) const;
};
} // namespace transmitter