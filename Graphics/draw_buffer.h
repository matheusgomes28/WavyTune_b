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

	virtual const DrawData3& get_vertices() const;
	virtual const DrawData3& get_normals() const;
	virtual const DrawData2& get_texels() const;
	virtual const DrawData4& get_colours() const;
	virtual const Indices& get_indices() const;

	virtual void set_vertices(DrawData3Ptr vertices);
	virtual void set_normals(DrawData3Ptr normals);
	virtual void set_texels(DrawData2Ptr texels);
	virtual void set_colours(DrawData4Ptr colours);
	virtual void set_indices(IndicesPtr indices);

	std::string get_description() const;
	void set_description(const std::string& texels);

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
