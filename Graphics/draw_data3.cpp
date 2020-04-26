// Includes form this project
#include "pch.h"
#include "draw_data3.h"

DrawData3::DrawData3()
{
}

auto DrawData3::getData() const -> const DrawVector&
{
	return data_;
}

void DrawData3::setData(const DrawVector& data)
{
	data_ = data;
}