#ifndef ENTITY
#define ENTITY

#include <Box2D/Box2D.h>
#include <string>
#include <vector>
#include "Component.h"
#include <QOpenGLTexture>
#include <QString>

#define ENTITY_COUNT 100

//convertion constants
const float MTOP = 100;
const float PTOM = 1 / MTOP;
const float DTOR = M_PI/180.0;
const float RTOD = 180.0/M_PI;
const float defalt_screen_width = 819;
const float defalt_screen_height = 582;

extern int global_id;
extern bool pause;
//extern int screen_width;
//extern int screen_height;
extern int error_count;//for debug


extern b2World* world;

struct RealWorld{
    std::vector<int> mask;
    int entity_num[500];//TODO Make check for array out of boundaries
    std::vector<Motor> has_platform_vec;
    std::vector<Coordinates> coordinates_vec;
    std::vector<PhysicsBody> physics_body_vec;
    std::vector<Name> name_vec;
    std::vector<BodyType> body_type_vec;
    std::vector<BodySize> body_size_vec;
    std::vector<Selected> selected_vec;
    std::vector<b2Vec2> velocity_vec;
    std::vector<Colors> color_vec;
    std::vector<Teleportation> teleportation_vec;
    std::vector<EntityFanFlags> ent_flag_vec;
    std::vector<JointLenght> joint_vec;//TODO remove ?
    std::vector<Fan> fan_vec;
    std::vector<Inverted> inverted_vec;
    std::vector<EntityFlags> entity_flags_vec;
    std::vector<TemplateFlag> template_flags_vec;
    std::vector<Textures> textures_vec;
    std::vector<int> quantity;
    std::vector<int> template_id;
    std::vector<Sticky> sticky_id;
    std::vector<BodyDelete> for_delete_vec;
};
extern RealWorld real_world;
extern bool create;

int createEntity();
void init_entity(int id);

void destroyEntity(unsigned int entity);

b2Body* create_box_entity(int x, int y, int w, int h, std::string type, std::string name, std::string full_name, bool sel, bool is_sensor, bool is_invisible, bool has_gravity, int color_mask, int selected_color, bool is_template, float angle, int texture_mask, int selected_texture);
b2Body* create_box_object(int x, int y, int width, int height, std::string type, int entity, bool is_sensor, bool has_gravity, float angle);

b2Body* create_circle_entity(float x, float y, int radius, float density, std::string type, std::string name, std::string full_name, bool sel, int color_mask, int sel_col, bool is_template, int texture_mask, int sel_texure);
b2Body* create_circle_object(float x, float y, int radius, float density, std::string type, int entity);

b2Body* create_ushape_entity(int x, int y, int w, int h, int vertical_height, std::string type, std::string name, std::string full_name, bool sel, int color_mask, int sel_col, bool is_template, float angle, int texture_mask, int selected_texture);
b2Body* create_ushape_object(int x, int y, int width, int height, int vertical_height, std::string type, int entity, float angle);

void create_joint_entity(int x, int y, int w, int h, float angle, int sel_colA, int sel_colB, bool is_template, int sel_texA, int sel_texB, bool sel);
bool create_elevator_entity(int x, int y, float r, float g, float b);
b2Body* get_motor();
void create_motor_entity(int x, int y, float r, float g, float b);

void create_test_elevator(int xm, int ym, int xp, int yp, float motor_speed, bool is_inverted, int m_sel_col, int p_sel_col, bool is_template, int m_sel_tex, int p_sel_tex, bool sel);
void create_spring(int x, int y, int px, int py, float motor_speed, float angle, int m_sel_col, int p_sel_col, bool is_template, int m_sel_tex, int p_sel_tex, bool sel);
void create_teleport(int ax, int ay, int bx, int by, float angleA, float angleB, int a_sel_col, int b_sel_col, bool is_template, int a_sel_tex, int b_sel_tex, bool sel);
void create_fan(int x, int y, bool is_inverted, int butt_sel_col, int fan_sel_col, int body_sel_col, bool is_template, int butt_sel_tex, int fan_sel_tex, int body_sel_tex, bool sel);
int get_id(b2Body* body);
void create_domino(int x, int y, float angle, int sel_col, bool is_template, int sel_tex, bool sel);
void create_balloon(int x, int y, int p_sel_col, int b_sel_col, bool is_template);
b2Body* create_balloon_object(int x, int y, float radius, std::string type, int entity);
void create_wrecking_ball(int px, int py, int wx, int wy, float rope_lenght, int p_sel_col, int b_sel_col, bool is_template, int p_sel_tex, int b_sel_tex, bool sel);
void create_catapult(int x, int y, bool is_inverted, int l_sel_col, int c_sel_col, bool is_template, int l_sel_tex, int c_sel_tex, bool sel);
void create_templates(int screen_width);

int scale_width(float size);
int scale_height(float size);

#endif // ENTITY

