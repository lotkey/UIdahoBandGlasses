#pragma once

/// Shared code between the receiver and transmitter. This code does not depend
/// on the receiver or the transmitter. It can be fully isolated from the two.
namespace common {}

/// Include all other headers in the folder so they can all be easily included with
/// "#include "common/common.hpp""
#include "Color.hpp"
#include "Image.hpp"
#include "Matrix.hpp"