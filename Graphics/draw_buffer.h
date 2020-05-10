#ifndef GRAPHICS_ABSTRACT_DRAW_BUFFER_H
#define GRAPHICS_ABSTRACT_DRAW_BUFFER_H
#include <memory>
#include <string>

//! TODO : Think about whether this class really needs
//! to be a virtual class. I think this is pretty general
//! enough and it should handle all drawing cases.

class DrawData2;
class DrawData3;
class Indices;
class DrawData4;
class DrawBuffer
{
	using IndicesPtr = std::unique_ptr<Indices>;
	using DrawData2Ptr = std::unique_ptr<DrawData2>;
	using DrawData3Ptr = std::unique_ptr<DrawData3>;
	using DrawData4Ptr = std::unique_ptr<DrawData4>;
public:

	DrawBuffer();
	DrawBuffer(const std::string& desc);
	virtual ~DrawBuffer();

	virtual const DrawData3& getVertices() const;
	virtual const DrawData3& getNormals() const;
	virtual const DrawData2& getTexels() const;
	virtual const Indices& getIndices() const;
	virtual const DrawData4& getColours() const;


	virtual void setVertices(DrawData3* vertices);
	virtual void setNormals(DrawData3* normals);
	virtual void setTexels(DrawData2* texels);
	virtual void setIndices(Indices* indices);
	virtual void setColours(DrawData4* colours);

	std::string getDescription() const;
	void setDescription(const std::string& texels);

	//! virtual glm::mat4 getTransform() const = 0;

private:
	DrawData3Ptr verticesPtr_;
	DrawData3Ptr normalsPtr_;
	DrawData2Ptr texelsPtr_;
	DrawData4Ptr colourPtr_;
	IndicesPtr indicesPtr_;
	std::string description_;
	//! virtual void setUp() = 0;
};
#endif
