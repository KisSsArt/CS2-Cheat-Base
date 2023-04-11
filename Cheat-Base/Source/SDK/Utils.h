#pragma once

#include <Windows.h>

#include "ScanUtils/Signature.h"

signature resolveRip(std::uint8_t* addr, std::uint32_t rva, std::uint32_t size);