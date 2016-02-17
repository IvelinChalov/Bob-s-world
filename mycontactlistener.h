#ifndef MYCONTACTLISTENER_H
#define MYCONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include <string>


class MyContactListener :public b2ContactListener
{
public:
   void BeginContact(b2Contact* contact);

   void EndContact(b2Contact* contact);

   void PreSolve (b2Contact *contact, const b2Manifold *oldManifold);

   void  PostSolve (b2Contact *contact, const b2ContactImpulse *impulse);

   void change_moving_platform_direction(int entity);

   void activate_spring_motor(int entity);

   void teleport_objectA(int entityA, int entityB, int contact_id, b2Contact *contact);

   void teleport_objectB(int entityA, int entityB, int contact_id, b2Contact *contact);

};

#endif // MYCONTACTLISTENER_H
