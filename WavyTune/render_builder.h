#ifndef WAVY_TUNE_RENDER_BUILDER_H
#define WAVY_TUNE_RENDER_BUILDER_H

class AbstractRenderer;
class RenderBuilder
{
public:
	// TODO : Make this into a proper builder
	static AbstractRenderer* buildBarRenderer();


	/*
	AbstractRenderer* buildOtherRenderer();
	*/
};

#endif
