#ifndef ABSTRACTIONS_VERTEX_ATTRIBUTE_H
#define ABSTRACTIONS_VERTEX_ATTRIBUTE_H

#include <GL/glew.h>
enum class VERTEX_TYPE {
	BYTE = GL_BYTE,
	UBYTE = GL_UNSIGNED_BYTE,
	SHORT = GL_SHORT,
	USHORT = GL_UNSIGNED_SHORT,
	INT = GL_INT,
	UINT = GL_UNSIGNED_INT,
	FLOAT = GL_FLOAT
};

class VertexAttribute
{
public:
	VertexAttribute();

	VERTEX_TYPE getType() const;
	unsigned getSize() const;
	unsigned getOffset() const;
	unsigned getStride() const;
	bool getNormalised() const;

	void setType(const VERTEX_TYPE& type);
	void setSize(const unsigned& size);
	void setOffset(const unsigned& offset);
	void setStride(const unsigned& stride);
	void setNormalised(const bool& normalised);

private:
	VERTEX_TYPE type_;
	unsigned size_;
	unsigned offset_;
	unsigned stride_;
	bool normalised_;
};
#endif
