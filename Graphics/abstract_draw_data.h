#ifndef GRAPHICS_ABSTRACT_DRAW_DATA
#define GRAPHICS_ABSTRACT_DRAW_DATA
#include <vector>

template <typename T>
class AbstractDrawData
{
public:
	using DrawVector = std::vector<T>;
	virtual const DrawVector& getData() const = 0;
	virtual void setData(const DrawVector& data) = 0;

	unsigned getGPUSize() const
	{
		return sizeof(T) * data_.size();
	}

protected:
	std::vector<T> data_;
};

#endif

