#ifndef GLABSTRACTIONS_ABSTRACT_ID_OBJECT_H
#define GLABSTRACTIONS_ABSTRACT_ID_OBJECT_H

#include <string>
#include <GL/glew.h>

class AbstractIdObject
{
public:
	AbstractIdObject();
	AbstractIdObject(const std::string& name);
	virtual bool isValid() const;

	GLuint getId() const;
	std::string getName() const;

	void setName(const std::string& name);

private:
	std::string name_;
	GLuint id_;

	virtual void generateId() = 0;

protected:
	void setId(const GLuint& id);
};
#endif
