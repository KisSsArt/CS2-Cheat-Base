#include "Utils.h"

signature resolveRip(std::uint8_t* addr, std::uint32_t rva, std::uint32_t size)
{
	auto relative = addr + *reinterpret_cast<int*>(addr + rva);
	return signature(relative + size);
}