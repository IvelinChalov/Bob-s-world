#include "savelevelsystem.h"
#include "component.h"
#include "errorsystem.h"
#include <iostream>
#include "entity.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/rapidjson.h"
#include "component.h"
#include <fstream>
#include <cmath>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QRegularExpression>
#include <iostream>
#include <QDesktopWidget>
#include "mainwindow.h"

//IF PROBLEM WITH MASK IS FRIM HERE

using namespace std;

SaveComponets components;

template <typename Writer>
void SaveEntity(Writer& writer) {
    std::cout<<"in SaveEntity \n";
    writer.StartObject();
    writer.String("name");
    writer.String(CreateCharFromString(components.name));
    writer.String("type");
    writer.String(CreateCharFromString(components.type));
    //writer.String("coordinates");
   // writer.StartArray();
      //  writer.StartObject();
        writer.String("x");
        //writer.Int(components.coordinates.x);
        MainWindow m;
        writer.Double(percent_convert(components.coordinates.x, m.get_screen_width()));
        writer.String("y");
        writer.Double(percent_convert(components.coordinates.y, m.get_screen_height()));
        //writer.Int(components.coordinates.y);
    //    writer.EndObject();
   // writer.EndArray();
    std::cout<<"end first arr in saveentity \n";
   // writer.String("size");
   // writer.StartArray();
   //     writer.StartObject();
        writer.String("width");
        writer.Int(components.size.x);
        //writer.Double(percent_convert(components.size.x, m.get_screen_width()));
        writer.String("height");
        //writer.Double(percent_convert(components.size.y, m.get_screen_height()));
        writer.Int(components.size.y);
    //    writer.EndObject();
   // writer.EndArray();
    writer.String("radius");
   // writer.Double(percent_convert(components.radius, m.get_screen_width()));
    writer.Int(components.radius);
    writer.String("angle");
    writer.Double(components.angle);
    writer.String("color_mask");
    writer.Int(components.color_mask);
    writer.String("fan_is_reversed");
    writer.Bool(components.fan_is_reversed);
    writer.String("is_reversed");
    writer.Bool(components.is_inverted);
    writer.String("is_sensor");
    writer.Bool(components.is_sensor);
    writer.String("is_invisible");
    writer.Bool(components.is_visible);
    writer.String("has_gravity");
    writer.Bool(components.has_gravity);
    writer.String("quantity");
    writer.Int(components.quantity);
    writer.String("is_template");
    writer.Bool(components.is_template);
    writer.String("full_name");
    writer.String(CreateCharFromString(components.full_name));
    writer.String("selected_color");
    writer.Int(components.selected_color);
    writer.String("texture_mask");
    writer.Int(components.texture_mask);
    writer.String("selected_texture");
    writer.Int(components.selected_texture);
    writer.EndObject();
     std::cout<<"end  saveentity \n";
}


#define SAVE_LEVEL_SYSTEM_MASK (SAVE_MASK)
int SaveLevelSystem(QString file_name, bool from_play_button) {
        int mask = SAVE_MASK;
        if(from_play_button){
            mask = COMPONENT_PHYSICS_BODY;
        }

        std::cout<<"start of a SaveLevelSystem \n";
        StringBuffer s;
        std::vector<int> entity_components_id;
        PrettyWriter<StringBuffer> writer(s);
        writer.StartObject();
        //std::cout<<"ok\n";
        writer.String("world");
        ///std::cout<<"ok\n";
        writer.StartArray();
        for(int entity = 0; entity < real_world.mask.size(); ++entity) {
            string a = real_world.name_vec[entity].full_name;
            int c_mask = real_world.mask[entity];
            int sum = real_world.mask[entity] & mask;
            if(((real_world.mask[entity] & mask) == mask) && ((real_world.mask[entity] & ALREADY_CHECKED) != ALREADY_CHECKED)) {
                cout<< "Name = " << real_world.name_vec[entity].name << endl;
                b2JointEdge* j = real_world.physics_body_vec[entity].body->GetJointList();
                if(j) {
                    for (j; j; j = j->next) {
                      int id = get_id(j->other);
                      entity_components_id.push_back(id);
                    }
                    entity_components_id.push_back(entity);
                    writer.StartArray();
                    for(int i = 0; i < entity_components_id.size(); ++i) {
                        fill_component(entity_components_id[i]);
                        SaveEntity(writer);
                    }
                    entity_components_id.clear();
                    writer.EndArray();
                }else {
                    fill_component(entity);
                    SaveEntity(writer);
                }
            }
        }
        writer.EndArray();
        writer.EndObject();
        std::cout<<"end of a SaveLevelSystem \n";
        //return s;
        std::string content = s.GetString();
        int error_code = SaveLevelToFile(content, file_name);
        reset_entity_masks();
        return error_code;
}

int SaveLevelToFile(std::string content, QString file_name) {
    ofstream myfile;
    int error_code = check_file_name(file_name);
     if(error_code != NO_ERROR)return error_code;
    std::string file_name_buff = file_name.toLocal8Bit().constData();
    std::string file_path = "levels/" + file_name_buff + ".txt";
    myfile.open(file_path.c_str());
    if (myfile.is_open()) {
        myfile << content;
        myfile.close();
        error_code = NO_ERROR;
        return error_code;
    }else {
        error_code = WRONG_CHARECTER_ERROR;
        return error_code;
        qDebug() <<"Unable to open file";
    }
}

char *CreateCharFromString(string str) {
    char* result = new char [str.length()+1];
    std::strcpy(result,str.c_str());
    return result;
}

void fill_component(int entity) {
    components.name = real_world.name_vec[entity].name;
    components.type = real_world.body_type_vec[entity].type;
    components.coordinates.x = real_world.physics_body_vec[entity].body->GetPosition().x * MTOP;
    components.coordinates.y = real_world.physics_body_vec[entity].body->GetPosition().y * MTOP;
    components.size.x = real_world.body_size_vec[entity].width;
    components.size.y = real_world.body_size_vec[entity].height;
    components.radius = real_world.body_size_vec[entity].radius;
    components.angle = (double)real_world.coordinates_vec[entity].angle;
    components.color_mask = real_world.color_vec[entity].color_mask;
    components.fan_is_reversed = real_world.fan_vec[entity].is_reversed;
    std::cout << "CHANGE from save " << components.fan_is_reversed << " " << real_world.name_vec[entity].name<<endl;
    components.is_inverted = real_world.inverted_vec[entity].is_inverted;
    components.is_visible = real_world.entity_flags_vec[entity].is_visible;
    components.is_sensor = real_world.entity_flags_vec[entity].is_sensor;
    components.has_gravity = real_world.entity_flags_vec[entity].has_gravity;
    components.quantity = real_world.quantity[entity];
    components.is_template = real_world.template_flags_vec[entity].is_template;
    components.full_name = real_world.name_vec[entity].full_name;
    components.selected_color = real_world.color_vec[entity].selected_color;
    components.texture_mask = real_world.textures_vec[entity].texture_mask;
    components.selected_texture = real_world.textures_vec[entity].selected_texture;
    real_world.mask[entity] = real_world.mask[entity] | ALREADY_CHECKED;
}

void reset_entity_masks() {
   for(int entity = 0; entity < real_world.mask.size(); ++entity) {
       if(((real_world.mask[entity] & SAVE_MASK) == SAVE_MASK)) {
           real_world.mask[entity] = real_world.mask[entity] - ALREADY_CHECKED;
       }
   }
}

void change_quatity(std::string name, bool is_increasing) {
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
        if(real_world.mask[entity] != COMPONENT_NONE && name == real_world.name_vec[entity].full_name) {
            if(is_increasing){
                real_world.quantity[entity]++;
                cout<<"inside of increase \n";
                cout<< "before == " << real_world.mask[entity]<< endl;
                if( ((real_world.mask[entity] & SAVE_MASK)!= SAVE_MASK))real_world.mask[entity] = real_world.mask[entity] + SAVE_MASK;
                cout<< "mask == " << real_world.mask[entity]<<endl;
            }else{
                real_world.quantity[entity]--;
                cout<<"inside of decrease \n";
                cout<< "before == " << real_world.mask[entity]<< endl;
                if( real_world.quantity[entity] == 0)real_world.mask[entity] = real_world.mask[entity] - SAVE_MASK;
                cout<< "mask == " << real_world.mask[entity]<< endl;
            }
            cout << "quntity of "<< real_world.name_vec[entity].name << " = " << real_world.quantity[entity] <<endl;
        }
    }
}


bool is_entity_valid(int entity) {
//    std::string name = real_world.name_vec[entity].full_name;
//    if((name.find("template") != std::string::npos) && real_world.quantity > 0){
//        return true;
//    }
//    if(((real_world.mask[entity] & COMPONENT_PHYSICS_BODY) == COMPONENT_PHYSICS_BODY) && ((real_world.mask[entity] & ALREADY_CHECKED) != ALREADY_CHECKED)) {
//         return true;
//    }

//    return false;
}

int check_file_name(QString file_name) {
    QString current = QCoreApplication::applicationDirPath();
    QString path = current + "/levels/" + file_name + ".txt";
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isFile()) {
        return FILE_EXISTS_ERROR;
    } else if(file_name.length() == 0){
        return NO_NAME_ERROR;
    }else {
        return NO_ERROR;
    }
}

bool is_file_name_valid(QString file_name) {
    QString str_ex = "^[: \\ \ / \* \? \" ' < > | ]$";
    QRegularExpression ex(str_ex);
    bool result = file_name.contains(ex);
    qDebug() << "result = " <<result;
    return !result;
}

void remove_file(QString file_name)
{
    QString current = QCoreApplication::applicationDirPath();
    QString path = current + "/levels/" + file_name + ".txt";
    QFile file(path);
    if(file.exists()){
        file.close();
        file.remove();
    }
}

void init_quantity() {
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
        if(real_world.mask[entity] != COMPONENT_NONE && real_world.name_vec[entity].full_name.find("template") == std::string::npos && ((real_world.mask[entity] & ALREADY_CHECKED) != ALREADY_CHECKED)) {
           if(real_world.template_flags_vec[entity].is_template){
                std::string name = "template_obstecle_" + real_world.name_vec[entity].full_name;
                change_quatity(name, true);
                real_world.mask[entity] = real_world.mask[entity] | ALREADY_CHECKED;
                b2JointEdge* j = real_world.physics_body_vec[entity].body->GetJointList();
                for (j; j; j = j->next) {
                    b2Body* other = j->other;
                    int id = get_id(other);
                    real_world.mask[id] = real_world.mask[entity] | ALREADY_CHECKED;
                }
           }else{
               real_world.mask[entity] = real_world.mask[entity] | ALREADY_CHECKED;
           }
        }
    }
    //reset_entity_masks();
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
        if(((real_world.mask[entity] & ALREADY_CHECKED) == ALREADY_CHECKED)) {
            real_world.mask[entity] = real_world.mask[entity] - ALREADY_CHECKED;
        }
    }
}

double percent_convert(int num, int resolution) {
    double d_num = num;
    double d_resolution = resolution;
    double result = d_num/d_resolution;
    cout<<"-----Save level system-----" << endl;
    cout<< "radius is = " << result << endl;
    return result;
}
