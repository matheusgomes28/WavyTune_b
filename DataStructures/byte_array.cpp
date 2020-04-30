#include "pch.h"
#include "byte_array.h"
#include <algorithm>
#include <cstdlib>

ByteArrayException::ByteArrayException(std::string err_message)
	: std::runtime_error{err_message},
	  message_{ std::move(err_message) }
{
}

const char* ByteArrayException::what() const
{
	return message_.c_str();
}

ByteArray::ByteArray(std::size_t size)
	: data_{ std::vector<Byte>(size) }
{
}

std::size_t ByteArray::get_size() const
{
	return data_.size();
}

Byte* ByteArray::get_data()
{
	return &data_[0];
}

void ByteArray::set_data(Byte* data, std::size_t size, std::size_t offset)
{
	if (offset + size <= data_.size())
	{
		// cant use std::replace with raw pointers :(
		for (int i = 0; i < size; ++i)
		{
			data_[i] = data[i];
		}
	}
	else
	{
		throw ByteArrayException("data given lies out of bound");
	}
}