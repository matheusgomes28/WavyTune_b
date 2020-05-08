#ifndef WAVY_TUNE_RENDER_BUILDER_H
#define WAVY_TUNE_RENDER_BUILDER_H

#include <memory>

class AbstractRenderer;
class RenderBuilder
{
public:
	// TODO : Make this into a proper builder
	static std::unique_ptr<AbstractRenderer> buildBarRenderer();


	/*
	AbstractRenderer* buildOtherRenderer();
	*/
};

#endif
