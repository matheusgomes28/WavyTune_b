// Includes from this project
#include "pch.h"
#include "draw_buffer.h"
#include "draw_data2.h"
#include "draw_data3.h"
#include "indices.h"
#include "colour_data.h"

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

const DrawData3& DrawBuffer::getVertices() const
{
	return *verticesPtr_;
}

const DrawData3& DrawBuffer::getNormals() const
{
	return *normalsPtr_;
}

const DrawData2& DrawBuffer::getTexels() const
{
	return *texelsPtr_;
}

const Indices& DrawBuffer::getIndices() const
{
	return *indicesPtr_;
}

const ColourData& DrawBuffer::getColours() const
{
	return *colourPtr_;
}

void DrawBuffer::setVertices(DrawData3* vertices)
{
	verticesPtr_ = std::unique_ptr<DrawData3>(vertices);
}

void DrawBuffer::setNormals(DrawData3* normals)
{
	normalsPtr_ = std::unique_ptr<DrawData3>(normals);
}

void DrawBuffer::setTexels(DrawData2* texels)
{
	texelsPtr_ = std::unique_ptr<DrawData2>(texels);
}

void DrawBuffer::setIndices(Indices* indices)
{
	indicesPtr_ = std::unique_ptr<Indices>(indices);
}

void DrawBuffer::setColours(ColourData* colours)
{
	colourPtr_ = std::unique_ptr<ColourData>(colours);
}

std::string DrawBuffer::getDescription() const
{
	return description_;
}
void DrawBuffer::setDescription(const std::string& desc)
{
	description_ = desc;
}
