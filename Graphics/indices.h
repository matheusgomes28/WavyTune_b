#ifndef GRAPHICS_INDICES_H
#define GRAPHICS_INDICES_H
#include "abstract_draw_data.h"

class Indices : public AbstractDrawData<unsigned>
{
public:
	Indices();

	const DrawVector& getData() const override;
	void setData(const DrawVector& data) override;
};
#endif