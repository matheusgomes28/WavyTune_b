#ifndef WAVY_TUNE_RENDER_BUILDER_H
#define WAVY_TUNE_RENDER_BUILDER_H

#include <memory>

class AbstractRenderer;
class ConcreteRenderer;
class RenderBuilder
{
public:
	// TODO : Make this into a proper builder
	std::unique_ptr<ConcreteRenderer> buildBarRenderer(unsigned char* vs, std::size_t vs_size, unsigned char* fs, std::size_t fs_size);


	/*
	AbstractRenderer* buildOtherRenderer();
	*/
};

#endif
