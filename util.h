#pragma once
#include <typeinfo>
#include <typeindex>

template<typename T, typename U>
bool is_of_type(U* other)
{
	return typeid(*other) == typeid(T);
}

template<typename T, typename U>
bool has_parent(U* other)
{
	auto tmp = std::type_index(typeid(*other));
	return false;
}
constexpr bool is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };
	return bint.c[0] == 1;
}

constexpr uint16_t swap16(uint16_t in)
{
	uint16_t ret = in & 0xff;
	ret <<= 8;
	in >>= 8;
	return ret | (in & 0xff);
}
constexpr uint32_t swap32(uint32_t in)
{
	uint32_t ret = in & 0xff;
	ret <<= 8;
	in >>= 8;
	for (unsigned i = 0; i < 2; i++)
	{
		ret |= in & 0xff;
		ret <<= 8;
		in >>= 8;
	}
	ret |= in & 0xff;
	return ret;
}
constexpr uint32_t swap64(uint64_t in)
{
	uint32_t ret = in & 0xff;
	ret <<= 8;
	in >>= 8;
	for (unsigned i = 0; i < 6; i++)
	{
		ret |= in & 0xff;
		ret <<= 8;
		in >>= 8;
	}
	ret |= in & 0xff;
	return ret;
}