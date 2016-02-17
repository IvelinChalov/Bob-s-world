#ifndef DELETESYSTEM
#define DELETESYSTEM

#include "Component.h"
#include "Entity.h"
#include "glwidget.h"

void DeleteEntitySystem();
void ResetEntity(int id);
void CheckForDelete(b2Body* body, int id, int screen_height);
void clear_all_entities();

#endif // DELETESYSTEM

