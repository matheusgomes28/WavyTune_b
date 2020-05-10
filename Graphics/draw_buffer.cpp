// Includes from this project
#include "pch.h"
#include "draw_buffer.h"
#include "draw_data2.h"
#include "draw_data3.h"
#include "draw_data4.h"
#include "indices.h"

DrawBuffer::DrawBuffer()
	: description_(""),
	verticesPtr_(nullptr),
	normalsPtr_(nullptr),
	texelsPtr_(nullptr),
	colourPtr_(nullptr),
	indicesPtr_(nullptr)
{
}

DrawBuffer::DrawBuffer(const std::string& desc)
	: description_(desc),
	verticesPtr_(nullptr),
	normalsPtr_(nullptr),
	texelsPtr_(nullptr),
	colourPtr_(nullptr),
	indicesPtr_(nullptr)
{
}

DrawBuffer::~DrawBuffer()
{
	// Dont need anything as the unique pointers
	// will be deleted
}

const DrawData3& DrawBuffer::get_vertices() const
{
	return *verticesPtr_;
}

const DrawData3& DrawBuffer::get_normals() const
{
	return *normalsPtr_;
}

const DrawData2& DrawBuffer::get_texels() const
{
	return *texelsPtr_;
}

const Indices& DrawBuffer::get_indices() const
{
	return *indicesPtr_;
}

const DrawData4& DrawBuffer::get_colours() const
{
	return *colourPtr_;
}

void DrawBuffer::set_vertices(DrawData3Ptr vertices)
{
	verticesPtr_ = std::move(vertices);
}

void DrawBuffer::set_normals(DrawData3Ptr normals)
{
	normalsPtr_ = std::move(normals);
}

void DrawBuffer::set_texels(DrawData2Ptr texels)
{
	texelsPtr_ = std::move(texels);
}

void DrawBuffer::set_indices(IndicesPtr indices)
{
	indicesPtr_ = std::move(indices);
}

void DrawBuffer::set_colours(DrawData4Ptr colours)
{
	colourPtr_ = std::move(colours);
}

std::string DrawBuffer::get_description() const
{
	return description_;
}
void DrawBuffer::set_description(const std::string& desc)
{
	description_ = desc;
}
