#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <list>

class Entity;

class EntityList
{
public:
    static EntityList& getInstance() {
        static EntityList  p_instance;
        return p_instance;
    }

    std::list<Entity*> list;

private:
    static EntityList * p_instance;
    EntityList() {}
    EntityList( const EntityList& );
    EntityList& operator=( EntityList& );
};

#endif // ENTITYLIST_H
