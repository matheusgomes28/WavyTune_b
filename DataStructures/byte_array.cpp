#include "pch.h"
#include "byte_array.h"
#include <cstdlib>

ByteArray::ByteArray(std::size_t size)
	: data_{ std::vector<Byte>(size) }
{
}

Byte* ByteArray::get_data()
{
	return &data_[0];
}

void ByteArray::set_data(Byte* data, std::size_t size, std::size_t offset)
{
	if (offset + size <= data_.size())
	{
		data_.insert(begin(data_) + offset, data, data + size);
	}

	else
	{
		// TODO
		// Throw an out of bounds addition here
	}
}