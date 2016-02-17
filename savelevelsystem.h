#ifndef SAVELEVELSYSTEM
#define SAVELEVELSYSTEM
#include "Box2D/Box2D.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include <string>
#include <QString>

using namespace rapidjson;

struct SaveComponets{
    std::string name;
    std::string type;
    b2Vec2 coordinates;
    b2Vec2 size;
    int radius;
    double angle;
    bool fan_is_reversed;
    bool is_inverted;
    bool is_visible;
    bool is_sensor;
    bool has_gravity;
    int color_mask;
    bool is_template;
    int quantity;
    std::string full_name;
    int selected_color;
    int texture_mask;
    int selected_texture;

};

int SaveLevelSystem(QString file_name, bool from_play_button);
int SaveLevelToFile(std::string content, QString file_name);
char* CreateCharFromString(std::string str);
void fill_component(int entity);
void reset_entity_masks();
int check_file_name(QString file_name);
bool is_file_name_valid(QString file_name);
void change_quatity(std::string name, bool is_increasing);
bool is_entity_valid(int entity);
void remove_file(QString file_name);
void init_quantity();
double percent_convert(int num, int resolution);

#endif // SAVELEVELSYSTEM

