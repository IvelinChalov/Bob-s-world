#include "Component.h"
#include "Entity.h"
//#include "System.h"
#include "glwidget.h"
#include <stdio.h>
#include <cmath>
#include <QDebug>
#include <iostream>
#include <mainwindow.h>
#include <QDesktopWidget>
using namespace std;

int createEntity() {
    int result;
    if(real_world.mask.size() == 0) {
       result = global_id;
       init_entity(global_id);
       return(result);
    }
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
        if(real_world.mask[entity] == COMPONENT_NONE) {
       result = entity;
     return(result);
        }
    }

  global_id++;
    result = global_id;

  init_entity(global_id);
  return(result);
}

void init_entity(int id) {
  Motor motor;
  Coordinates coordinates;
  PhysicsBody physics_body;
  Name name;
  BodyType body_type;
  BodySize body_size;
  Selected selected;
  Colors color;
  Teleportation teleport;
  EntityFanFlags entity;
  Fan fan;
  Inverted inv;
  JointLenght joint_len;
  TemplateFlag tem_flag;
  EntityFlags e_flags;
  Textures text;
  Sticky sticky;
  BodyDelete body_delete;

  int mask = COMPONENT_NONE;
  int quantity = 0;
  b2Vec2 vel(1.f, 0.f);

  real_world.mask.push_back(mask);
  int buff = id;
  real_world.entity_num[buff] = buff;
  real_world.velocity_vec.push_back(vel);
  real_world.has_platform_vec.push_back(motor);
  real_world.coordinates_vec.push_back(coordinates);
  real_world.physics_body_vec.push_back(physics_body);
  real_world.name_vec.push_back(name);
  real_world.body_type_vec.push_back(body_type);
  real_world.body_size_vec.push_back(body_size);
  real_world.selected_vec.push_back(selected);
  real_world.color_vec.push_back(color);
  real_world.teleportation_vec.push_back(teleport);
  real_world.ent_flag_vec.push_back(entity);
  real_world.fan_vec.push_back(fan);
  real_world.joint_vec.push_back(joint_len);
  real_world.inverted_vec.push_back(inv);
  real_world.entity_flags_vec.push_back(e_flags);
  real_world.quantity.push_back(quantity);
  real_world.template_flags_vec.push_back(tem_flag);
  real_world.template_id.push_back(-1);
  real_world.textures_vec.push_back(text);
  real_world.sticky_id.push_back(sticky);
  real_world.for_delete_vec.push_back(body_delete);
}

void destroyEntity(unsigned int entity)
{
    printf("Entity destroyed: %d\n", entity);
    real_world.mask[entity] = COMPONENT_NONE;
}


b2Body* create_box_entity(int x, int y, int width, int height, string type, string name, string full_name,
                            bool sel, bool is_sensor, bool is_invisible, bool has_gravity, int color_mask,
                            int selected_color, bool is_template, float angle, int texture_mask, int selected_texture) {
  int id = createEntity();
  int scaled_width = scale_width(width);
  int scaled_heiht = scale_height(height);

  int mask = COMPONENT_PHYSICS_BODY | COMPONENT_BODYTYPE | COMPONENT_NAME | SAVE_MASK | MOVE | RENDER_MASK| COLLISION | BODY_DELETE;
  if (is_invisible) {
    mask = COMPONENT_BODYTYPE | COMPONENT_NAME | COMPONENT_PHYSICS_BODY | SAVE_MASK | MOVE | BODY_DELETE;
  }else if(name.find("template") != std::string::npos) {
    cout<<"name from create box = " << name << endl;
    mask = COMPONENT_PHYSICS_BODY | COMPONENT_BODYTYPE | COMPONENT_NAME | SAVE_MASK | MOVE | COLLISION | RENDER_MASK | BODY_DELETE | QUANTITY_MASK;
  }
   // std::cout<<"start else\n";
  real_world.mask[id] = mask;
  int buff = id;
  real_world.entity_num[buff] = buff;

  real_world.entity_flags_vec[id].is_sensor = is_sensor;
  real_world.entity_flags_vec[id].is_visible = is_invisible;
  real_world.entity_flags_vec[id].has_gravity = has_gravity;

  real_world.template_flags_vec[id].is_template = is_template;

  Color* col;
  col = get_colors(color_mask);
  for (int i = 0; i < 3; ++i) {
    real_world.color_vec[id].color[i] = col[i];
  }

  real_world.color_vec[id].color_mask = color_mask;
  real_world.color_vec[id].selected_color = selected_color;
  real_world.body_type_vec[id].type = type;

  //Textures* texture;
  unsigned int* texture_arr = get_textures(texture_mask);
  for (int i = 0; i < 7; ++i) {
    real_world.textures_vec[id].texture_arr[i] = texture_arr[i];
  }

  real_world.textures_vec[id].texture_mask = texture_mask;
  real_world.textures_vec[id].selected_texture = selected_texture;
  real_world.name_vec[id].name = name;
  real_world.name_vec[id].full_name = full_name;
  b2Body* body;
      body = create_box_object(x, y, scaled_width, scaled_heiht, type, id, is_sensor, has_gravity, angle);
      real_world.body_size_vec[id].width = scaled_width;
      real_world.body_size_vec[id].height = scaled_heiht;

  real_world.selected_vec[id].selected = sel;

  real_world.coordinates_vec[id].x = x;
  real_world.coordinates_vec[id].y = y;
  real_world.coordinates_vec[id].angle = angle;
  real_world.physics_body_vec[id].body = body;
  return body;
}

b2Body* create_box_object(int x, int y, int width, int height, string type, int entity, bool is_sensor, bool has_gravity, float angle) {
    b2BodyDef bodyDef;
    if (type == "dynamic") {
      bodyDef.type = b2_dynamicBody;
    }else if (type == "static"){
      bodyDef.type = b2_staticBody;
    }else {
      bodyDef.type = b2_kinematicBody;
    }
    bodyDef.position.Set(x * PTOM, y * PTOM);//user data за ID
    if(!has_gravity) {
      bodyDef.gravityScale = 0;
    }
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(&real_world.entity_num[entity]);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((width * PTOM)/2, (height * PTOM)/2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    if(is_sensor) {
      fixtureDef.isSensor = true;
    }
    fixtureDef.density = 0.5f;

    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    body->CreateFixture(&fixtureDef);
    body->SetTransform(b2Vec2(x * PTOM, y * PTOM), angle * DTOR);
    return body;
}

b2Body* create_circle_entity(float x, float y, int radius, float density, std::string type,
                                std::string name, string full_name, bool sel, int color_mask, int sel_col, bool is_template, int texture_mask, int sel_texure) {
  int id = createEntity();
  int scaled_radius = scale_width(radius);

  int mask = COMPONENT_PHYSICS_BODY | COMPONENT_BODYTYPE | RENDER_MASK | COMPONENT_NAME | SAVE_MASK | MOVE |COLLISION | BODY_DELETE;

  if(name.find("template") != std::string::npos) {
      mask = COMPONENT_PHYSICS_BODY | SAVE_MASK | COMPONENT_BODYTYPE | RENDER_MASK | COMPONENT_NAME | MOVE |COLLISION | BODY_DELETE;
  }
  real_world.mask[id] = mask;
  real_world.body_type_vec[id].type = type;
  real_world.name_vec[id].name = name;
  real_world.name_vec[id].full_name = full_name;

  real_world.coordinates_vec[id].x = x;
  real_world.coordinates_vec[id].y = y;
  real_world.coordinates_vec[id].angle = 0;

  real_world.selected_vec[id].selected = sel;

  real_world.template_flags_vec[id].is_template = is_template;

  Color* col;
  col = get_colors(color_mask);
  for (int i = 0; i < 3; ++i) {
    real_world.color_vec[id].color[i] = col[i];
  }
  real_world.color_vec[id].color_mask = color_mask;
  real_world.color_vec[id].selected_color = sel_col;

  unsigned int* texture_arr = get_textures(texture_mask);
  for (int i = 0; i < 7; ++i) {
    real_world.textures_vec[id].texture_arr[i] = texture_arr[i];
  }

  real_world.textures_vec[id].texture_mask = texture_mask;
  real_world.textures_vec[id].selected_texture = sel_texure;

  real_world.body_size_vec[id].width = scaled_radius*2;
  real_world.body_size_vec[id].height = scaled_radius*2;
  real_world.body_size_vec[id].radius = scaled_radius;
  b2Body *body;
  if(name == "balloon") {
    body = create_balloon_object(x, y, scaled_radius, type, id);
  }else {
    body = create_circle_object(x, y, scaled_radius, density, type, id);
  }
  real_world.physics_body_vec[id].body = body;
  return body;
}

b2Body* create_circle_object(float x, float y, int radius, float density, std::string type, int entity) {
    b2BodyDef bodyDef;
    if (type == "dynamic") {
      bodyDef.type = b2_dynamicBody;
    }else {
      bodyDef.type = b2_staticBody;
    }
    bodyDef.position.Set(x * PTOM, y * PTOM);//user data за ID
    bodyDef.linearDamping = 0.05;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(&real_world.entity_num[entity]);//new

    b2CircleShape circle_shape;
    circle_shape.m_p.Set(0, 0);
    circle_shape.m_radius = radius * PTOM;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle_shape;
    fixtureDef.density = density;
    fixtureDef.restitution = 0.2f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* create_balloon_object(int x, int y, float radius, std::string type, int entity) {
     b2BodyDef bodyDef;
    if (type == "dynamic") {
      bodyDef.type = b2_dynamicBody;
    }else {
      bodyDef.type = b2_staticBody;
    }
    bodyDef.position.Set(x * PTOM, y * PTOM);//user data за ID
    bodyDef.gravityScale = -0.7;
    bodyDef.fixedRotation = true;

    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(&real_world.entity_num[entity]);//new

    // Define another box shape for our dynamic body.
    b2CircleShape circle_shape;
    circle_shape.m_p.Set(0, 0);
    circle_shape.m_radius = radius * PTOM;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle_shape;

    fixtureDef.density = 0.35f;
    fixtureDef.restitution = 0.2f;

    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* create_ushape_entity(int x, int y, int w, int h, int vertical_height, std::string type, std::string name,
                                std::string full_name, bool sel, int color_mask, int sel_col, bool is_template, float angle,
                                int texture_mask, int selected_texture) {
  int id = createEntity();
  int mask = COMPONENT_PHYSICS_BODY | COMPONENT_BODYTYPE | RENDER_MASK | COMPONENT_NAME | SAVE_MASK | MOVE |COLLISION | BODY_DELETE;
  real_world.mask[id] = mask;
  real_world.body_type_vec[id].type = type;

  real_world.template_flags_vec[id].is_template = is_template;

  Color* col;
  col = get_colors(color_mask);
  for (int i = 0; i < 3; ++i) {
    real_world.color_vec[id].color[i] = col[i];
  }
  real_world.color_vec[id].color_mask = color_mask;
  real_world.color_vec[id].selected_color = sel_col;

  unsigned int* texture_arr = get_textures(texture_mask);
  for (int i = 0; i < 7; ++i) {
    real_world.textures_vec[id].texture_arr[i] = texture_arr[i];
  }

  real_world.textures_vec[id].texture_mask = texture_mask;
  real_world.textures_vec[id].selected_texture = selected_texture;

  real_world.coordinates_vec[id].x = x;
  real_world.coordinates_vec[id].y = y;
  real_world.coordinates_vec[id].angle = angle;

  real_world.body_size_vec[id].width = w;
  real_world.body_size_vec[id].height = h;
  real_world.body_size_vec[id].radius = 1;

  real_world.selected_vec[id].selected = sel;
  real_world.name_vec[id].name = name;
  real_world.name_vec[id].full_name = full_name;
  b2Body *body = create_ushape_object(x, y, w, h, vertical_height, type, id, angle);
  real_world.physics_body_vec[id].body = body;
  return body;

}
b2Body* create_ushape_object(int x, int y, int width, int height, int vertical_height, std::string type, int entity, float angle) {
  b2BodyDef U_shape_object;
    if (type == "dynamic") {
      U_shape_object.type = b2_dynamicBody;
    }else if (type == "kinematic"){
      U_shape_object.type = b2_kinematicBody;
    }else{
      U_shape_object.type = b2_staticBody;
    }

   b2FixtureDef myFixtureDef;
   myFixtureDef.density = 1.0f;
   myFixtureDef.friction = 0.3f;

   U_shape_object.position.Set(x* PTOM, y* PTOM);

  b2PolygonShape polygonShape;
  myFixtureDef.shape = &polygonShape;
  polygonShape.SetAsBox((width * PTOM) / 2, (height * PTOM) / 2);
  b2Body* body = world->CreateBody(&U_shape_object);
  body->SetTransform(b2Vec2(x* PTOM, y* PTOM), angle * DTOR);
  body->SetUserData(&real_world.entity_num[entity]);
  body->CreateFixture(&myFixtureDef);

  myFixtureDef.shape = &polygonShape;
  b2Vec2 left_poly_center;
  int left_width = height;
  int left_height = vertical_height;

  left_poly_center.x = -(width/2 + left_width/2) * PTOM;
  left_poly_center.y = -(left_height/2 - height / 2) * PTOM;
  polygonShape.SetAsBox((left_width * PTOM) / 2, (left_height * PTOM) / 2, left_poly_center, 0);
  body->CreateFixture(&myFixtureDef);

  myFixtureDef.shape = &polygonShape;
  b2Vec2 right_poly_center;
  right_poly_center.x = (width/2 + left_width/2) * PTOM;
  right_poly_center.y = -(left_height / 2 - height / 2) * PTOM;
  polygonShape.SetAsBox((left_width * PTOM) / 2, (left_height * PTOM) / 2, right_poly_center, 0);
  body->CreateFixture(&myFixtureDef);
  return body;
}

void create_joint_entity(int x, int y, int w, int h, float angle, int sel_colA, int sel_colB, bool is_template, int sel_texA, int sel_texB, bool sel) {
  b2Body* wrapper_body = create_box_entity(x, y, w, h, "dynamic", "moving_platform","moving_platform", false, true, false, false, MOVING_PLATFORM_MIDDLE_COLOR_MASK, sel_colA, is_template, angle, PLATFORM_TEXTURE_MASK, sel_texA);
  b2Body* core_body = create_box_entity(x, y, w * 0.78, h * 0.7, "kinematic", "moving_platform_middle", "moving_platform", sel, false, false, false, MOVING_PLATFORM_MIDDLE_COLOR_MASK, sel_colB, is_template, angle, PLATFORM_TEXTURE_MASK, sel_texB);
  b2WeldJointDef weld_joint;
  weld_joint.bodyA = wrapper_body;
  weld_joint.bodyB = core_body;
  b2Vec2 anchor_point_a(0, 0.f);
  b2Vec2 anchor_point_b(0, 0.f);
  weld_joint.localAnchorA = anchor_point_a;
  weld_joint.localAnchorB = anchor_point_b;
  world->CreateJoint(&weld_joint);
  b2Vec2 vel;
  vel.x = 1 * cos(angle * DTOR);
  vel.y = 1 * sin(angle * DTOR);
  int id = get_id(wrapper_body);

  real_world.velocity_vec[id] = vel;
  core_body->SetLinearVelocity(vel);
  qDebug() << "platform created";
}

bool create_elevator_entity(int x, int y, Color col[]) {

}

void create_motor_entity(int x, int y, float r, float g, float b) {
  // create_box_entity(x, y, 20, 20, "static", "motor", true, true, false, r, g, b);
}

#define MOTOR_MASK (COMPONENT_MOTOR)

b2Body* get_motor() {
  for(int entity = 0; entity < real_world.mask.size(); ++entity) {
    if((real_world.mask[entity] & MOTOR_MASK) == MOTOR_MASK) {
      cout<<"entity name = "<<real_world.name_vec[entity].name<<endl;
      cout<<"has platform = "<<real_world.has_platform_vec[entity].has_platform<<endl;
      if(real_world.name_vec[entity].name == "motor" && real_world.has_platform_vec[entity].has_platform == false) {
        cout<<"inside"<<endl;
        real_world.has_platform_vec[entity].has_platform = true;
        cout<<"AFTER platform = "<<real_world.has_platform_vec[entity].has_platform<<endl;
        return real_world.physics_body_vec[entity].body;
      }
    }
  }
  return NULL;
}


void create_test_elevator(int xm, int ym, int xp, int yp, float motor_speed, bool is_inverted, int m_sel_col, int p_sel_col, bool is_template, int m_sel_tex, int p_sel_tex, bool sel) {
  b2Vec2 ax(0, -1);
  b2Vec2 a(0.f, 10.f * PTOM);
  b2Vec2 b(0.f, -10.f * PTOM);//need PTOM if not 0
  b2Body* platfor;
  int low_translation  = -(abs(yp - ym));;
  if(is_inverted){
      ax.y = 1;
      a.y = -10.f * PTOM;
      b.y = 10.f * PTOM;
  }
  platfor = create_box_entity(xp, yp, 50 + ratio, 10 + ratio, "dynamic", "elavator_platform", "elevator", false, false, false, false, ELEVATOR_PLATFORM_COLOR_MASK, p_sel_col, is_template, 0, ELEVATOR_PLATFORM_TEXTURE_MASK, p_sel_tex);
  b2Body* motor = create_box_entity(xm, ym, 20 + ratio, 20 + ratio, "static", "elavator_motor", "elevator", sel, true, false, false, ELEVATOR_MOTOR_COLOR_MASK, m_sel_col, is_template, 0, ELEVATOR_MOTOR_TEXTURE_MASK, m_sel_tex);
  b2PrismaticJointDef prisJointDef;
  prisJointDef.collideConnected = false;
  prisJointDef.bodyA = motor;
  prisJointDef.bodyB = platfor;
  prisJointDef.localAnchorA = a;
  prisJointDef.localAnchorB = b;
  prisJointDef.localAxisA = ax;
  prisJointDef.enableMotor = false;
  prisJointDef.maxMotorForce = 500;
  prisJointDef.motorSpeed = motor_speed;
  prisJointDef.enableLimit = true;
  prisJointDef.upperTranslation = 0 * PTOM;
  prisJointDef.lowerTranslation = low_translation * PTOM;
  world->CreateJoint(&prisJointDef);

  int id = get_id(motor);
  real_world.inverted_vec[id].is_inverted = is_inverted;
  id = get_id(platfor);
  real_world.inverted_vec[id].is_inverted = is_inverted;
}

void create_spring(int x, int y, int px, int py, float motor_speed, float angle, int m_sel_col, int p_sel_col, bool is_template, int m_sel_tex, int p_sel_tex, bool sel) {//TODO remove x,y and make it dynamic !!
  b2Body* platform = create_box_entity(px, py, 50 + ratio, 10 + ratio, "dynamic", "spring_platform", "spring", false, false, false, false, SPRING_PLATFORM_COLOR_MASK, p_sel_col, is_template, angle, SPRING_PLATFORM_TEXTURE_MASK, p_sel_tex);
  b2Body* motor = create_box_entity(x, y, 20 + ratio, 20 + ratio, "static", "spring_motor", "spring", sel, true, false, false, SPRING_MOTOR_COLOR_MASK, m_sel_col, is_template, angle, SPRING_MOTOR_TEXTURE_MASK, m_sel_tex);
  b2PrismaticJointDef prisJointDef;
  prisJointDef.collideConnected = false;
  prisJointDef.bodyA = motor;
  prisJointDef.bodyB = platform;
  b2Vec2 ax(0, 1);
  b2Vec2 a(0.f, (-15.f - ratio) * PTOM);
  b2Vec2 b(0.f, 0.f);//need PTOM if not 0
  prisJointDef.localAnchorA = a;
  prisJointDef.localAnchorB = b;
  prisJointDef.localAxisA = ax;
  prisJointDef.enableMotor = true;
  prisJointDef.maxMotorForce = 500;
  prisJointDef.motorSpeed = motor_speed;
  prisJointDef.enableLimit = true;
  prisJointDef.upperTranslation = 0 * PTOM;
  prisJointDef.lowerTranslation = -20 * PTOM;
  world->CreateJoint(&prisJointDef);
}

void create_teleport(int ax, int ay, int bx, int by, float angleA, float angleB, int a_sel_col, int b_sel_col, bool is_template, int a_sel_tex, int b_sel_tex, bool sel) {//TODO make the distance dynaamic
  b2Body* teleportB = create_box_entity(bx, by, 10 + ratio, 30 + ratio, "static", "teleportB", "teleport", false, true, false, false, TELEPORT_B_COLOR_MASK, a_sel_col, is_template, angleB, TELEPORT_B_TEXTURE_MASK, b_sel_tex);
  b2Body* teleportA = create_box_entity(ax, ay, 10 + ratio, 30 + ratio, "static", "teleportA", "teleport", sel, true, false, false, TELEPORT_A_COLOR_MASK, b_sel_col, is_template, angleA, TELEPORT_A_TEXTURE_MASK, a_sel_tex);
  b2RopeJointDef  ropeJointDef;
  ropeJointDef.collideConnected = false;
  ropeJointDef.bodyA = teleportA;
  ropeJointDef.bodyB = teleportB;
  b2Vec2 a(0.f, 0.f);
  b2Vec2 b(0.f, 0.f);//need PTOM if not 0
  ropeJointDef.localAnchorA = a;
  ropeJointDef.localAnchorB = b;
  ropeJointDef.maxLength = 8;

   world->CreateJoint(&ropeJointDef);
}

void create_fan(int x, int y, bool is_inverted, int butt_sel_col, int fan_sel_col, int body_sel_col, bool is_template, int butt_sel_tex, int fan_sel_tex, int body_sel_tex, bool sel) {
  cout<<"---Creating fan---"<< endl;
  b2Body* bodies[3];
  int body_width = 10 + ratio;
  int body_height = 60 + ratio;
  int fan_width = body_height;
  int fan_height = body_height / 2;
  int button_width = body_width / 2;
  int button_height = body_width / 2;
  cout<<"---Creating body of the fan---"<< endl;
  b2Body* body = create_box_entity(x, y, body_width, body_height, "static", "fan_body", "fan", sel, false, false, false, FAN_BODY_COLOR_MASK, body_sel_col, is_template, 0, FAN_BODY_TEXTURE_MASK, body_sel_tex);
  cout<<"---Creating button of the fan---"<< endl;
  b2Body* button = create_box_entity(x, (y -1) - (body_height / 2 + button_height / 2), button_width, button_height, "static", "fan_button", "fan", false, false, false, false, FAN_BUTTON_COLOR_MASK, butt_sel_col, is_template, 0, FAN_BUTTON_TEXTURE_MASK, butt_sel_tex);

  b2Body* fan;
  if(is_inverted) {
      cout<<"---Creating wind (reversed) of the fan---"<< endl;
      fan = create_box_entity(1 + x + (body_width/2 + fan_width / 2), y - (body_height / 4), fan_width, fan_height, "static", "fan", "fan", false, true, true, false, FAN_COLOR_MASK, fan_sel_col, is_template, 0, FAN_WIND_TEXTURE_MASK, fan_sel_tex);
      real_world.inverted_vec[get_id(fan)].is_inverted = true;
      real_world.inverted_vec[get_id(body)].is_inverted = true;
      real_world.inverted_vec[get_id(button)].is_inverted = true;
      real_world.fan_vec[get_id(fan)].is_reversed = true;
      real_world.fan_vec[get_id(body)].is_reversed = true;
      real_world.fan_vec[get_id(button)].is_reversed = true;
  }else {
      cout<<"---Creating wind of the fan---"<< endl;
      fan = create_box_entity(x - ((body_width/2 + fan_width / 2) + 1), y - (body_height / 4), fan_width, fan_height, "static", "fan", "fan", false, true, true, false, FAN_COLOR_MASK, fan_sel_col, is_template, 0, FAN_WIND_TEXTURE_MASK, fan_sel_tex);
  }
  bodies[0] = body;
  bodies[1] = fan;
  bodies[2] = button;
  int body_id =  get_id(body);
  int fan_id =  get_id(fan);
  int button_id =  get_id(button);
  for (int i = 0; i < 3; ++i) {
    int id = get_id(bodies[i]);
    real_world.fan_vec[id].fan_body_id = body_id;
    real_world.fan_vec[id].fan_id = fan_id;
    real_world.fan_vec[id].fan_button_id = button_id;
  }

  b2WeldJointDef body_fan_joint;
  body_fan_joint.collideConnected = false;
  body_fan_joint.bodyA = body;
  body_fan_joint.bodyB = fan;
  b2Vec2 a(0.f, 0.f);//works because the bodies are static
  b2Vec2 b(0.f, 0.f);//need PTOM if not 0
  body_fan_joint.localAnchorA = a;
  body_fan_joint.localAnchorB = b;
  world->CreateJoint(&body_fan_joint);

  b2WeldJointDef body_button_joint;
  body_button_joint.collideConnected = false;
  body_button_joint.bodyA = body;
  body_button_joint.bodyB = button;
  body_button_joint.localAnchorA = a;
  body_button_joint.localAnchorB = b;
  world->CreateJoint(&body_button_joint);

  b2WeldJointDef fan_button_joint;
  fan_button_joint.collideConnected = false;
  fan_button_joint.bodyA = fan;
  fan_button_joint.bodyB = button;
  fan_button_joint.localAnchorA = a;
  fan_button_joint.localAnchorB = b;
  world->CreateJoint(&fan_button_joint);
  cout<< "end of fan create" << endl;
}


int get_id(b2Body* body) {
  void* user_data = body->GetUserData();
  int* id = static_cast<int*>(user_data);
  return *id;
}

void create_domino(int x, int y, float angle, int sel_col, bool is_template, int sel_tex, bool sel) {
  create_box_entity(x, y, 1 + ratio, 40 + ratio, "dynamic", "domino", "domino", sel, false, false, true, DOMINO_COLOR_MASK, sel_col, is_template, angle, DOMINO_TEXTURE_MASK, sel_tex);
}

void create_balloon(int x, int y, int p_sel_col, int b_sel_col, bool is_template) {
  b2Body* balloon = create_circle_entity(x, y, 25, 0.3, "dynamic", "balloon", "balloon", false, TELEPORT_A_COLOR_MASK, b_sel_col, is_template, TELEPORT_A_COLOR_MASK, b_sel_col);
  b2Body* platform = create_box_entity(x, y + 18, 30, 5, "dynamic", "balloon_platform", "balloon", false, false, false, true, TELEPORT_B_COLOR_MASK, p_sel_col, is_template, 0, TELEPORT_B_COLOR_MASK, 0);

  b2RopeJointDef  ropeJointDef;
  ropeJointDef.collideConnected = true;
  ropeJointDef.bodyA = balloon;
  ropeJointDef.bodyB = platform;
  b2Vec2 a(-25 * PTOM, 0.f * PTOM);
  b2Vec2 b(-15.f * PTOM, 0.f);//need PTOM if not 0
  ropeJointDef.localAnchorA = a;
  ropeJointDef.localAnchorB = b;
  ropeJointDef.maxLength = 100 * PTOM;
  world->CreateJoint(&ropeJointDef);

  a = b2Vec2(25 * PTOM, 0.f * PTOM);
  b = b2Vec2(15.f * PTOM, 0.f);//need PTOM if not 0
  ropeJointDef.localAnchorA = a;
  ropeJointDef.localAnchorB = b;
  ropeJointDef.maxLength = 100 * PTOM;

   world->CreateJoint(&ropeJointDef);
}

void create_wrecking_ball(int px, int py, int wx, int wy, float rope_lenght, int p_sel_col, int b_sel_col, bool is_template, int p_sel_tex, int b_sel_tex, bool sel) {
  b2Body* ball = create_circle_entity(wx, wy, 5 + ratio, 5, "dynamic", "wrecking_ball", "wrecking_ball", false, WRECKING_BALL_COLOR_MASK, b_sel_col, is_template, SPRING_PLATFORM_TEXTURE_MASK, b_sel_tex);
  b2Body* wrecking_ball_platform = create_box_entity(px, py, 10 + ratio, 10 + ratio, "static", "wrecking_ball_platform", "wrecking_ball", sel, false, false, false, TELEPORT_B_COLOR_MASK, p_sel_col, is_template, 0, SPRING_PLATFORM_TEXTURE_MASK, p_sel_tex);
  ball->SetFixedRotation(true);
  b2RopeJointDef  ropeJointDef;
  ropeJointDef.collideConnected = true;
  ropeJointDef.bodyA = wrecking_ball_platform;
  ropeJointDef.bodyB = ball;
  b2Vec2 a(0.f, 0.f);
  b2Vec2 b(0.f, 0.f);//need PTOM if not 0
  ropeJointDef.localAnchorA = a;
  ropeJointDef.localAnchorB = b;
  ropeJointDef.maxLength = rope_lenght * PTOM;

   world->CreateJoint(&ropeJointDef);
}

void create_catapult(int x, int y, bool is_inverted, int l_sel_col, int c_sel_col, bool is_template, int l_sel_tex, int c_sel_tex, bool sel) {
  b2Vec2 a(5.f * PTOM, 0.f);
  b2Vec2 b(0.f, (- ratio) * PTOM);//need PTOM if not 0
   b2Body* catapult_platform;
  if(is_inverted){
      catapult_platform = create_box_entity(x, y + ratio, ratio, 10 + ratio, "static", "catapult_platform", "catapult", false, false, false, false, TELEPORT_B_COLOR_MASK, c_sel_col, is_template, 0, CATAPULT_PLATFORM_TEXTURE_MASK, c_sel_tex);
      a.x *= -1;
  }else{
      catapult_platform = create_box_entity(x, y + ratio, ratio, 10 + ratio, "static", "catapult_platform", "catapult", false, false, false, false, TELEPORT_B_COLOR_MASK, c_sel_col, is_template, 0, CATAPULT_PLATFORM_TEXTURE_MASK, c_sel_tex);
  }
  //b2Body* catapult_lever = create_ushape_entity( x, y, 80, 5 + ratio, 10 + ratio, "dynamic", "catapult_lever", "catapult", sel, WRECKING_BALL_COLOR_MASK, l_sel_col, is_template, 0, CATAPULT_TEXTURE_MASK, l_sel_tex);
  b2Body* catapult_lever = create_box_entity(x, y, 120, ratio - 8, "dynamic", "catapult_lever", "catapult", sel, false, false, true, WRECKING_BALL_COLOR_MASK, l_sel_col, is_template, 0, CATAPULT_PLATFORM_TEXTURE_MASK, l_sel_tex);
  int id = get_id(catapult_lever);
  real_world.inverted_vec[id].is_inverted = is_inverted;
  id = get_id(catapult_platform);
  real_world.inverted_vec[id].is_inverted = is_inverted;
  b2RevoluteJointDef revoluteJointDef;
  revoluteJointDef.collideConnected = false;
  revoluteJointDef.bodyA = catapult_lever;
  revoluteJointDef.bodyB = catapult_platform;
  revoluteJointDef.localAnchorA = a;
  revoluteJointDef.localAnchorB = b;

   world->CreateJoint(&revoluteJointDef);
}

void create_templates(int screen_width) {
    int offset = 50;
    int sel_col = 0;
    int width = 40 + ratio;//ration = 10
    int height = 40 + ratio;
    int sel_tex = 6;
    int x =screen_width - offset;
    int y = 100 + ratio;
    int spacer = 10;

    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_wood", "template_obstecle_wood", false, true, false, false, WOOD_COLOR_MASK, sel_col, false, 0, WOOD_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_circle", "template_obstecle_circle", false, true, false, false, CIRCLE_COLOR_MASK, sel_col, false, 0, CIRCLE_TEXURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_moving_platform", "template_obstecle_moving_platform", false, true, false, false, MOVING_PLATFORM_MIDDLE_COLOR_MASK, sel_col,  false, 0, PLATFORM_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_spring", "template_obstecle_spring", false, true, false, false, SPRING_PLATFORM_COLOR_MASK, sel_col, false, 0, SPRING_MOTOR_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_teleport", "template_obstecle_teleport", false, true, false, false, TELEPORT_A_COLOR_MASK, sel_col, false, 0, TELEPORT_A_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_fan", "template_obstecle_fan", false, true, false, false, FAN_COLOR_MASK, sel_col, false, 0, FAN_BUTTON_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_domino", "template_obstecle_domino", false, true, false, false, DOMINO_COLOR_MASK, sel_col, false, 0, DOMINO_TEXTURE_MASK, sel_tex);
    x = x - width - spacer;
    y = 100 + ratio;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_wrecking_ball", "template_obstecle_wrecking_ball", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, SPRING_PLATFORM_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_catapult", "template_obstecle_catapult", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, CATAPULT_TEXTURE_MASK, sel_tex);
    y = y + spacer + height;
    create_box_entity(x, y, width, height, "kinematic", "template_obstecle_goal", "template_obstecle_goal", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, GOAL_TEXTURE_MASK, sel_tex);
    // create_circle_entity(screen_width - offset, 130 + ratio, 5 + ratio, 1 + ratio, "kinematic", "template_obstecle_circle", "template_obstecle_circle", false, CIRCLE_COLOR_MASK, 0, false, CIRCLE_TEXURE_MASK, 0);
    //create_box_entity(screen_width - offset, 160 + ratio, 50 + ratio, 8 + ratio, "kinematic", "template_obstecle_moving_platform", "template_obstecle_moving_platform", false, true, false, false, MOVING_PLATFORM_MIDDLE_COLOR_MASK, sel_col,  false, 0, PLATFORM_TEXTURE_MASK, sel_tex);

//    create_box_entity(screen_width - offset, 200 + ratio, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_elevator", "template_obstecle_elevator", false, true, false, false, ELEVATOR_PLATFORM_COLOR_MASK, sel_col, false, 0, ELEVATOR_PLATFORM_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 222 + ratio*2, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_spring", "template_obstecle_spring", false, true, false, false, SPRING_PLATFORM_COLOR_MASK, sel_col, false, 0, SPRING_PLATFORM_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 244 + ratio*3, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_teleport", "template_obstecle_teleport", false, true, false, false, TELEPORT_A_COLOR_MASK, sel_col, false, 0, TELEPORT_A_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 266 + ratio*4, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_fan", "template_obstecle_fan", false, true, false, false, FAN_COLOR_MASK, sel_col, false, 0, FAN_WIND_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 298 + ratio*5, 6 + ratio, 30 + ratio, "kinematic", "template_obstecle_domino", "template_obstecle_domino", false, true, false, false, DOMINO_COLOR_MASK, sel_col, false, 0, DOMINO_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 330 + ratio*6, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_wrecking_ball", "template_obstecle_wrecking_ball", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, SPRING_PLATFORM_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 352 + ratio*7, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_catapult", "template_obstecle_catapult", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, CATAPULT_TEXTURE_MASK, sel_tex);
//    create_box_entity(screen_width - offset, 374 + ratio*8, 20 + ratio, 20 + ratio, "kinematic", "template_obstecle_goal", "template_obstecle_goal", false, true, false, false, WRECKING_BALL_COLOR_MASK, sel_col, false, 0, GOAL_TEXTURE_MASK, sel_tex);
//    std::cout<<"Global id = "<<global_id<<endl;
}




int scale_width(float size) {
    QDesktopWidget dw;
    QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
    int w = mainScreenSize.width() * 0.6;
    //MainWindow m;
    float x_scale_factor = size/defalt_screen_width;
    int result = x_scale_factor * w;
    return result;
}

int scale_height(float size) {
   // MainWindow m;
    QDesktopWidget dw;
    QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
    int h = mainScreenSize.height() * 0.8;
    float y_scale_factor = size/defalt_screen_height;
    int result = y_scale_factor * h;
    return result;
}
