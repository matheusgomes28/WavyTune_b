#ifndef GRAPHICS_COLOUR_DATA_H
#define GRAPHICS_COLOUR_DATA_H
#include <glm/glm.hpp>
#include "abstract_draw_data.h"

class ColourData : public AbstractDrawData<glm::vec4>
{
public:
	const DrawVector& getData() const override;
	void setData(const DrawVector& data) override;
};
#endif
