#include "loadlevelsystem.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "glwidget.h"
#include "Entity.h"
#include "mainwindow.h"
#include <cstdio>
#include <sstream>
#include <iostream>
#include <string>
#include <QDebug>
#include <error.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <bitset>

using namespace rapidjson;
using namespace std;

template <typename T>
T StringToNumber ( const string &Text )//Text not by const reference so that the function can be used with a
{                               //character array as argument
    stringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

void load_level_system(QString file_name, bool from_restart_btn) {
    cout<<"Entering load level system"<<endl;
    std::vector<string> curr_entity;
    file_name = "levels/" + file_name + ".txt";
    const char* file_char = file_name.toStdString().c_str();
    cout<<"char* = " << file_char<<endl;
    std::cout<< file_char << "<------ from load level \n";
    errno = 0;
    FILE* fp = fopen(file_char, "rb"); // non-Windows use "r"
    if(fp != NULL){
        cout<< "EROOR NUM: " << errno << endl;
        cout<< "EROOR TEXT: " << strerror(errno) << endl;
        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        Document d;
        d.ParseStream(is);

        const Value& a = d["world"];
        assert(a.IsArray());
        for (SizeType i = 0; i < a.Size(); i++) {
            if(a[i].IsObject()){
                //If enters here is a simple object
                for (Value::ConstMemberIterator itr = a[i].MemberBegin();itr != a[i].MemberEnd(); ++itr){
                    if(itr->value.IsString()) {
                        curr_entity.push_back(itr->value.GetString());
                    }else if(itr->value.IsInt()) {
                         curr_entity.push_back(NumberToString(itr->value.GetInt()));
                    }else if(itr->value.IsDouble()) {
                        itr->value.GetDouble();
                         curr_entity.push_back(NumberToString(itr->value.GetDouble()));
                    }else if(itr->value.IsBool()) {
                         curr_entity.push_back(bool_to_string(itr->value.GetBool()));
                    }
                }
            if((load_from_editor) || from_restart_btn || (!load_from_editor && curr_entity[15] == "false" && curr_entity[14] != "0")){
                create_element(curr_entity);
            }
            curr_entity.clear();
            }else if(a[i].IsArray()){
               //If enters here is a compound object
               for (SizeType j = 0; j < a[i].Size(); j++) {
                   cout<< "size of the array is: " <<a[i].Size()<<endl;
                   if(a[i][j].IsObject()){
                       for (Value::ConstMemberIterator itr = a[i][j].MemberBegin();itr != a[i][j].MemberEnd(); ++itr){
                           //qDebug() << itr->name.GetString();
                           if(itr->value.IsString()){
                               curr_entity.push_back(itr->value.GetString());
                           }else if(itr->value.IsInt()) {
                               curr_entity.push_back(NumberToString(itr->value.GetInt()));
                           }else if(itr->value.IsDouble()) {
                               curr_entity.push_back(NumberToString(itr->value.GetDouble()));
                           }else if(itr->value.IsBool()) {
                               curr_entity.push_back(bool_to_string(itr->value.GetBool()));
                           }
                       }
                   }
               }
               if((load_from_editor) || from_restart_btn || (!load_from_editor && curr_entity[15] == "false" && curr_entity[14] != "0")){
                   create_element(curr_entity);
               }
               curr_entity.clear();
            }
        }
        fclose(fp);
    }else{
        QMessageBox messageBox;
        messageBox.setFixedSize(500,200);
        messageBox.setWindowTitle("Message");
        messageBox.setText("Problem with loading the file!!");
        messageBox.exec();
    }
}

template <typename T>
string NumberToString(T Number) {
    stringstream ss;
    ss << Number;
    return ss.str();
}

string bool_to_string(bool b) {
    if(b){
        return "true";
    }else {
        return "false";
    }
}

bool string_to_bool(string str) {
    if(str == "true"){
        return true;
    }else {
        return false;
    }
}

void create_element(std::vector<string> arr) {

    QDesktopWidget dw;
    QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
    int w = mainScreenSize.width() * 0.6;
    int h = mainScreenSize.height() * 0.8;

    int offset = 20;
    MainWindow m;
    if(arr[0].find("template") != std::string::npos) {
        b2Body* body = create_box_entity(generate_coordinates(StringToNumber<double>(arr[2]), w), generate_coordinates(StringToNumber<double>(arr[3]), h), StringToNumber<int>(arr[4]), StringToNumber<int>(arr[5]),
                arr[1], arr[0],arr[16], false, string_to_bool(arr[11]), string_to_bool(arr[12]),
                        string_to_bool(arr[13]), StringToNumber<int>(arr[8]), StringToNumber<int>(arr[17]), string_to_bool(arr[15]), StringToNumber<float>(arr[7]), StringToNumber<int>(arr[18]), StringToNumber<int>(arr[19]));
        int id = get_id(body);
        real_world.quantity[id] = StringToNumber<int>(arr[14]);
    }else if(arr[0] == "wood") {
        create_box_entity(generate_coordinates(StringToNumber<double>(arr[2]), w), generate_coordinates(StringToNumber<double>(arr[3]), m.get_screen_height()), StringToNumber<int>(arr[4]), StringToNumber<int>(arr[5]),
                arr[1], arr[0],arr[16], false, string_to_bool(arr[11]), string_to_bool(arr[12]),
                string_to_bool(arr[13]), StringToNumber<int>(arr[8]),StringToNumber<int>(arr[17]), string_to_bool(arr[15]), StringToNumber<float>(arr[7]), StringToNumber<int>(arr[18]), StringToNumber<int>(arr[19]));

    }else if(arr[0] == "circle") {
        create_circle_entity(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), m.get_screen_height()),StringToNumber<int>(arr[6]),
                1, arr[1], arr[0], arr[16], false, StringToNumber<int>(arr[8]), StringToNumber<int>(arr[17]), string_to_bool(arr[15]), StringToNumber<int>(arr[18]), StringToNumber<int>(arr[19]));
    }else if(arr[0] == "moving_platform_middle") {
        create_joint_entity(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), m.get_screen_height()),  generate_coordinates(StringToNumber<double>(arr[4 + offset]), m.get_screen_width()), generate_coordinates(StringToNumber<double>(arr[5 + offset]), m.get_screen_height()),
                StringToNumber<float>(arr[7]),StringToNumber<int>(arr[17]), StringToNumber<int>(arr[17 + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19 + offset]), false);
    }else if(arr[0] == "elavator_motor") {
        create_test_elevator(StringToNumber<int>(arr[2]), StringToNumber<int>(arr[3]),StringToNumber<int>(arr[2 + offset]), StringToNumber<int>(arr[3 + offset]), 0.3, string_to_bool(arr[10]),StringToNumber<int>(arr[17]), StringToNumber<int>(arr[17  + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19 + offset]), false);
    }else if(arr[0] == "spring_motor") {
        create_spring(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), h),generate_coordinates(StringToNumber<double>(arr[2 + offset]), w), generate_coordinates(StringToNumber<double>(arr[3 + offset]), h),
                5, StringToNumber<float>(arr[7]), StringToNumber<int>(arr[17]), StringToNumber<int>(arr[17 + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19]), false);
    }else if(arr[0] == "teleportA") {
        create_teleport(generate_coordinates(StringToNumber<double>(arr[2]), w), generate_coordinates(StringToNumber<double>(arr[3]), h),generate_coordinates(StringToNumber<double>(arr[2 + offset]), w),generate_coordinates(StringToNumber<double>(arr[3 + offset]), h),
                StringToNumber<float>(arr[7]), StringToNumber<float>(arr[7 + offset]), StringToNumber<int>(arr[17]), StringToNumber<int>(arr[17 + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19 + offset]), false);
    }else if(arr[0] == "fan_button") {
        create_fan(generate_coordinates(StringToNumber<double>(arr[2 + (offset * 2)]), w),generate_coordinates(StringToNumber<double>(arr[3  + (offset * 2)]), h),
                string_to_bool(arr[9]), StringToNumber<int>(arr[17]), StringToNumber<int>(arr[17 + offset]),
                StringToNumber<int>(arr[17 + (2 * offset)]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]),
                StringToNumber<int>(arr[19 + offset]), StringToNumber<int>(arr[19 + (2 * offset)]), false);
    }else if(arr[0] == "domino") {
        create_domino(generate_coordinates(StringToNumber<double>(arr[2]), w), generate_coordinates(StringToNumber<double>(arr[3]), h), StringToNumber<float>(arr[7]), StringToNumber<int>(arr[17]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), false);
    }else if(arr[0] == "wrecking_ball_platform") {
        b2Vec2 ball_pos;
        ball_pos.x = generate_coordinates(StringToNumber<double>(arr[2]), w);
        ball_pos.y = generate_coordinates(StringToNumber<double>(arr[3]), h);
        b2Vec2 platform_pos;
        platform_pos.x = generate_coordinates(StringToNumber<double>(arr[2 + offset]), w);
        platform_pos.y = generate_coordinates(StringToNumber<double>(arr[3 + offset]), h);
        b2Vec2 distance = ball_pos - platform_pos;
        create_wrecking_ball(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), h),
                generate_coordinates(StringToNumber<double>(arr[2 + offset]), w),generate_coordinates(StringToNumber<double>(arr[3 + offset]),h), distance.Length(), StringToNumber<int>(arr[17]),  StringToNumber<int>(arr[17 + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19]), false);
    }else if(arr[0] == "catapult_lever") {
        create_catapult(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), h), string_to_bool(arr[10]), StringToNumber<int>(arr[17]),  StringToNumber<int>(arr[17 + offset]), string_to_bool(arr[15]), StringToNumber<int>(arr[19]), StringToNumber<int>(arr[19]), false);
    }else if(arr[0] == "goal") {
        create_ushape_entity(generate_coordinates(StringToNumber<double>(arr[2]), w),generate_coordinates(StringToNumber<double>(arr[3]), h), StringToNumber<int>(arr[4]),
              StringToNumber<int>(arr[5]), 35, arr[1], "goal",arr[16], false, StringToNumber<int>(arr[8]), StringToNumber<int>(arr[17]), string_to_bool(arr[15]), StringToNumber<float>(arr[7]), StringToNumber<int>(arr[18]), StringToNumber<int>(arr[19]));
    }
}


void add_move_mask() {
    for(int entity = 0; entity < real_world.mask.size(); ++entity)
    {
//        if(real_world.mask[entity] != COMPONENT_NONE && real_world.quantity[entity] == 0) {
         if(real_world.mask[entity] != COMPONENT_NONE && real_world.name_vec[entity].full_name.find("template") == std::string::npos &&
                 real_world.template_flags_vec[entity].is_template == false) {
//            std::bitset<18> x(real_world.mask[entity]);
//            std::bitset<18> ax(real_world.mask[entity] - MOVE);
//            std::bitset<18> move(MOVE);
//            cout<<"name = "<<real_world.name_vec[entity].name << endl;
//            cout<<"move = " << move <<endl;
//            cout<<"Before substraction = " << x << endl;
//            cout<<"After substraction  = " << ax << endl;
//            cout<<"move = " << MOVE <<endl;
//            cout<<"Before substraction = " << real_world.mask[entity] << endl;
//            cout<<"After substraction  = " << real_world.mask[entity] - MOVE << endl;
//            cout<<"-------"<<endl;
            cout << "mask From add_move_mask After = " << real_world.mask[entity] << endl;
            real_world.mask[entity] = real_world.mask[entity] - MOVE;
            cout << "mask From add_move_mask Before = " << real_world.mask[entity] << endl;
        }
    }
}

int generate_coordinates(double percent, int resolution) {
    int result = (percent * resolution) + 0.5;//correction because int rounds on low 14,999 = 14
//    cout<<"percent is = " << percent << endl;
//    cout<<"cirlce radius is = " << result << endl;
    return result;
}
