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
class ColourData;
class DrawBuffer
{
	using IndicesPtr = std::unique_ptr<Indices>;
	using ColourDataPtr = std::unique_ptr<ColourData>;
	using DrawData2Ptr = std::unique_ptr<DrawData2>;
	using DrawData3Ptr = std::unique_ptr<DrawData3>;
public:

	DrawBuffer();
	DrawBuffer(const std::string& desc);
	virtual ~DrawBuffer();

	virtual const DrawData3& getVertices() const;
	virtual const DrawData3& getNormals() const;
	virtual const DrawData2& getTexels() const;
	virtual const Indices& getIndices() const;
	virtual const ColourData& getColours() const;


	virtual void setVertices(DrawData3* vertices);
	virtual void setNormals(DrawData3* normals);
	virtual void setTexels(DrawData2* texels);
	virtual void setIndices(Indices* indices);
	virtual void setColours(ColourData* colours);

	std::string getDescription() const;
	void setDescription(const std::string& texels);

	//! virtual glm::mat4 getTransform() const = 0;

private:
	DrawData3Ptr verticesPtr_;
	DrawData3Ptr normalsPtr_;
	DrawData2Ptr texelsPtr_;
	IndicesPtr indicesPtr_;
	ColourDataPtr colourPtr_;
	std::string description_;
	//! virtual void setUp() = 0;
};
#endif
