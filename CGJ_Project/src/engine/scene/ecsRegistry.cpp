#include "ecsRegistry.h"




EntityHandle ECSRegistry::createEntity()
{
	return EntityHandle(_nextId++);
}