#ifndef LOADLEVELSYSTEM
#define LOADLEVELSYSTEM
#include <string>
#include <vector>
#include <Box2D/Box2D.h>
#include <QString>

using namespace std;
void load_level_system(QString file_name, bool from_restart_btn);

template <typename T>
string NumberToString ( T Number );
string bool_to_string(bool b);
bool string_to_bool(string str);
void create_element(std::vector<string> arr);
void add_move_mask();
int generate_coordinates(double percent, int resolution);
#endif // LOADLEVELSYSTEM

