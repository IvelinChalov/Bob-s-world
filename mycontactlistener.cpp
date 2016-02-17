#include "mycontactlistener.h"
#include <string>
#include <cmath>
#include "Entity.h"
#include <iostream>
#include <QDebug>

// if(!have_joint) {
//       b2RopeJointDef rope_joint_def;
//       rope_joint_def.collideConnected = true;
//       rope_joint_def.bodyA =  contact->GetFixtureA()->GetBody();
//       rope_joint_def.bodyB =  contact->GetFixtureB()->GetBody();
//       rope_joint_def.maxLength = 2;
//       world->CreateJoint(&rope_joint_def);
//       have_joint = true;
//     }else{
//       b2JointEdge* edge_joint = real_world.physics_body[*entityA].body->GetJointList();
//       b2RopeJoint* rj = (b2RopeJoint*)edge_joint->joint;
//       world->DestroyJoint(rj);
//       have_joint = false;
//     }

using namespace std;

b2Vec2 direction(0, -1);
int circle_id = 0;
bool inside = false;
bool teleportA_flag = false;
bool teleportB_flag = false;

void MyContactListener::BeginContact(b2Contact* contact) {
  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  int* entityA = static_cast<int*>(bodyUserDataA);
  std::string entityA_name = real_world.name_vec[*entityA].name;
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
  int* entityB = static_cast<int*>(bodyUserDataB);
  std::string entityB_name = real_world.name_vec[*entityB].name;
//Moving platform
if ((entityA_name == "moving_platform") && (entityB_name != "goal" && entityB_name != "wrecking_ball" && entityB_name != "circle" && entityB_name != "elavator_motor" && entityB_name != "domino")) {
    change_moving_platform_direction(*entityA);
  }else if((entityB_name == "moving_platform") && (entityA_name != "goal" && entityA_name != "wrecking_ball" && entityA_name != "circle" && entityA_name != "elavator_motor" && entityA_name != "domino")) {
    change_moving_platform_direction(*entityB);
  }

  //For Elevator------------------------------------------------
  b2Vec2 zero(0, 0);
 if(entityA_name == "elavator_platform" && entityB_name == "circle") {
  b2JointEdge* j = real_world.physics_body_vec[*entityA].body->GetJointList();
  b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
  //zero = real_world.physics_body_vec[*entityA].body->GetLinearVelocity();
  //real_world.physics_body_vec[*entityB].body->SetLinearVelocity(zero);
  pj->EnableMotor(true);
  pj->SetMotorSpeed(0.3);
  pj->SetMaxMotorForce(500);
  real_world.sticky_id[*entityB].on_platform = true;
  real_world.sticky_id[*entityB].platform_id = *entityA;
  if(pj->GetJointTranslation() <= pj->GetLowerLimit() || pj->GetJointTranslation() >= pj->GetUpperLimit()) {
     real_world.sticky_id[*entityB].on_platform = false;
//    real_world.physics_body_vec[*entityB].body->SetLinearVelocity(b2Vec2(-(real_world.physics_body_vec[*entityA].body->GetLinearVelocity().x * 0.99), -real_world.physics_body_vec[*entityA].body->GetLinearVelocity().y ));
//    real_world.physics_body_vec[*entityB].body->SetAngularVelocity(0);
  }
 }else if(entityB_name == "elavator_platform" && entityA_name == "circle") {
  b2JointEdge* j = real_world.physics_body_vec[*entityB].body->GetJointList();
  b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
  //zero = real_world.physics_body_vec[*entityB].body->GetLinearVelocity();
  //real_world.physics_body_vec[*entityA].body->SetLinearVelocity(zero);
  pj->EnableMotor(true);
  pj->SetMotorSpeed(0.3);
  pj->SetMaxMotorForce(500);
  real_world.sticky_id[*entityA].on_platform = true;
  real_world.sticky_id[*entityA].platform_id = *entityB;
//  cout<<"min = "<<pj->GetLowerLimit() * MTOP<<" current = "<<pj->GetJointTranslation() * MTOP<<endl;
//  cout<<"max = "<<pj->GetUpperLimit() * MTOP<<" current = "<<pj->GetJointTranslation() * MTOP<<endl;
//  if(pj->GetJointTranslation() <= pj->GetLowerLimit() || pj->GetJointTranslation() >= pj->GetUpperLimit()) {
//    cout<<"COLLISION !!\n";
//    // pj->EnableMotor(true);
//    //pj->SetMotorSpeed (pj->GetMotorSpeed() * -1);
//    real_world.physics_body_vec[*entityA].body->SetLinearVelocity(b2Vec2(-(real_world.physics_body_vec[*entityA].body->GetLinearVelocity().x * 0.99), -real_world.physics_body_vec[*entityA].body->GetLinearVelocity().y ));
//    real_world.physics_body_vec[*entityA].body->SetAngularVelocity(0);
//  }
 }else{
  if(entityA_name == "elavator_platform" && entityA_name != "circle") {
    b2JointEdge* j = real_world.physics_body_vec[*entityA].body->GetJointList();
    b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
    pj->EnableMotor(false);
  }

  if(entityB_name == "elavator_platform" && entityA_name != "circle") {
    b2JointEdge* j = real_world.physics_body_vec[*entityB].body->GetJointList();
    b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
    pj->EnableMotor(false);
  }
}
//For spring----------------------------------
if(entityA_name == "spring_platform" && (entityB_name == "goal" || entityB_name == "wrecking_ball" || entityB_name == "domino" || entityB_name == "circle")) {
    activate_spring_motor(*entityA);
 }else if(entityB_name == "spring_platform" && (entityA_name == "goal" || entityA_name == "wrecking_ball" || entityA_name == "domino" || entityA_name == "circle")) {
    activate_spring_motor(*entityB);
 }else if(entityA_name == "spring_platform" && entityA_name != "circle") {
    b2JointEdge* j = real_world.physics_body_vec[*entityA].body->GetJointList();
    b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
    pj->EnableMotor(false);
  }

 //For teleport ------------------------------------------------
if(entityA_name == "teleportA" && (entityB_name == "circle" || entityB_name == "domino" )) {
    teleport_objectA(*entityA, *entityB, 0, contact);
 }else if(entityB_name == "teleportA" && (entityA_name == "circle" || entityA_name == "domino" )) {
    teleport_objectA(*entityB, *entityA, 1, contact);
 }
 if(entityA_name == "teleportB" && (entityB_name == "circle" || entityB_name == "domino" )) {
     teleport_objectB(*entityA, *entityB, 0, contact);
 }else if(entityB_name == "teleportB" && (entityA_name == "circle" || entityA_name == "domino" )) {
     teleport_objectB(*entityB, *entityA, 1, contact);
 }

 //For fan-------------------------------------------------
 if(entityA_name == "fan") {
    cout<<"inside A\n";
    real_world.ent_flag_vec[*entityB].is_inside = true;
    real_world.ent_flag_vec[*entityB].fan_id = *entityA;
    //cout<<"mass = "<<contact->GetFixtureA()->GetBody()->GetMass();
  }else if(entityB_name == "fan") {
    cout<<"inside B\n";
    real_world.ent_flag_vec[*entityA].is_inside = true;
    real_world.ent_flag_vec[*entityA].fan_id = *entityB;
    // cout<<"mass = "<<contact->GetFixtureB()->GetBody()->GetMass();
  }

  if(entityA_name == "fan_button") {
    cout<<"button A\n";
    real_world.fan_vec[*entityA].is_on = !real_world.fan_vec[*entityA].is_on;
    if(real_world.fan_vec[*entityA].is_on) {
        real_world.color_vec[*entityA].selected_color = 1;
        real_world.textures_vec[*entityA].selected_texture = 3;
        int id = real_world.fan_vec[*entityA].fan_id;
        real_world.mask[id] = real_world.mask[id] + RENDER_MASK;
        real_world.textures_vec[id].selected_texture = 0;
    }else {
        real_world.color_vec[*entityA].selected_color = 0;
        real_world.textures_vec[*entityA].selected_texture = 0;
        int id = real_world.fan_vec[*entityA].fan_id;
        real_world.mask[id] = real_world.mask[id] - RENDER_MASK;
        real_world.textures_vec[id].selected_texture = 0;
    }
    cout<<"on = "<<real_world.fan_vec[*entityA].is_on<<endl;

  }else if(entityB_name == "fan_button") {
    cout<<"button B\n";
    real_world.fan_vec[*entityB].is_on = !real_world.fan_vec[*entityB].is_on;
    if(real_world.fan_vec[*entityB].is_on) {
       real_world.color_vec[*entityB].selected_color = 1;
       real_world.textures_vec[*entityB].selected_texture = 3;
       int id = real_world.fan_vec[*entityB].fan_id;
       real_world.mask[id] = real_world.mask[id] + RENDER_MASK;
       real_world.textures_vec[id].selected_texture = 0;
    }else {
      real_world.color_vec[*entityB].selected_color = 0;
      real_world.textures_vec[*entityB].selected_texture = 0;
      int id = real_world.fan_vec[*entityB].fan_id;
      real_world.mask[id] = real_world.mask[id] - RENDER_MASK;
      real_world.textures_vec[id].selected_texture = 0;
    }
    cout<<"on = "<<real_world.fan_vec[*entityB].is_on<<endl;
  }

  //balloon-------------------------
  if(entityA_name == "balloon") {
    float balloon_mass = contact->GetFixtureA()->GetBody()->GetMass();
    float gravity = contact->GetFixtureA()->GetBody()->GetGravityScale() * world->GetGravity().y;
    float force = balloon_mass * gravity;
    cout<<"balloon force = "<<force<<endl;
  }else if(entityB_name == "balloon") {
    float balloon_mass = contact->GetFixtureA()->GetBody()->GetMass();
    float gravity = contact->GetFixtureA()->GetBody()->GetGravityScale() * world->GetGravity().y;
    float force = balloon_mass * gravity;
    cout<<"balloon force = "<<force<<endl;
  }

  if(entityA_name == "balloon_platform") {
    b2Vec2 vel = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
    vel *= 0;
    real_world.physics_body_vec[*entityB].body->SetLinearVelocity(vel);
    real_world.physics_body_vec[*entityB].body->SetAngularVelocity(0);
    // float balloon_mass = contact->GetFixtureA()->GetBody()->GetMass();
    // float gravity = contact->GetFixtureA()->GetBody()->GetGravityScale() * world->GetGravity().y;
    // float force = balloon_mass * gravity;
    cout<<"Contact A "<<endl;
  }else if(entityB_name == "balloon") {
    b2Vec2 vel = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
    vel *= 0;
    real_world.physics_body_vec[*entityA].body->SetLinearVelocity(vel);
    real_world.physics_body_vec[*entityA].body->SetAngularVelocity(0);
    cout<<"Contact B "<<endl;
    // float balloon_mass = contact->GetFixtureA()->GetBody()->GetMass();
    // float gravity = contact->GetFixtureA()->GetBody()->GetGravityScale() * world->GetGravity().y;
    // float force = balloon_mass * gravity;
    // cout<<"balloon_platform force = "<<force<<endl;
  }

 //FOR MOVING DOWN ON A COLLISION WITH MOTOR
 // if(entityA_name == "elavator_platform" && entityB_name == "motor") {
 //  b2JointEdge* j = real_world.physics_body[*entityA].body->GetJointList();
 //  b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
 //  pj->SetMotorSpeed (-0.5);
 // }else if(entityB_name == "elavator_platform" && entityA_name == "motor") {
 //  b2JointEdge* j = real_world.physics_body[*entityB].body->GetJointList();
 //  b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
 //  pj->SetMotorSpeed (-0.5);
 // }

//   if (entityB_name == "elevator" && entityA_name == "circle") {
//     contact->GetFixtureB()->GetBody()->SetLinearVelocity(direction);
//     circle_id = *entityA;
//   }else if(entityA_name == "elevator" && entityB_name == "circle") {
//     contact->GetFixtureA()->GetBody()->SetLinearVelocity(direction);
//     circle_id = *entityB;
//   }
//   if (entityB_name == "elevator" && entityA_name != "circle") {
//     b2Vec2 vel(0, 0);
//     direction.x = contact->GetFixtureB()->GetBody()->GetLinearVelocity().x * mul;
//     direction.y = contact->GetFixtureB()->GetBody()->GetLinearVelocity().y * mul;
//     b2Vec2 a(10 * PTOM, 0);
//     real_world.physics_body[circle_id].body->ApplyLinearImpulse(a, real_world.physics_body[circle_id].body->GetPosition(), true);
//     contact->GetFixtureB()->GetBody()->SetLinearVelocity(vel);
//     contact->GetFixtureB()->GetBody()->SetAngularVelocity(0);
//   }else if(entityA_name == "elevator" && entityB_name != "circle") {
//     direction.x = contact->GetFixtureA()->GetBody()->GetLinearVelocity().x * mul;
//     direction.y = contact->GetFixtureA()->GetBody()->GetLinearVelocity().y * mul;
//     b2Vec2 vel(0, 0);
//     b2Vec2 a(10 * PTOM, 0);
//     real_world.physics_body[circle_id].body->ApplyLinearImpulse(a, real_world.physics_body[circle_id].body->GetPosition(), true);
//     contact->GetFixtureA()->GetBody()->SetLinearVelocity(vel);
//     contact->GetFixtureA()->GetBody()->SetAngularVelocity(0);
//   }
// cout<<"direction = "<<direction.y<<endl;
 }

void MyContactListener::EndContact(b2Contact* contact) {
  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  int* entityA = static_cast<int*>(bodyUserDataA);
  std::string entityA_name = real_world.name_vec[*entityA].name;
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
  int* entityB = static_cast<int*>(bodyUserDataB);
  std::string entityB_name = real_world.name_vec[*entityB].name;

  if(entityA_name == "elavator_platform" && entityA_name != "circle") {
    b2JointEdge* j = real_world.physics_body_vec[*entityA].body->GetJointList();
    if(j) {
      b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
      pj->EnableMotor(true);
    }
  }

  if(entityB_name == "elavator_platform" && entityA_name != "circle") {
    b2JointEdge* j = real_world.physics_body_vec[*entityB].body->GetJointList();
    if(j) {
      b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
      pj->EnableMotor(true);
    }
  }
  //For fan---------------------------------------------
   if(entityA_name == "fan") {
    cout<<"inside A\n";
    real_world.ent_flag_vec[*entityB].is_inside = false;
  }else if(entityB_name == "fan") {
    cout<<"inside B\n";
    real_world.ent_flag_vec[*entityA].is_inside = false;
  }
  // if(entityA_name == "fan" && entityB_name == "circle") {
  //   cout<<"inside A\n";
  //   real_world.ent_flag_vec[*entityB].is_inside = false;
  // }else if(entityA_name == "circle" && entityB_name == "fan") {
  //   cout<<"inside B\n";
  //   real_world.ent_flag_vec[*entityA].is_inside = false;
  // }

// if (entityA_name == "circle" && entityB_name == "moving_platform") {
//     real_world.for_delete_vec[*entityB].for_delete = false;
//   } else if(entityA_name == "moving_platform" && entityB_name == "circle") {
//     real_world.for_delete_vec[*entityA].for_delete = false;
//   }

  // void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  // int* entityA = static_cast<int*>(bodyUserDataA);
  // std::string entityA_name = real_world.name[*entityA].name;

  // void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
  // int* entityB = static_cast<int*>(bodyUserDataB);
  // std::string entityB_name = real_world.name[*entityB].name;

  // if ( entityA_name == "circle" && entityB_name == "obstacle") {
  //   real_world.color_vec[*entityA].red = 1.0f;
  //   real_world.color_vec[*entityA].green = 1.0f;
  //   real_world.color_vec[*entityA].blue = 1.0f;
  // }else if(entityA_name == "obstacle" && entityB_name == "circle") {
  //   real_world.color_vec[*entityB].red = 1.0f;
  //   real_world.color_vec[*entityB].green = 1.0f;
  //   real_world.color_vec[*entityB].blue = 1.0f;
  // }

  // if (entityA_name == "circle" && entityB_name == "slippery_obstacle") {
  //   contact->GetFixtureA()->SetFriction(0.1f);
  //   contact->GetFixtureB()->SetFriction(0.1f);

  // } else if(entityA_name == "slippery_obstacle" && entityB_name == "circle") {
  //   contact->GetFixtureA()->SetFriction(0.1f);
  //   contact->GetFixtureB()->SetFriction(0.1f);
  // }

}



void MyContactListener::PreSolve (b2Contact *contact, const b2Manifold *oldManifold) {
  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  int* entityA = static_cast<int*>(bodyUserDataA);
  std::string entityA_name = real_world.name_vec[*entityA].name;
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
  int* entityB = static_cast<int*>(bodyUserDataB);
  std::string entityB_name = real_world.name_vec[*entityB].name;
   b2Vec2 zero(0, 0);
//   if(entityA_name == "elavator_platform" && entityB_name == "circle") {
//     real_world.physics_body_vec[*entityB].body->SetLinearVelocity(zero);
//     real_world.physics_body_vec[*entityB].body->SetAngularVelocity(0);
//   }else if(entityB_name == "elavator_platform" && entityA_name == "circle") {
//     real_world.physics_body_vec[*entityA].body->SetLinearVelocity(zero);
//     real_world.physics_body_vec[*entityB].body->SetAngularVelocity(0);
//   }
  // if (entityA_name == "elevator") {
  //    b2Vec2 vel = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
  //    float ang_vel = contact->GetFixtureA()->GetBody()->GetAngularVelocity();
  //    cout<<"velX = "<<vel.x<<"velY = "<<vel.y<<endl;
  //    cout<<"ang vel = "<<ang_vel<<endl;
  //    vel.x = vel.x * -1;
  //    vel.y = vel.y * -1;
  //    contact->GetFixtureA()->GetBody()->SetLinearVelocity(zero);
  //    contact->GetFixtureA()->GetBody()->SetAngularVelocity(0);
  // }
  // if (entityB_name == "elevator") {
  //   b2Vec2 vel = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
  //   float ang_vel = contact->GetFixtureB()->GetBody()->GetAngularVelocity();
  //   vel.x = vel.x * -1;
  //   vel.y = vel.y * -1;
  //   contact->GetFixtureB()->GetBody()->SetLinearVelocity(zero);
  //   contact->GetFixtureB()->GetBody()->SetAngularVelocity(0);
  //   cout<<"velX = "<<vel.x<<"velY = "<<vel.y<<endl;
  //   cout<<"ang vel = "<<ang_vel<<endl;
  // }

  // b2Vec2 normal = contact->GetManifold()->localNormal;

  // cout<<"normalX = "<<normal.x<<"normalY = "<<normal.y<<endl;
  // cout<<"impulses1 = "<<oldManifold->points[1].normalImpulse = 0.f;
}

void  MyContactListener::PostSolve (b2Contact *contact, const b2ContactImpulse *impulse) {
  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  int* entityA = static_cast<int*>(bodyUserDataA);
  std::string entityA_name = real_world.name_vec[*entityA].name;
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
  int* entityB = static_cast<int*>(bodyUserDataB);
  std::string entityB_name = real_world.name_vec[*entityB].name;
  b2Vec2 zero(0, 0);
  // if(entityA_name == "elavator_platform" && entityB_name == "circle") {
  //   real_world.physics_body[*entityB].body->SetLinearVelocity(zero);
  // }else if(entityB_name == "elavator_platform" && entityA_name == "circle") {
  //   real_world.physics_body[*entityA].body->SetLinearVelocity(zero);
  // }
  // // b2Vec2 zero(0, 0);
  // if (entityA_name == "elavator") {
  //    b2Vec2 vel = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
  //    float ang_vel = contact->GetFixtureA()->GetBody()->GetAngularVelocity();
  //    cout<<"velX = "<<vel.x<<"velY = "<<vel.y<<endl;
  //    cout<<"ang vel = "<<ang_vel<<endl;
  //    vel.x = vel.x * -1;
  //    vel.y = vel.y * -1;
  //    contact->GetFixtureA()->GetBody()->SetLinearVelocity(zero);
  //    contact->GetFixtureA()->GetBody()->SetAngularVelocity(0);
  // }
  // if (entityB_name == "elavator") {
  //   b2Vec2 vel = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
  //   float ang_vel = contact->GetFixtureB()->GetBody()->GetAngularVelocity();
  //   vel.x = vel.x * -1;
  //   vel.y = vel.y * -1;
  //   contact->GetFixtureB()->GetBody()->SetLinearVelocity(zero);
  //   contact->GetFixtureB()->GetBody()->SetAngularVelocity(0);
  //   cout<<"velX = "<<vel.x<<"velY = "<<vel.y<<endl;
  //   cout<<"ang vel = "<<ang_vel<<endl;
  // }
}

void MyContactListener::change_moving_platform_direction(int entity)
{
    float mul = -1;
    b2Vec2 vel;
    vel.x = real_world.velocity_vec[entity].x * mul;
    vel.y = real_world.velocity_vec[entity].y * mul;
    real_world.velocity_vec[entity] = vel;
    b2JointEdge* j = real_world.physics_body_vec[entity].body->GetJointList();
    if(j)j->joint->GetBodyB()->SetLinearVelocity(vel);//if the core body is deleted prevents from null
}

void MyContactListener::activate_spring_motor(int entity) {
    b2JointEdge* j = real_world.physics_body_vec[entity].body->GetJointList();
    b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
    pj->SetMotorSpeed (-5);
}

void MyContactListener::teleport_objectA(int entityA, int entityB, int contact_id, b2Contact* contact) {
    if(!teleportA_flag){
      b2JointEdge* j = real_world.physics_body_vec[entityA].body->GetJointList();
      b2Vec2 pos = j->other->GetPosition();

      float teleportB_angle = j->other->GetAngle();
      b2Vec2 ball_vel;
      float angle;
      if(contact_id == 0) {
          ball_vel = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
          angle = contact->GetFixtureB()->GetBody()->GetAngle();
      }else{
          ball_vel = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
          angle = contact->GetFixtureA()->GetBody()->GetAngle();
      }
      b2Vec2 vel;
      float multiplier = 0;
      if(abs(ball_vel.x) >= abs(ball_vel.y)) {
        multiplier = abs(ball_vel.x);
      }else {
        multiplier = abs(ball_vel.y);
      }
      vel.x = multiplier * cos(teleportB_angle);
      vel.y = multiplier * sin(teleportB_angle);


      qDebug() << "teleport angle: " << angle;
      real_world.teleportation_vec[entityB].is_teleporting = true;
      real_world.teleportation_vec[entityB].coordinates = pos;
      real_world.teleportation_vec[entityB].angle = angle;
      real_world.teleportation_vec[entityB].velocity = vel;
      teleportB_flag = true;
      teleportA_flag = false;
    }else{
      teleportA_flag = false;
    }
}

void MyContactListener::teleport_objectB(int entityA, int entityB, int contact_id, b2Contact *contact) {
    if(!teleportB_flag){
      b2JointEdge* j = real_world.physics_body_vec[entityA].body->GetJointList();
      b2Vec2 pos = j->other->GetPosition();

      float teleportA_angle = j->other->GetAngle();
      b2Vec2 ball_vel;
      float angle;
      if(contact_id == 0) {
          ball_vel = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
          angle = contact->GetFixtureB()->GetBody()->GetAngle();
      }else{
          ball_vel = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
          angle = contact->GetFixtureA()->GetBody()->GetAngle();
      }

      b2Vec2 vel;
      float multilyer = 0;
      if(abs(ball_vel.x) >= abs(ball_vel.y)) {
        multilyer = abs(ball_vel.x);
      }else {
        multilyer = abs(ball_vel.y);
      }
      vel.x = multilyer * cos(teleportA_angle);
      vel.y = multilyer * sin(teleportA_angle);


      real_world.teleportation_vec[entityB].is_teleporting = true;
      real_world.teleportation_vec[entityB].coordinates = pos;
      real_world.teleportation_vec[entityB].angle = angle;
      real_world.teleportation_vec[entityB].velocity = vel;

      teleportA_flag = true;
      teleportB_flag = false;
    }else {
      teleportB_flag = false;
    }

}


