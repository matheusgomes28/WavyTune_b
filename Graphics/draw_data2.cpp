// Includes from this project
#include "pch.h"
#include "draw_data2.h"

DrawData2::DrawData2()
{
}

auto DrawData2::getData() const -> const DrawVector&
{
	return data_;
}

void DrawData2::setData(const DrawVector& data)
{
	data_ = data;
}