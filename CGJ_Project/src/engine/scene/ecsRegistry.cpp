#include "ecsRegistry.h"




EntityHandle ECSRegistry::createEntity()
{
	return EntityHandle(_nextId++);
}


void ECSRegistry::destroyEntity(EntityHandle entityHandle)
{
	// empty
}