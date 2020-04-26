// Includes from this project
#include "pch.h"
#include "vertex_attribute.h"

VertexAttribute::VertexAttribute()
	: type_(VERTEX_TYPE::FLOAT),
	size_(0),
	offset_(0),
	stride_(0),
	normalised_(false)
{
}

VERTEX_TYPE VertexAttribute::getType() const
{
	return type_;
}

unsigned VertexAttribute::getSize() const
{
	return size_;
}

unsigned VertexAttribute::getOffset() const
{
	return offset_;
}

unsigned VertexAttribute::getStride() const
{
	return stride_;
}

bool VertexAttribute::getNormalised() const
{
	return normalised_;
}

void VertexAttribute::setType(const VERTEX_TYPE& type)
{
	type_ = type;
}

void VertexAttribute::setSize(const unsigned& size)
{
	size_ = size;
}

void VertexAttribute::setOffset(const unsigned& offset)
{
	offset_ = offset;
}

void VertexAttribute::setStride(const unsigned& stride)
{
	stride_ = stride;
}

void VertexAttribute::setNormalised(const bool& normalised)
{
	normalised_ = normalised;
}