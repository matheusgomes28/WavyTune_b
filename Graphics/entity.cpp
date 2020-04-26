// Includes from this project
#include "pch.h"
#include "entity.h"

// TODO : make this class properly thread-safe
unsigned Entity::nextId = 0;
std::mutex Entity::idMutex;

void Entity::assignId()
{
	std::lock_guard<std::mutex> lg(Entity::idMutex);
	id_ = ++Entity::nextId;
}

Entity::Entity()
	: name_("__none__"),
	parent_(nullptr)
{
	assignId();
}

Entity::Entity(const std::string& name)
	: name_(name),
	parent_(nullptr)
{
	assignId();
}

Entity::Entity(Entity* parent)
	: parent_(parent)
{
	assignId();
}

Entity::Entity(const std::string& name, Entity* parent)
	: name_(name),
	parent_(parent)
{
	assignId();
}

Entity::~Entity()
{
	for (Entity* child : children_) {
		delete child;
	}
}

std::string Entity::getName() const
{
	return name_;
}

auto Entity::getParent() const -> EntityPtr
{
	return parent_;
}

auto Entity::getChild(const unsigned& i) const -> EntityPtr
{
	return children_.at(i);
}

unsigned Entity::getChildrenCount() const
{
	return children_.size();
}

void Entity::setName(const std::string& name)
{
	name_ = name;
}

void Entity::setParent(Entity* parent)
{
	parent_ = EntityPtr(parent);
}

void Entity::addChild(Entity* child)
{
	children_.emplace_back(EntityPtr(child));
}