#include "DeleteSystem.h"
#include <iostream>
#include <QDebug>

using namespace std;

void CheckForDelete(b2Body* body, int id, int screen_height) {
    if (body->GetWorldCenter().y * MTOP > screen_height) {
        BodyDelete *delete_body;
        delete_body = &(real_world.for_delete_vec[id]);
        delete_body->for_delete = true;
    }
}

#define DELETE_ENTITY_SYSTEM_MASK (COMPONENT_PHYSICS_BODY | BODY_DELETE)

void DeleteEntitySystem() {
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
       // cout<< "-------------------------------------------" << endl;
       // cout<< "Delete mask = " << DELETE_ENTITY_SYSTEM_MASK << endl;
       // int b = real_world.mask[entity] & DELETE_ENTITY_SYSTEM_MASK;
       // cout << "mask = " << real_world.mask[entity] <<endl;
        //cout<< "mask & = " << b << endl;
        //cout << "mask From DeleteEntitySystem Before = " << real_world.mask[entity] << endl;
        //if((real_world.mask[entity] & DELETE_ENTITY_SYSTEM_MASK) == DELETE_ENTITY_SYSTEM_MASK) {
        if(real_world.mask[entity] != COMPONENT_NONE) {

          //  cout<<"-----------------------------------------------------"<<endl;
          //  cout <<"entiy " << real_world.name_vec[entity].name << endl;
          //  cout << "Flag = " << real_world.for_delete_vec[entity].for_delete << endl;
          //  cout<<"-----------------------------------------------------"<<endl;
            PhysicsBody* physics_body;
            physics_body = &(real_world.physics_body_vec[entity]);
            b2Body* body = physics_body->body;
            BodyDelete *delete_body;
            delete_body = &(real_world.for_delete_vec[entity]);
            if (delete_body->for_delete) {
                //qDebug() <<"entity = "<<entity<<" delete flag = "<<delete_body->for_delete<<endl;
                std::cout<<"in delete = "<<entity<<" \n";
                ResetEntity(entity);
                world->DestroyBody(body);
             //   cout<<"-----------------------------------------------------"<<endl;
             //   cout <<"After entiy " << real_world.name_vec[entity].name << " destroyed" << endl;
             //   cout << "After Flag = " << real_world.for_delete_vec[entity].for_delete << endl;
             //   cout<<"-----------------------------------------------------"<<endl;
             //   cout << "mask From DeleteEntitySystem After = " << real_world.mask[entity] << endl;
            }
        }
    }
}

void ResetEntity(int id) {
  qDebug() <<"In reset components " <<  QString::fromStdString(real_world.name_vec[id].name)<<"\n";
  real_world.mask[id] = COMPONENT_NONE;
  real_world.has_platform_vec[id].has_platform = false;
  b2Vec2 vel(-1.f, 0.f);
  real_world.velocity_vec[id] = vel;
  real_world.selected_vec[id].selected = false;
  real_world.color_vec[id].selected_color = 0;
  real_world.teleportation_vec[id].is_teleporting = false;
  real_world.fan_vec[id].is_reversed = false;
  real_world.fan_vec[id].is_on = false;
  real_world.ent_flag_vec[id].is_inside = false;
  real_world.inverted_vec[id].is_inverted = false;
  real_world.template_flags_vec[id].is_template = false;
  real_world.quantity[id] = 0;
  real_world.for_delete_vec[id].for_delete = false;
 // cout<<"end of reset\n";
}

void clear_all_entities() {
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
       // if((real_world.mask[entity] & DELETE_ENTITY_SYSTEM_MASK) == DELETE_ENTITY_SYSTEM_MASK) {
        if(real_world.mask[entity] != COMPONENT_NONE) {
            real_world.for_delete_vec[entity].for_delete = true;
            //cout<<"-----------------------------------------------------"<<endl;
            //cout <<"entiy " << real_world.name_vec[entity].name << " for destuction" << endl;
            cout << "mask From clear_all_entities = " << real_world.mask[entity] << endl;
            //cout << "Flag = " << real_world.for_delete_vec[entity].for_delete << endl;
            //cout<<"-----------------------------------------------------"<<endl;
        }
    }
}
