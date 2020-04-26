#include "pch.h"
#include "abstract_id_object.h"

AbstractIdObject::AbstractIdObject()
	: name_(""),
	id_(0)
{
}

AbstractIdObject::AbstractIdObject(const std::string& name)
	: name_(name),
	id_(0)
{
}

bool AbstractIdObject::isValid() const
{
	return id_ != 0;
}

GLuint AbstractIdObject::getId() const
{
	return id_;
}

std::string AbstractIdObject::getName() const
{
	return name_;
}

void AbstractIdObject::setName(const std::string& name)
{
	name_ = name;
}

void AbstractIdObject::setId(const GLuint& id)
{
	id_ = id;
}