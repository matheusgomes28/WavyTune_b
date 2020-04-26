// Includes from this project
#include "pch.h"
#include "indices.h"

Indices::Indices()
{
}

auto Indices::getData() const -> const DrawVector&
{
	return data_;
}

void Indices::setData(const DrawVector& data)
{
	data_ = data;
}