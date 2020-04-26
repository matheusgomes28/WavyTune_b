// Includes from this project
#include "pch.h"
#include "colour_data.h"

auto ColourData::getData() const -> const DrawVector&
{
	return data_;
}

void ColourData::setData(const DrawVector& data)
{
	data_ = data;
}