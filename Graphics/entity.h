#ifndef GRAPHICS_ENTITY_H
#define GRAPHICS_ENTITY_H
#include <mutex>
#include <vector>

class Entity
{
	using EntityPtr = Entity*;
	using ParentPtr = Entity*;
	using EntityChildren = std::vector<EntityPtr>;

public:
	Entity();
	Entity(const std::string& name);
	Entity(Entity* parent);
	Entity(const std::string& name, Entity* parent);
	~Entity();

	std::string getName() const;
	EntityPtr getParent() const;
	EntityPtr getChild(const unsigned& i) const;
	unsigned getChildrenCount() const;

	void setName(const std::string& name);
	void setParent(Entity* parent);
	void addChild(Entity* child);

private:

	std::string name_;
	EntityPtr parent_;
	EntityChildren children_;
	unsigned id_;

	void assignId();
	static unsigned nextId;
	static std::mutex idMutex;
};
#endif

