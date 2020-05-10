#ifndef GLABSTRACTIONS_VAO_H
#define GLABSTRACTIONS_VAO_H

#include "abstract_id_object.h"

#include <map>
#include <memory>
#include <string>
class VBO;
class VertexAttribute;
class VAO : public AbstractIdObject
{
	using BufferPtr = VBO*;
	using AttributesPtr = std::unique_ptr<VertexAttribute>;
	using BufferSettingsMap = std::map<BufferPtr, AttributesPtr>;

public:
	VAO();
	VAO(const std::string& name);

	void addBufferConfigs(BufferPtr buffer, AttributesPtr attr);
	const BufferSettingsMap& getBufferConfigs() const;


private:
	BufferSettingsMap buffer_settings_;
	void generateId() override;
};
#endif
