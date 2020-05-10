// Includes from this project
#include "pch.h"
#include "draw_data4.h"

auto DrawData4::getData() const -> const DrawVector&
{
	return data_;
}

void DrawData4::setData(const DrawVector& data)
{
	data_ = data;
}