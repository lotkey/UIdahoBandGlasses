// Chris McVickar
#include "BMP.hpp"
#include "Image.hpp"
#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace transmitter {
BMP::BMP(std::filesystem::path const& img_path) { load(img_path); }

void BMP::load(std::filesystem::path const& img_path)
{
  std::ifstream infile(img_path, std::ios_base::binary);
  if (infile) {
    infile.read((char*)&m_fileHeader, sizeof(m_fileHeader));
    if (m_fileHeader.file_type != 0x4D42) {
      throw std::runtime_error("Error! Unrecognized file format.");
    }
    infile.read((char*)&m_infoHeader, sizeof(m_infoHeader));

    // The ColorHeader is used only for transparent images
    if (32 == m_infoHeader.bit_count) {
      // Check if the file has bit mask color information
      if (m_infoHeader.size >= (sizeof(InfoHeader) + sizeof(ColorHeader))) {
        infile.read((char*)&m_colorHeader, sizeof(m_colorHeader));
        // Check if the pixel data is stored as BGRA and if the
        // color space type is sRGB
        checkColorHeader(m_colorHeader);
      } else {
        std::cerr << "Error! The file \"" << img_path
                  << "\" does not seem to contain bit mask information\n";
        throw std::runtime_error("Error! Unrecognized file format.");
      }
    }

    // Jump to the pixel data location
    infile.seekg(m_fileHeader.offset_data, infile.beg);

    // Adjust the header fields for output.
    // Some editors will put extra info in the image file, we only save
    // the headers and the data.
    if (32 == m_infoHeader.bit_count) {
      m_infoHeader.size = sizeof(InfoHeader) + sizeof(ColorHeader);
      m_fileHeader.offset_data =
        sizeof(FileHeader) + sizeof(InfoHeader) + sizeof(ColorHeader);
    } else {
      m_infoHeader.size = sizeof(InfoHeader);
      m_fileHeader.offset_data = sizeof(FileHeader) + sizeof(InfoHeader);
    }
    m_fileHeader.file_size = m_fileHeader.offset_data;

    if (m_infoHeader.height < 0) {
      throw std::runtime_error(
        "The program can treat only BMP images with the origin in "
        "the bottom left corner!");
    }

    m_data.resize(m_infoHeader.width * m_infoHeader.height *
                  m_infoHeader.bit_count / 8);

    // Here we check if we need to take into account row padding
    if (0 == m_infoHeader.width % 4) {
      infile.read((char*)m_data.data(), m_data.size());
      m_fileHeader.file_size += static_cast<uint32_t>(m_data.size());
    } else {
      m_rowStride = m_infoHeader.width * m_infoHeader.bit_count / 8;
      uint32_t new_stride = makeStrideAligned(4);
      std::vector<uint8_t> padding_row(new_stride - m_rowStride);

      for (int y = 0; y < m_infoHeader.height; ++y) {
        infile.read((char*)(m_data.data() + m_rowStride * y), m_rowStride);
        infile.read((char*)padding_row.data(), padding_row.size());
      }
      m_fileHeader.file_size +=
        static_cast<uint32_t>(m_data.size()) +
        m_infoHeader.height * static_cast<uint32_t>(padding_row.size());
    }
  } else {
    throw std::runtime_error("Unable to open the input image file.");
  }
}

BMP::BMP(Image const& img)
{
  m_infoHeader.width = img.numCols();
  m_infoHeader.height = img.numRows();
  m_infoHeader.size = sizeof(InfoHeader);
  m_fileHeader.offset_data = sizeof(FileHeader) + sizeof(InfoHeader);

  m_infoHeader.bit_count = 24;
  m_infoHeader.compression = 0;
  m_rowStride = m_infoHeader.width * 3;
  m_data.resize(m_rowStride * m_infoHeader.height);

  uint32_t new_stride = makeStrideAligned(4);
  m_fileHeader.file_size = m_fileHeader.offset_data +
                           static_cast<uint32_t>(m_data.size()) +
                           m_infoHeader.height * (new_stride - m_rowStride);

  for (int i = 0; i < img.numRows(); i++) {
    for (int j = 0; j < img.numCols(); j++) { set(i, j, img.at(i, j)); }
  }
}

Image BMP::toImage() const
{
  Image img(m_infoHeader.height, m_infoHeader.width);

  for (int i = 0; i < img.numRows(); i++) {
    for (int j = 0; j < img.numCols(); j++) { img.at(i, j) = at(i, j); }
  }
  return img;
}

void BMP::save(std::filesystem::path const& img_path) const
{
  std::ofstream outfile(img_path, std::ios_base::binary);
  if (outfile) {
    if (32 == m_infoHeader.bit_count) {
      writeHeaders(outfile);
      writeData(outfile);
    } else if (24 == m_infoHeader.bit_count) {
      if (0 == m_infoHeader.width % 4) {
        writeHeaders(outfile);
        writeData(outfile);
      } else {
        uint32_t new_stride = makeStrideAligned(4);
        std::vector<uint8_t> padding_row(new_stride - m_rowStride);

        writeHeaders(outfile);

        for (int y = 0; y < m_infoHeader.height; ++y) {
          outfile.write((char const*)(m_data.data() + m_rowStride * y),
                        m_rowStride);
          outfile.write((char const*)padding_row.data(), padding_row.size());
        }
      }
    } else {
      throw std::runtime_error("The program can treat only 24 or 32 "
                               "bits per pixel BMP files");
    }
  } else {
    throw std::runtime_error("Unable to open the output image file.");
  }
}

void BMP::set(int row, int col, common::Color const& color)
{
  row = (m_infoHeader.height - 1) - row;

  uint32_t channels = m_infoHeader.bit_count / 8;
  int baseIndex = channels * (row * m_infoHeader.width + col);
  m_data[baseIndex + 2] = color.getR();
  m_data[baseIndex + 1] = color.getG();
  m_data[baseIndex + 0] = color.getB();
}

common::Color BMP::at(int row, int col) const
{
  row = (m_infoHeader.height - 1) - row;

  uint32_t channels = m_infoHeader.bit_count / 8;
  int baseIndex = channels * (row * m_infoHeader.width + col);
  int r = m_data[baseIndex + 2];
  int g = m_data[baseIndex + 1];
  int b = m_data[baseIndex + 0];
  return {r, g, b};
}

uint8_t BMP::at(int row, int col, int channel) const
{
  row = (m_infoHeader.height - 1) - row;

  uint32_t channels = m_infoHeader.bit_count / 8;
  return m_data[channels * (row * m_infoHeader.width + col) + channel];
}

uint8_t& BMP::at(int row, int col, int channel)
{
  row = (m_infoHeader.height - 1) - row;

  uint32_t channels = m_infoHeader.bit_count / 8;
  return m_data[channels * (row * m_infoHeader.width + col) + channel];
}

void BMP::writeHeaders(std::ofstream& outfile) const
{
  outfile.write((char const*)&m_fileHeader, sizeof(m_fileHeader));
  outfile.write((char const*)&m_infoHeader, sizeof(m_infoHeader));
  if (32 == m_infoHeader.bit_count) {
    outfile.write((char const*)&m_colorHeader, sizeof(m_colorHeader));
  }
}

void BMP::writeData(std::ofstream& outfile) const
{
  outfile.write((char const*)m_data.data(), m_data.size());
}

// Add 1 to the m_rowStride until it is divisible with align_stride
uint32_t BMP::makeStrideAligned(uint32_t align_stride) const
{
  uint32_t new_stride = m_rowStride;
  while (new_stride % align_stride != 0) { new_stride++; }
  return new_stride;
}

// Check if the pixel data is stored as BGRA and if the color space type is
// sRGB
void BMP::checkColorHeader(ColorHeader& m_colorHeader) const
{
  ColorHeader expected_color_header;
  if (expected_color_header.red_mask != m_colorHeader.red_mask ||
      expected_color_header.blue_mask != m_colorHeader.blue_mask ||
      expected_color_header.green_mask != m_colorHeader.green_mask ||
      expected_color_header.alpha_mask != m_colorHeader.alpha_mask) {
    throw std::runtime_error(
      "Unexpected color mask format! The program expects the pixel "
      "data to be in the BGRA format");
  }
  if (expected_color_header.color_space_type !=
      m_colorHeader.color_space_type) {
    throw std::runtime_error(
      "Unexpected color space type! The program expects sRGB values");
  }
}
} // namespace transmitter