#ifndef GLABSTRACTIONS_VBO_H
#define GLABSTRACTIONS_VBO_H

#include <string>
#include <vector>
#include "abstract_id_object.h"
class VBO : public AbstractIdObject
{
public:
	VBO();
	VBO(const std::string& name);

	void allocateMemory(const unsigned& size) const;

	template <typename T>
	void addData(const T& data, const unsigned& offset = 0) const
	{
		const unsigned ELEM_SIZE = sizeof(T);
		addData(&data, ELEM_SIZE, offset);
	}

	template <typename T>
	void addData(const std::vector<T>& data, const unsigned& offset = 0) const
	{
		const unsigned ELEM_SIZE = sizeof(T);
		const unsigned VECTOR_SIZE = data.size();
		addData(&data[0], ELEM_SIZE * VECTOR_SIZE, offset);
	}

	void addData(const void* data, const unsigned& size, const unsigned& offset = 0) const;

private:
	void generateId() override;
};
#endif
