#ifndef GRAPHICS_DRAW_DATA_3
#define GRAPHICS_DRAW_DATA_3
#include "abstract_draw_data.h"
#include <glm/glm.hpp>

class DrawData3 : public AbstractDrawData<glm::vec3>
{
public:
	DrawData3();

	const DrawVector& getData() const override;
	void setData(const DrawVector& data) override;
};
#endif

