#include "glwidget.h"

#include <stdio.h>
#include <cmath>
#include "DeleteSystem.h"
#include "savelevelsystem.h"
#include "loadlevelsystem.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include <GL/glu.h>
#include <QDebug>
#include <QStackedWidget>
#include <QOpenGLTexture>
#include <QImage>
#include <QFontDatabase>

#include<ft2build.h>
#include FT_FREETYPE_H

using namespace std;

GLWidget::GLWidget(QWidget *parent):QGLWidget(parent)
{
    id = QFontDatabase::addApplicationFont(":/new/Fonts/fonts/edosz.ttf");
    family = QFontDatabase::applicationFontFamilies(id).at(0);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(16);
    setMouseTracking(true);
    flag = false;
    clicked = false;
    selected_element = 0;
    colide = false;
    pause = true;
    texture_change_flag = true;
}

void GLWidget::initializeGL() {
    screen_height = this->height();
    screen_width = this->width();
    glViewport( 0.f, 0.f, screen_width, screen_height );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, screen_width, screen_height, 0.0, -1.0, 1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 1.f, 1.f, 1.f, 1.f );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
   // glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

   //cout<<"initGL load level system----------------------------" <<endl;
//   if(!from_editor) {
//       clear_all_entities();
//       load_level_system(gf->getCurrent_level(), false);
//       add_move_mask();
//   }else{
//      clear_all_entities();
//      //load_level_system(gf->getCurrent_level(), false);
//      create_templates(1048);
//      init_quantity();
//   }
}


void GLWidget::paintEvent(QPaintEvent *event) {
   QPainter painter(this);
   painter.beginNativePainting();
      paintGL();
   painter.endNativePainting();

   draw_numbers(&painter);
}


//#define RENDER_SYSTEM_MASK (QUANTITY_MASK)
void GLWidget::draw_numbers(QPainter *painter) {

    QColor fontColor = QColor(255, 153, 0);
    painter->setPen(fontColor);
    QFont monospace(family, 12, QFont::Normal, false);
    painter->setFont(monospace);

    for(int entity = 0; entity < real_world.mask.size(); ++entity)
    {
        if((real_world.mask[entity] & QUANTITY_MASK) == QUANTITY_MASK)
        {
            //cout<<"entity = " << entity<< endl;
            QString number = QString::number(real_world.quantity[entity]);
            int x = real_world.coordinates_vec[entity].x;
            int y = real_world.coordinates_vec[entity].y;
            int w = real_world.body_size_vec[entity].width;
            int h = real_world.body_size_vec[entity].height;
            x = x + w/5;
            y = y - h/5;
            painter->drawText(x, y, number);
        }
    }
   painter->end();
}



#define RENDER_SYSTEM_MASK (RENDER_MASK)
void GLWidget::paintGL() {
  //  QPainter painter(this);

    //int num = real_world.mask.size();

        //for(std::vector<int>::iterator it = real_world.mask.begin() ; it != real_world.mask.end(); ++it)
        //int entity = 0;
  // painter.beginNativePainting();

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );
        glClear(GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

   //     QGLWidget::renderText(1, 100, "some text", QFont( "ariel", 12, QFont::Bold, TRUE ) );

        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
          fprintf(stderr, "Could not init freetype library\n");
          //return 1;
        }




       // textures();
        for(int entity = 0; entity < real_world.mask.size(); ++entity)
        {
            if((real_world.mask[entity] & RENDER_SYSTEM_MASK) == RENDER_SYSTEM_MASK)
            {
                PhysicsBody *physics_body;
                physics_body = &(real_world.physics_body_vec[entity]);
                b2Body* body = physics_body->body;
                body->SetLinearDamping(0.1);//AIR resistance
                //Colors color;
                //color = real_world.color_vec[entity];
                int cur_color_id = real_world.color_vec[entity].selected_color;
                float r = real_world.color_vec[entity].color[cur_color_id].r;
                float g = real_world.color_vec[entity].color[cur_color_id].g;
                float b = real_world.color_vec[entity].color[cur_color_id].b;
                float a = real_world.color_vec[entity].color[cur_color_id].a;

                // cout<<"color from Render r = "<<r<<endl;
                // cout<<"color g = "<<g<<endl;
                // cout<<"color b = "<<b<<endl;
                CheckForDelete(body, entity, screen_height);
                //cout<<"Written red = "<<r<<" green = "<<g<<" blue = "<<b<<endl;
                BodyDelete *delete_body;
                delete_body = &(real_world.for_delete_vec[entity]);
                //std::cout<<body->GetWorldCenter().y<<"| \n";
                for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
                    b2Shape::Type t = f->GetShape()->GetType();
                     if ( t == b2Shape::e_polygon ) {//TO DO add more shapes if its needed
                         b2PolygonShape* poly_shape = (b2PolygonShape*)f->GetShape();
                         int32 count = poly_shape->GetVertexCount();
                         b2Vec2 points[count];
                         for (int i = 0; i < count; ++i) {
                            points[i] = poly_shape->GetVertex(i);
                         }
                         //render_object(points, body->GetPosition(), body->GetAngle(), count, r, g, b, a, entity);
                         textures(points, body->GetPosition(), body->GetAngle(), entity);
                        // std::cout<<body->GetWorldCenter().x<<"|"<<body->GetWorldCenter().y<<"\n";
                     }else if (t == b2Shape::e_circle) {
                        b2CircleShape* circle_shape = (b2CircleShape*)body->GetFixtureList()->GetShape();
                        float radius = circle_shape->m_radius;
                       // circle_texture(body->GetWorldCenter(), radius, body->GetAngle(), entity);
                        render_circle_object(body->GetWorldCenter(), radius, body->GetAngle(), r, g, b, a, entity);
                     }
                }
                //cout<<"----end-----\n";

                for (b2JointEdge* j = body->GetJointList(); j; j = j->next) {
                      // std::cout<<"aX = "<<j->joint->GetAnchorA().x * MTOP<<"aY = "<<j->joint->GetAnchorA().y * MTOP<<"\n";
                      // std::cout<<"bX = "<<j->joint->GetAnchorB().x * MTOP<<"bY = "<<j->joint->GetAnchorB().y * MTOP<<"\n";
                      int32 count = 2;
                      b2Vec2 points[count];
                      points[0] = j->joint->GetAnchorA();
                      points[1] = j->joint->GetAnchorB();
                      render_joint(points, entity);
                 }
            }
            //num--;
            //entity++;
        }



   float32 timeStep = 1.0f / 60.0f;
   int32 velocityIterations = 6;
   int32 positionIterations = 2;
   if(!pause)world->Step(timeStep, velocityIterations, positionIterations);
   update();
   DeleteEntitySystem();

  // painter.endNativePainting();

}

void GLWidget::resizeGL(int w, int h)
{
    screen_height = h;
    screen_width = w;
    float aratio = (float)w/h;

    glViewport( 0.f, 0.f, (GLsizei)w, (GLsizei)h );
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0.0, w , h, 0.0, -1.0, 1.0 );
    //gluPerspective(45, aratio, 0, 1000);
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glClearColor( 0.f, 0.f, 0.f, 1.f );
}

#define MOVE_MASK (COMPONENT_NONE | MOVE)
void GLWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->pos().x();
    int y = event->pos().y();
    qDebug() << "x = " << x;
    qDebug() << "y = " << y;
    if(event->button() == Qt::LeftButton  && clicked == true && colide == false && pause) {
        set_defalt_texture();
        if(real_world.name_vec[selected_element].name == "elavator_platform") {
            b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
            ((b2PrismaticJoint*)j->joint)->EnableMotor(false);
            ((b2PrismaticJoint*)j->joint)->SetMotorSpeed(0);
            ((b2PrismaticJoint*)j->joint)->SetMaxMotorForce(0);
            b2Body* motor = j->other;
            b2Vec2 selected_element_pos;
            selected_element_pos.x = real_world.physics_body_vec[selected_element].body->GetPosition().x * MTOP;
            selected_element_pos.y = real_world.physics_body_vec[selected_element].body->GetPosition().y * MTOP;
            b2Vec2 motor_element_pos;
            motor_element_pos.x = motor->GetPosition().x * MTOP;
            motor_element_pos.y = motor->GetPosition().y * MTOP;
            if(selected_element_pos.x > motor_element_pos.x - 4 && selected_element_pos.x < motor_element_pos.x + 4) {
                flag = false;
                clicked = false;
                real_world.selected_vec[selected_element].selected = false;
                real_world.coordinates_vec[selected_element].x = real_world.physics_body_vec[selected_element].body->GetPosition().x * MTOP;
                real_world.coordinates_vec[selected_element].y = real_world.physics_body_vec[selected_element].body->GetPosition().y * MTOP;
                // b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
                // b2Body* motor = j->other;
                float motor_y = motor->GetPosition().y * MTOP;
                float max_translation = abs(motor_y - y);
                b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
                pj->SetLimits ((-(max_translation - 10)) * PTOM, 0);
                std::cout<<"in reset ELEVATOR !!!\n";
                selected_element = -1;
            }
        }else if(real_world.name_vec[selected_element].name == "wrecking_ball") {
            b2Body* ball = real_world.physics_body_vec[selected_element].body;
            b2JointEdge* j = ball->GetJointList();
            b2RopeJoint* rj = (b2RopeJoint*)j->joint;
            b2Body* platform = j->other;
            b2Vec2 distance = ball->GetWorldCenter() - platform->GetWorldCenter();
            rj->SetMaxLength(distance.Length());
            flag = false;
            clicked = false;
            real_world.selected_vec[selected_element].selected = false;
            real_world.coordinates_vec[selected_element].x = real_world.physics_body_vec[selected_element].body->GetPosition().x * MTOP;
            real_world.coordinates_vec[selected_element].y = real_world.physics_body_vec[selected_element].body->GetPosition().y * MTOP;
            selected_element = -1;

        }else if(real_world.name_vec[selected_element].name != "el_templ") {
            flag = false;
            clicked = false;
            real_world.selected_vec[selected_element].selected = false;
            real_world.coordinates_vec[selected_element].x = real_world.physics_body_vec[selected_element].body->GetPosition().x * MTOP;
            real_world.coordinates_vec[selected_element].y = real_world.physics_body_vec[selected_element].body->GetPosition().y * MTOP;
            selected_element = -1;
        }
        texture_change_flag = true;
    }else if(event->button() == Qt::LeftButton  && clicked == false && pause) {
        for(int entity = 0; entity < real_world.mask.size(); ++entity) {
            if((real_world.mask[entity] & MOVE_MASK) == MOVE_MASK) {
                bool inside = checkForMouseCollision(x, y, entity);
                Color* col;
                int scaled_height;
                int scaled_width;
                std::cout<<inside<<"<---FLAG \n";
                if(inside && real_world.name_vec[entity].name == "template_obstecle_wood") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        col = get_colors(WOOD_COLOR_MASK);
                        scaled_width = (10 + ratio);
                        scaled_height = (90 + ratio);
                        if(!from_editor){
                            create_box_entity(x, y, scaled_width, scaled_height, "static", "wood", "wood", true, false, false, false, WOOD_COLOR_MASK, 0, true, 0, WOOD_TEXTURE_MASK, 0);
                        }else{
                            create_box_entity(x, y, scaled_width, scaled_height, "static", "wood", "wood", true, false, false, false, WOOD_COLOR_MASK, 0, false, 0, WOOD_TEXTURE_MASK, 0);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_circle") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        col = get_colors(CIRCLE_COLOR_MASK);
                        int scaled_radius = 5 + ratio;
                        if(!from_editor){
                            create_circle_entity(x, y, scaled_radius, 1, "dynamic", "circle", "circle", true, CIRCLE_COLOR_MASK, 0, true, CIRCLE_TEXURE_MASK, 0);
                        }else{
                            create_circle_entity(x, y, scaled_radius, 1, "dynamic", "circle", "circle", true, CIRCLE_COLOR_MASK, 0, false, CIRCLE_TEXURE_MASK, 0);
                        }
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_moving_platform") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        scaled_width = scale_width(50 + ratio);
                        scaled_height = scale_height(20 + ratio);
                        if(!from_editor){
                            create_joint_entity(x, y, scaled_width, scaled_height, 0, 0, 0, true, 0, 0, true);
                        }else{
                            create_joint_entity(x, y, scaled_width, scaled_height, 0, 0, 0, false, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_elevator") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_test_elevator(x, y, x, y - 30, 0.3, true, 0, 0, true, 0, 0, true);
                        }else{
                            create_test_elevator(x, y, x, y - 30, 0.3, true, 0, 0, false, 0, 0, true);
                        }
                        //create_motor_entity(x, y, 222.0f, 255.0f, 0.0f);
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_spring") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_spring(x, y, x, y - (15+ratio), 5, 0, 0, 0, true, 0, 0, true);
                        }else{
                            create_spring(x, y, x, y - (15+ratio), 5, 0, 0, 0, false, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_teleport") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_teleport(x, y, x + (45+ratio), y , 0, 0, 0, 0, true, 0, 0, true);
                        }else{
                            create_teleport(x, y, x + (45+ratio), y , 0, 0, 0, 0, false, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_fan") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_fan(x, y, false, 0, 0, 0, true, 0, 0, 0, true);
                        }else{
                            create_fan(x, y, false, 0, 0, 0, false, 0, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_domino") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor) {
                            create_domino(x, y, 0, 0, true, 0, true);
                        }else{
                            create_domino(x, y, 0, 0, false, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                        cout << "end IF domino created \n";
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_wrecking_ball") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_wrecking_ball(x, y, x, y + 95, 100, 0, 0, true, 0, 0, true);
                        }else{
                            create_wrecking_ball(x, y, x, y + 95, 100, 0, 0, false, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_catapult") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        if(!from_editor){
                            create_catapult(x, y, true, 0, 0, true, 0, 0, true);
                        }else{
                            create_catapult(x, y, true, 0, 0, false, 0, 0, true);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        qDebug() << "From template sel. elem. = " << selected_element;
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside && real_world.name_vec[entity].name == "template_obstecle_goal") {
                    if(from_editor || real_world.quantity[entity] > 0){
                        col = get_colors(WRECKING_BALL_COLOR_MASK);
                        if(!from_editor){
                            create_ushape_entity( x, y, 35, 5, 35, "dynamic", "goal", "goal", true, WRECKING_BALL_COLOR_MASK, 0, true, 0, CATAPULT_TEXTURE_MASK, 0);
                        }else{
                            create_ushape_entity( x, y, 35, 5, 35, "dynamic", "goal", "goal", true, WRECKING_BALL_COLOR_MASK, 0, false, 0, CATAPULT_TEXTURE_MASK, 0);
                        }
                        std::cout<<"number of components ="<<real_world.mask.size()<<"\n";
                        flag = true;
                        clicked = true;
                        selected_element = getSelectedElement();
                        if(!from_editor)real_world.quantity[entity]--;
                    }
                }else if(inside) {
                    flag = true;
                    clicked = true;
                    cout<<"you clicked on "<<real_world.name_vec[entity].name<<endl;
                    real_world.selected_vec[entity].selected = true;
                    selected_element = entity;
                    cout << "Flags set" << endl;
                }
            }
        }
     }
    qDebug() << "selected element id = " << selected_element;
}

int  GLWidget::getSelectedElement() {
//    if(real_world.for_delete_vec[4].for_delete  || real_world.for_delete_vec[0].for_delete){
//        cout<<"getSelectedElement SPRING ENTITY DELETE FLAG = "<<real_world.for_delete_vec[4].for_delete<<"error_count = "<<error_count<<endl;
//        error_count++;
//    };
    for(int entity = 0; entity < real_world.mask.size(); ++entity) {
        if(real_world.selected_vec[entity].selected) return entity;
    }

        return -1;
}
#define COLLISION_MASK (COLLISION)
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouse_coordinates = event->pos();
    if(flag) {
//            cout << "element is moving"<<endl;
            b2Vec2 pos;
            pos.x = mouse_coordinates.x() * PTOM;
            pos.y = mouse_coordinates.y() * PTOM;
            colide = false;
            bool local_colide = false;
            b2Vec2* bodyA_points;
            b2Vec2* bodyB_points;

            b2Body* sel_body = real_world.physics_body_vec[selected_element].body;
            if(sel_body == NULL) cout<<"Problem !!!!!"<<endl;
            cout << "selected_element = " << selected_element <<endl;
            cout << "name is == "<< real_world.name_vec[selected_element].name << endl;
            cout << "before sel_body"<<endl;
            b2JointEdge* jo = sel_body->GetJointList();
            cout << "sel_body passed"<<endl;
            int buff = -1;
            int* id = &buff;
            if(jo) {
                b2Body* other = jo->other;
                void* body_user_data = other->GetUserData();
                id = static_cast<int*>(body_user_data);
            }
            bodyA_points = get_vertexes(sel_body, pos.x * MTOP, pos.y * MTOP, false);
            for(int entity = 0; entity < real_world.mask.size(); ++entity) {
                if((real_world.mask[entity] != COMPONENT_NONE) && ((real_world.mask[entity] & COLLISION_MASK) == COLLISION_MASK)) {
                    if(entity == selected_element || entity == *id) continue;
                    b2Body* cur_body = real_world.physics_body_vec[entity].body;
                    bodyB_points = get_vertexes(cur_body, pos.x * MTOP, pos.y * MTOP, true);
                    if(testPlace(bodyA_points, bodyB_points, 4)) {
                        colide = true;
                        local_colide = true;
                        break;
                    }else {
                        colide = false;
                    }
                }
            }

            // cout<<"colide = "<<colide<<endl;
            // cout<<"local_colide = "<<local_colide<<endl;
            // cout<<"---------------\n";
            for (b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList(); j; j = j->next) {
                b2Body* sel_body = j->other;
                void* body_user_data = sel_body->GetUserData();
                int* id = static_cast<int*>(body_user_data);
                if(real_world.name_vec[*id].name == "fan")continue;
                bodyA_points = get_vertexes(sel_body, pos.x, pos.y, false);
                for(int entity = 0; entity < real_world.mask.size(); ++entity) {
                    if((real_world.mask[entity] != COMPONENT_NONE) && ((real_world.mask[entity] & COLLISION_MASK) == COLLISION_MASK)) {
                        if(entity == *id || entity == selected_element) {//Logic is bad need to remove the joints from the collision
                            //local_colide = false;
                            continue;
                        }
                        b2Body* cur_body = real_world.physics_body_vec[entity].body;
                        bodyB_points = get_vertexes(cur_body, pos.x, pos.y, true);
                        if(testPlace(bodyA_points, bodyB_points, 4)) {//TODO make it dynamic the points
                            colide = true;
                            local_colide = true;
                            break;
                        }else {
                            colide = false;
                        }
                    }
                }
            }

            colide = local_colide;
            int fan_body_id = real_world.fan_vec[selected_element].fan_body_id;
            change_texture_on_collision(colide);
                b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
            if(j && real_world.name_vec[selected_element].name != "elavator_platform" &&
                    real_world.name_vec[selected_element].name != "teleportA" &&
                    real_world.name_vec[selected_element].name != "teleportB" &&
                    real_world.name_vec[selected_element].name != "wrecking_ball") {
                // cout<<"START\n";
                b2Body* curr = real_world.physics_body_vec[selected_element].body;
                for (b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList(); j; j = j->next) {
                    b2Body* other = j->other;
                    move_body(curr, other, pos);
                }
                float32 angle =  curr->GetAngle();
                curr->SetTransform(pos, angle);
            } else {
                cout<<"point ELSE reached\n";

                float32 angle =  real_world.physics_body_vec[selected_element].body->GetAngle();
                real_world.coordinates_vec[selected_element].x = pos.x;
                real_world.coordinates_vec[selected_element].y = pos.y;
                real_world.physics_body_vec[selected_element].body->SetTransform(pos, angle);
            }

             delete[] bodyA_points;
             delete[] bodyB_points;
        }

       // cout<<"end of moving"<<endl;
}

void  GLWidget::move_body(b2Body* curr, b2Body* other, b2Vec2 pos) {
    float32 angle =  curr->GetAngle();
    b2Vec2 other_pos;
    other_pos.x = ((pos.x) - curr->GetPosition().x) + other->GetPosition().x;
    other_pos.y = ((pos.y) - curr->GetPosition().y) + other->GetPosition().y;
    real_world.coordinates_vec[selected_element].x = pos.x * MTOP;
    real_world.coordinates_vec[selected_element].y = pos.y * MTOP;
    int id = get_id(other);
    real_world.coordinates_vec[id].x = other_pos.x * MTOP;
    real_world.coordinates_vec[id].y = other_pos.y * MTOP;
    other->SetTransform(other_pos, angle);
}

b2Vec2* GLWidget::get_vertexes(b2Body* body, int x, int y, bool is_second_body) {
    b2Vec2* points;
        int num = 0;
        b2Fixture* fix = body->GetFixtureList();
        b2Shape::Type t = fix->GetShape()->GetType();
        if (t == b2Shape::e_polygon) {
            b2PolygonShape* poly_shape = (b2PolygonShape*)fix->GetShape();
            int32 count = poly_shape->GetVertexCount();
            num = count;
            points = new (nothrow) b2Vec2[count];
            if(points == NULL) {
                cout << "ERROR: memory could not be allocated";
            }

            for (int i = 0; i < count; ++i) {
                points[i].x = body->GetWorldPoint(poly_shape->GetVertex(i)).x * MTOP;
                points[i].y = body->GetWorldPoint(poly_shape->GetVertex(i)).y * MTOP;
            }
        }else if(t == b2Shape::e_circle && is_second_body == false) {
            int radius = real_world.body_size_vec[selected_element].radius;
            points = new (nothrow) b2Vec2[4];
            if(points == NULL) {
                cout << "ERROR: memory could not be allocated";
            }
            num = 4;
            points[0].x = x - radius;
            points[0].y = y - radius;
            points[1].x = x - radius;
            points[1].y = y + radius;
            points[2].x = x + radius;
            points[2].y = y + radius;
            points[3].x = x + radius;
            points[3].y = y - radius;
        }else if(t == b2Shape::e_circle && is_second_body) {
            void* body_user_data = body->GetUserData();
            int* id = static_cast<int*>(body_user_data);
            int radius = real_world.body_size_vec[*id].radius;
            points = new (nothrow) b2Vec2[4];
            if(points == NULL) {
                cout << "ERROR: memory could not be allocated";
            }
            num = 4;
            points[0].x = body->GetPosition().x * MTOP - radius;
            points[0].y = body->GetPosition().y * MTOP - radius;
            points[1].x = body->GetPosition().x * MTOP - radius;
            points[1].y = body->GetPosition().y * MTOP + radius;
            points[2].x = body->GetPosition().x * MTOP + radius;
            points[2].y = body->GetPosition().y * MTOP + radius;
            points[3].x = body->GetPosition().x * MTOP + radius;
            points[3].y = body->GetPosition().y * MTOP - radius;
        }
        b2Vec2* result = points;
        return result;
}

bool GLWidget::testPlace(b2Vec2 selected_points[], b2Vec2 other_object_points[], int lenght) {
        b2Vec2 axes1[lenght];
        b2Vec2 axes2[lenght];

        for (int i = 0; i < lenght; ++i) {
            axes1[i] = getEdgeNormal(selected_points[i], selected_points[i + 1 == lenght ? 0 : i + 1]);
            axes2[i] = getEdgeNormal(other_object_points[i], other_object_points[i + 1 == lenght ? 0 : i + 1]);
        }
        //b2Vec2* axes1 = getEdgeNormals(selected_points, lenght);//on the cursor
        //b2Vec2* axes2 = getEdgeNormals(other_object_points, lenght);
        // loop over the axes1
        for (int i = 0; i < lenght; i++) {//axes1
          b2Vec2 axis = axes1[i];
          // std::cout<<axes1[0].x<<""<<"|"<<axes1[0].y<<"<_---------------------\n";
          // std::cout<<axes2[0].x<<""<<"|"<<axes2[0].y<<"<_---------------------\n";
          // project both shapes onto the axis
          b2Vec2 p1 = project(selected_points, axis, lenght);
          b2Vec2 p2 = project(other_object_points, axis, lenght);
          // do the projections overlap?
          // std::cout<<"p1 min = "<<p1.x<<" p1 max = "<<p1.y<<"\n";
          // std::cout<<"p2 min = "<<p2.x<<" p2 max = "<<p2.y<<"\n";
          if (!overlap(p1, p2)) {
            // then we can guarantee that the shapes do not
            return false;
          }
        }
        // loop over the axes2
        for (int i = 0; i < lenght; i++) {//axes2
          b2Vec2 axis = axes2[i];
          // project both shapes onto the axis
          b2Vec2 p1 = project(selected_points, axis, lenght);
          b2Vec2 p2 = project(other_object_points, axis, lenght);
          // do the projections overlap?
          if (!overlap(p1, p2)) {
            // then we can guarantee that the shapes  place overlap
            return false;
          }
        }
        // if we get here then we know that every axis had overlap on it
        // so we can guarantee an intersection
        return true;
}

b2Vec2 GLWidget::getEdgeNormal(b2Vec2 ver_a, b2Vec2 ver_b) {
    if(real_world.for_delete_vec[4].for_delete || real_world.for_delete_vec[0].for_delete){
            cout<<"getEdgeNormal SPRING ENTITY DELETE FLAG = "<<real_world.for_delete_vec[4].for_delete<<"error_count = "<<error_count<<endl;
            error_count++;
        };
        b2Vec2 axes;
        b2Vec2 p1 = ver_a;
        b2Vec2 p2 = ver_b;
        b2Vec2 edge;
        edge.x = p2.x - p1.x;
        edge.y = p2.y - p1.y;
        // get either perpendicular vector
        b2Vec2 normal;
        normal.x = -edge.y;
        normal.y = edge.x;
        // the perp method is just (x, y) => (-y, x) or (y, -x)
        // double len = sqrt(normal.x * normal.x + normal.y * normal.y);
        // normal.x = normal.x/len;
        // normal.y = normal.y/len;
        axes = normal;

        return axes;
}

bool GLWidget::overlap(b2Vec2 p1, b2Vec2 p2) {
    if(real_world.for_delete_vec[4].for_delete || real_world.for_delete_vec[0].for_delete){
            cout<<"update SPRING ENTITY DELETE FLAG = "<<real_world.for_delete_vec[4].for_delete<<"error_count = "<<error_count<<endl;
            error_count++;
        };
        if(p1.y > p2.x && p1.x < p2.y) {
          return true;
        }

        return false;
}

bool GLWidget::checkForMouseCollision(const int& x, const int& y, const int& id) {
        bool inside = true;
        if(real_world.name_vec[id].name == "moving_platform"){return false;}
        if(real_world.name_vec[id].name == "circle" || real_world.name_vec[id].name == "balloon" || real_world.name_vec[id].name == "wrecking_ball") {
            int radius = real_world.body_size_vec[id].radius;
            int xCor = real_world.physics_body_vec[id].body->GetPosition().x * MTOP;
            int yCor = real_world.physics_body_vec[id].body->GetPosition().y * MTOP;
            int distance = sqrt(pow(xCor - x, 2) + pow(yCor - y, 2));
            if(distance > radius) {
                inside = false;
            }
        }else{
            b2Vec2 mouse_pos[4];
            mouse_pos[0].x = x;
            mouse_pos[0].y = y;
            mouse_pos[1].x = x;
            mouse_pos[1].y = y + 1;
            mouse_pos[2].x = x + 1;
            mouse_pos[2].y = y + 1;
            mouse_pos[3].x = x + 1;
            mouse_pos[3].y = y;

            b2Body* body = real_world.physics_body_vec[id].body;
            for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
                b2Shape::Type t = f->GetShape()->GetType();
                if ( t == b2Shape::e_polygon ) {
                    b2PolygonShape* poly_shape = (b2PolygonShape*)f->GetShape();
                    int32 count = poly_shape->GetVertexCount();
                    b2Vec2 points[count];
                    for (int i = 0; i < count; ++i) {
                        points[i].x = body->GetWorldPoint(poly_shape->GetVertex(i)).x * MTOP;
                        points[i].y = body->GetWorldPoint(poly_shape->GetVertex(i)).y * MTOP;
                    }

                    if(!testPlace(mouse_pos, points, count)) {
                        inside = false;
                    }else return true;

                }
            }
        }
        return inside;
}

b2Vec2 GLWidget::project(b2Vec2 pos[], b2Vec2 axis, int lenght) {
    double min = dotProduct(axis, pos[0]);
    double max = min;
    for (int i = 1; i <lenght; ++i) {
      double p = dotProduct(axis, pos[i]);
      if (p < min) {
        min = p;
      } else if (p > max) {
        max = p;
      }
    }
    b2Vec2 proj;
    proj.x = min;
    proj.y = max;
    return proj;
}

double GLWidget::dotProduct(const b2Vec2& a, const b2Vec2& b) {
    double result = ((a.x * b.x) + (a.y * b.y));
    return result;
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_C) {
        DeleteEntitySystem();
//        Color* col;
//        col = get_colors(CIRCLE_COLOR_MASK);
//        create_domino(mouse_coordinates.x(), mouse_coordinates.y(), 0, 0, false, 0, false);
  }if(event->key() == Qt::Key_P) {
        cout<<"Number of components = "<< real_world.mask.size() <<endl;
        for (int id = 0; id < real_world.mask.size(); ++id) {
            //if(real_world.mask[id] != COMPONENT_NONE) {
            cout<<"-----------------------------------------------------"<<endl;
            cout <<"entiy mask " << real_world.mask[id] << endl;
            cout <<"entiy " << real_world.name_vec[id].name << " flag is = " << real_world.for_delete_vec[id].for_delete << endl;
            cout<<"-----------------------------------------------------"<<endl;
           // }
           //cout << real_world.name_vec[id].name << " is selected = " << real_world.selected_vec[id].selected << endl;
        }
        cout<<"----------"<<endl;
  }if(event->key() == Qt::Key_S) {
        if(from_editor && !flag){
            for(int entity = 0; entity < real_world.mask.size(); ++entity) {
                if(real_world.mask[entity] != COMPONENT_NONE) {
                    bool inside = checkForMouseCollision( mouse_coordinates.x(), mouse_coordinates.y(), entity);
                    if(inside) {
                        int cur_col = 0;
                        int selected_texture = 0;
                        int height = 0;
                        int width = 0;
                        real_world.template_flags_vec[entity].is_template = !real_world.template_flags_vec[entity].is_template;
                        if(real_world.template_flags_vec[entity].is_template){
                            real_world.color_vec[entity].selected_color = 2;
                            real_world.textures_vec[entity].selected_texture = 2;
                            cur_col = 2;
                            selected_texture = 2;
                            height = 13;
                            width = 5;
                        }else{
                            real_world.color_vec[entity].selected_color = 0;
                            real_world.textures_vec[entity].selected_texture = 0;
                            cur_col = 0;
                            selected_texture = 0;
                            height = -13;
                            width = -5;
                        }
                        //changeObjectHeight(width, height);
                        for (b2JointEdge* j = real_world.physics_body_vec[entity].body->GetJointList(); j; j = j->next) {
                           b2Body* other = j->other;
                           int id = get_id(other);
                           real_world.template_flags_vec[id].is_template = !real_world.template_flags_vec[id].is_template;
                           real_world.color_vec[id].selected_color = cur_col;
                           real_world.textures_vec[id].selected_texture = selected_texture;
                          // changeObjectHeight(width, height);
                          // qDebug() <<"for loop pass";
                        }
                        string a = real_world.name_vec[entity].name;
                        cout << a << " is selected = " << real_world.template_flags_vec[entity].is_template << endl;
                        std::string name = "template_obstecle_" + real_world.name_vec[entity].full_name;
                        change_quatity(name, real_world.template_flags_vec[entity].is_template);
                        break;
                    }
                }
            }

        }

   }else if(event->key() == Qt::Key_E) {
        if(flag){
            flag = false;
            clicked = false;
            real_world.for_delete_vec[selected_element].for_delete = true;
            real_world.selected_vec[selected_element].selected = false;
            if(!from_editor) {
                std::string name = "template_obstecle_" + real_world.name_vec[selected_element].full_name;
                change_quatity(name, true);//for increase and decreese
            }
            for (b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList(); j; j = j->next) {
                    b2Body* other_body = j->other;
                    void* user_data = other_body->GetUserData();
                    int* id = static_cast<int*>(user_data);
                    real_world.for_delete_vec[*id].for_delete = true;
            }
        }
    }else if (event->key() == Qt::Key_A) {
        if(flag){
            std::cout<<real_world.coordinates_vec[selected_element].angle<<"\n";
            if(real_world.name_vec[selected_element].name == "moving_platform_middle") {
                rotateObject(-5);//degrees
                b2Vec2 vel;
                vel.x = 1 * cos(real_world.physics_body_vec[selected_element].body->GetAngle());
                vel.y = 1 * sin(real_world.physics_body_vec[selected_element].body->GetAngle());
                cout<<"vel.x = "<<vel.x<<"vel.y = "<<vel.y<<endl;
                void* body = real_world.physics_body_vec[selected_element].body->GetJointList()->joint->GetBodyA()->GetUserData();
                int* id = static_cast<int*>(body);
                real_world.velocity_vec[*id] = vel;
                real_world.physics_body_vec[selected_element].body->SetLinearVelocity(vel);
                b2Body* middle_body = real_world.physics_body_vec[selected_element].body;
                real_world.physics_body_vec[*id].body->SetTransform(middle_body->GetPosition(), middle_body->GetAngle());
            }else {
                rotateObject(-5);
            }
        }
    }else if (event->key() == Qt::Key_D) {
        if(flag){
            if(real_world.name_vec[selected_element].name == "moving_platform_middle") {
                rotateObject(5);//degrees
                b2Vec2 vel;
                vel.x = 1 * cos(real_world.physics_body_vec[selected_element].body->GetAngle());
                vel.y = 1 * sin(real_world.physics_body_vec[selected_element].body->GetAngle());
                cout<<"vel.x = "<<vel.x<<"vel.y = "<<vel.y<<endl;
                void* body = real_world.physics_body_vec[selected_element].body->GetJointList()->joint->GetBodyA()->GetUserData();
                int* id = static_cast<int*>(body);
                real_world.velocity_vec[*id] = vel;
                real_world.physics_body_vec[selected_element].body->SetLinearVelocity(vel);
                b2Body* middle_body = real_world.physics_body_vec[selected_element].body;
                real_world.physics_body_vec[*id].body->SetTransform(middle_body->GetPosition(), middle_body->GetAngle());
            }else {
                rotateObject(5);
            }
            std::cout<<real_world.coordinates_vec[selected_element].angle<<"\n";
        }
    }
}

void GLWidget::changeObjectHeight(const int& step_w, const int& step_h) {
    if(real_world.name_vec[selected_element].name == "wood") {
        b2Body* body = real_world.physics_body_vec[selected_element].body;
        b2Fixture* old_fixture = body->GetFixtureList();
        body->DestroyFixture(old_fixture);
        b2PolygonShape dynamicBox;
        int width = real_world.body_size_vec[selected_element].width;
        int height = real_world.body_size_vec[selected_element].height;
        cout<<"width before = "<<width<<endl;
        cout<<"height before = "<<height<<endl;
        height += step_h;
        width += step_w;
        real_world.body_size_vec[selected_element].height = height;
        real_world.body_size_vec[selected_element].width = width;
        dynamicBox.SetAsBox((width * PTOM)/2, (height * PTOM)/2);
        cout<<"width = "<<width<<endl;
        cout<<"height = "<<height<<endl;
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.shape = &dynamicBox;
        body->CreateFixture(&fixtureDef);
    }
}

void GLWidget::rotateObject(const float32& vel) {
    if(real_world.name_vec[selected_element].name == "elavator_motor" || real_world.name_vec[selected_element].name == "spring_motor") {
        float degrees = 0;
        if(real_world.name_vec[selected_element].name == "elavator_motor") {
            degrees = 180;
        } else{
            degrees = vel;
        }
        b2Vec2 motor_pos = real_world.physics_body_vec[selected_element].body->GetPosition();
        float32 motor_angle = real_world.physics_body_vec[selected_element].body->GetAngle() * RTOD;
        motor_angle = (motor_angle + degrees) * DTOR;
        real_world.coordinates_vec[selected_element].angle = motor_angle * RTOD;
        real_world.physics_body_vec[selected_element].body->SetTransform(motor_pos, motor_angle);

        float angle = degrees * DTOR;
        float rotation_angle = real_world.physics_body_vec[selected_element].body->GetAngle();

        float motor_x = real_world.physics_body_vec[selected_element].body->GetPosition().x * MTOP;
        float motor_y = real_world.physics_body_vec[selected_element].body->GetPosition().y * MTOP;
        b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
        b2Body* elevator = j->other;

        float platform_x = elevator->GetPosition().x * MTOP;
        float platform_y = elevator->GetPosition().y * MTOP;

        platform_x = cos(angle) * (platform_x - motor_x) - sin(angle) * (platform_y - motor_y) + motor_x;
        platform_y = sin(angle) * (platform_x - motor_x) + cos(angle) * (platform_y - motor_y) + motor_y;

        b2Vec2 pos;
        pos.x = (platform_x) * PTOM;
        pos.y = (platform_y) * PTOM;
        void* body_user = elevator->GetUserData();
        int* id = static_cast<int*>(body_user);
        real_world.physics_body_vec[*id].body->SetTransform(pos, rotation_angle);
        real_world.coordinates_vec[*id].angle = rotation_angle * RTOD;
        real_world.inverted_vec[selected_element].is_inverted = !real_world.inverted_vec[selected_element].is_inverted;
        real_world.inverted_vec[*id].is_inverted = !real_world.inverted_vec[*id].is_inverted;
        qDebug() << "motor angle = " << motor_angle * RTOD;
        qDebug() << "platform angle = " << rotation_angle * RTOD;
    }else if(real_world.name_vec[selected_element].full_name == "fan") {
        int fan_id = real_world.fan_vec[selected_element].fan_id;
        int body_id = real_world.fan_vec[selected_element].fan_body_id;
        int button_id = real_world.fan_vec[selected_element].fan_button_id;
        //real_world.fan_vec[fan_id].is_reversed = !real_world.fan_vec[fan_id].is_reversed;
        real_world.fan_vec[fan_id].is_reversed = !real_world.fan_vec[fan_id].is_reversed;
        real_world.fan_vec[body_id].is_reversed = !real_world.fan_vec[body_id].is_reversed;
        real_world.inverted_vec[body_id].is_inverted = !real_world.inverted_vec[body_id].is_inverted;
        real_world.fan_vec[button_id].is_reversed = !real_world.fan_vec[button_id].is_reversed;
        b2Vec2 fan_pos = real_world.physics_body_vec[fan_id].body->GetWorldCenter();
        b2Vec2 body_pos = real_world.physics_body_vec[body_id].body->GetWorldCenter();
        fan_pos.x = fan_pos.x - (2 * (fan_pos.x - body_pos.x));
        float angle = real_world.physics_body_vec[fan_id].body->GetAngle();
        real_world.physics_body_vec[fan_id].body->SetTransform(fan_pos, angle);
        if(real_world.fan_vec[fan_id].is_reversed){
            real_world.textures_vec[body_id].selected_texture = 3;
        }else {
            real_world.textures_vec[body_id].selected_texture = 0;
        }
        texture_change_flag = true;//FOR setting the texture;
        qDebug() << "CHANGE " << real_world.fan_vec[fan_id].is_reversed;
    }else if(real_world.name_vec[selected_element].name == "catapult_lever") {
        b2Body* catapult = real_world.physics_body_vec[selected_element].body;
        b2JointEdge* j = catapult->GetJointList();
        b2RevoluteJoint* rj = (b2RevoluteJoint*)j->joint;
        b2Body* platform = j->other;
        world->DestroyJoint(rj);
        b2RevoluteJointDef revoluteJointDef;
        revoluteJointDef.collideConnected = false;
        revoluteJointDef.bodyA = catapult;
        revoluteJointDef.bodyB = platform;
        b2Vec2 a(5.f * PTOM, 0.f);
        bool invert = real_world.inverted_vec[selected_element].is_inverted;
        int id = get_id(platform);
        b2Vec2 pos;
         pos.x = real_world.coordinates_vec[selected_element].x;
         pos.y = real_world.coordinates_vec[selected_element].y + (real_world.body_size_vec[selected_element].height/2 + real_world.body_size_vec[id].height/2);
         if(!invert) {
            a.x *= -1;
            pos.x = pos.x - 5;
         }else {
            pos.x = pos.x + 5;
         }
        b2Vec2 b(0.f, -10.f * PTOM);//need PTOM if not 0
        revoluteJointDef.localAnchorA = a;
        revoluteJointDef.localAnchorB = b;
        world->CreateJoint(&revoluteJointDef);
        pos *= PTOM;
        platform->SetTransform(pos, 0);
        real_world.inverted_vec[selected_element].is_inverted = !invert;
        real_world.inverted_vec[id].is_inverted = !invert;
    }else if(real_world.name_vec[selected_element].name == "catapult_platform") {
        b2Body* platform = real_world.physics_body_vec[selected_element].body;
        b2JointEdge* j = platform->GetJointList();
        b2RevoluteJoint* rj = (b2RevoluteJoint*)j->joint;
        b2Body* catapult = j->other;
        world->DestroyJoint(rj);
        b2RevoluteJointDef revoluteJointDef;
        revoluteJointDef.collideConnected = false;
        revoluteJointDef.bodyA = catapult;
        revoluteJointDef.bodyB = platform;
        b2Vec2 a(5.f * PTOM, 0.f);
        bool invert = real_world.inverted_vec[selected_element].is_inverted;
        int id = get_id(catapult);
        b2Vec2 pos;
         pos.x = real_world.coordinates_vec[selected_element].x;
         pos.y = real_world.coordinates_vec[selected_element].y - (real_world.body_size_vec[selected_element].height/2 + real_world.body_size_vec[id].height/2);
         if(!invert) {
            a.x *= -1;
            pos.x = pos.x + 5;
         }else {
            pos.x = pos.x - 5;
         }
        b2Vec2 b(0.f, -10.f * PTOM);//need PTOM if not 0
        revoluteJointDef.localAnchorA = a;
        revoluteJointDef.localAnchorB = b;
        world->CreateJoint(&revoluteJointDef);
        pos *= PTOM;
        catapult->SetTransform(pos, 0);
        real_world.inverted_vec[selected_element].is_inverted = !invert;
        real_world.inverted_vec[id].is_inverted = !invert;
    }else {
        b2Vec2 pos = real_world.physics_body_vec[selected_element].body->GetPosition();
        float32 angle = real_world.physics_body_vec[selected_element].body->GetAngle() * RTOD;
        angle = (angle + vel) * DTOR;
        real_world.coordinates_vec[selected_element].angle = angle * RTOD;
        real_world.physics_body_vec[selected_element].body->SetTransform(pos, angle);
    }
}



void GLWidget::render_joint(b2Vec2* points, int id) {
    //std::cout<<"number= "<<vertex<<"\n";
    if(real_world.name_vec[id].full_name != "teleport" && real_world.name_vec[id].full_name != "fan"){
        glColor3f(0,1,0);
        glPushMatrix();
        //glTranslatef(center.x * MTOP, center.y * MTOP, 0);
        //glRotatef(angle*180.0/M_PI, 0, 0, 1);
        glBegin(GL_LINES);
          glVertex2f(points[0].x * MTOP, points[0].y * MTOP);
          glVertex2f(points[1].x * MTOP, points[1].y * MTOP);
        glEnd();
        glPopMatrix();
        glColor3f(1,1,1);
        //std::cout<<"end fun \n";
    }
 }

int num = 0;
 void GLWidget::render_object(b2Vec2* points, b2Vec2 center, float angle, int ver_count, float r, float g, float b, float a, int id) {
    //std::cout<<"vertexes = "<<ver_count<<endl;
    float color_scale = 255;
    float rl = r / color_scale;
    float gl = g / color_scale;
    float bl = b / color_scale;
    glPushMatrix();
    glTranslatef(center.x * MTOP, center.y * MTOP, 0);
    glRotatef(angle * 180.0/M_PI, 0, 0, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i < ver_count; ++i) {
        if((real_world.name_vec[id].name == "teleportA" || real_world.name_vec[id].name == "teleportB") && (i == 1 || i == 2)) {
            rl = 0.f;
            gl = 1.f;
            bl = 0.f;
            num = 0;
        }else {
            rl = r / color_scale;
            gl = g / color_scale;
            bl = b / color_scale;
        }
        glColor4f(rl, gl, bl, a);
       glVertex2f(points[i].x * MTOP, points[i].y * MTOP);
    }
    glEnd();
    glPopMatrix();
 }

void GLWidget::render_circle_object(b2Vec2 center, float radius, float angle, float r, float g, float b, float a, int id) {
    int presision = 20;
    float color_scale = 255;
    r = r / color_scale;
    g = g / color_scale;
    b = b / color_scale;
    //glColor4f(r, g, b, a);
    int selected_texture = real_world.textures_vec[id].selected_texture;
    GLuint texture = real_world.textures_vec[id].texture_arr[selected_texture];
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTranslatef(center.x * MTOP, center.y * MTOP, 0);
    glRotatef(angle*180.0/M_PI, 0, 0, 1);
    radius *= 1.8;
    glBegin(GL_POLYGON);
    float an, radian, x, y, tx, ty, xcos, ysin;
    for (an=0.0; an<360.0; an+=2.0){
        radian = an * (M_PI/180.0f);

        xcos = (float)cos(radian);
        ysin = (float)sin(radian);
        x = xcos * radius * RTOD;
        y = ysin * radius * RTOD;
        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        glTexCoord2f(tx, ty);
        glVertex2f(x, y);
       // qDebug() << "texture x= " << tx << "texture y= " << ty;
    }
    glEnd();
    glPopMatrix();
}
void GLWidget::update() {
    if(!pause){
        for (int i = 0; i < real_world.mask.size(); ++i) {
            if(real_world.mask[i] != COMPONENT_NONE) {
                if(real_world.name_vec[i].name == "spring_motor") {
                    b2JointEdge* j = real_world.physics_body_vec[i].body->GetJointList();
                    b2PrismaticJoint* pj = (b2PrismaticJoint*)j->joint;
                    if(pj->GetJointTranslation() <= pj->GetLowerLimit()) {
                        pj->SetMotorSpeed (5);
                    }
                }
                if(real_world.teleportation_vec[i].is_teleporting) {
                    b2Vec2 pos = real_world.teleportation_vec[i].coordinates;
                    float angle = real_world.teleportation_vec[i].angle;
                    real_world.teleportation_vec[i].is_teleporting = false;
                    real_world.physics_body_vec[i].body->SetTransform(pos, angle);
                    b2Vec2 vel = real_world.teleportation_vec[i].velocity;
                    real_world.physics_body_vec[i].body->SetLinearVelocity(vel);
                }

                if(real_world.ent_flag_vec[i].is_inside) {
                    cout << "current entity name  = " << real_world.name_vec[i].name;
                    int fan_id = real_world.ent_flag_vec[i].fan_id;
                    int fan_btn_id = real_world.fan_vec[fan_id].fan_button_id;
                    cout<<"fan on = "<<real_world.fan_vec[fan_id].is_on<<endl;
                    cout<<"id from update = "<<fan_id<<endl;
                    if(real_world.fan_vec[fan_btn_id].is_on) {
                        float force = 0;
                        float fan_gravity = 5;
                        float circle_mass = 0;
                        b2Vec2 fan_pos = real_world.physics_body_vec[fan_id].body->GetWorldCenter();
                        b2Vec2 cirle_pos = real_world.physics_body_vec[i].body->GetWorldCenter();
                        circle_mass = real_world.physics_body_vec[i].body->GetMass();

                        // b2Vec2 top_fan_pos;
                        // top_fan_pos.x = fan_pos.x - (40 * PTOM);
                        // top_fan_pos.y = fan_pos.y - (40 * PTOM);
                        // top_fan_pos = cirle_pos - top_fan_pos;
                        // float top = top_fan_pos.Length();
                        // cout<<"top = "<<top<<endl;

                        b2Vec2 middle_fan_pos;

                        if(real_world.fan_vec[fan_id].is_reversed) {
                            middle_fan_pos.x = fan_pos.x - (40 * PTOM);
                        }else {
                            middle_fan_pos.x = fan_pos.x + (40 * PTOM);
                        }
                        middle_fan_pos.y = fan_pos.y;
                        middle_fan_pos = cirle_pos - middle_fan_pos;
                        //float mid = middle_fan_pos.Length();
                        //cout<<"mid = "<<mid<<endl;

                        // b2Vec2 bot_fan_pos;
                        // bot_fan_pos.x = fan_pos.x - (40 * PTOM);
                        // bot_fan_pos.y = fan_pos.y + (40 * PTOM);
                        // bot_fan_pos = cirle_pos - bot_fan_pos;
                        // float bot = bot_fan_pos.Length();
                        // cout<<"bot = "<<bot<<endl;

                        // b2Vec2 min_distance;
                        // if(top <= mid && top <= bot) {
                        // 	min_distance = top_fan_pos;
                        // }else if(mid <= top && mid <= bot) {
                        // 	min_distance = middle_fan_pos;
                        // }else {
                        // 	min_distance = bot_fan_pos;
                        // }


                        //b2Vec2 planet_distance = cirle_pos - fan_pos;
                        force = (fan_gravity * circle_mass) / pow(middle_fan_pos.Length(), 2);
                        //planet_distance *= -1;
                        //cout<<"min_distance = "<<min_distance.Length()<<endl;
                        middle_fan_pos *= force;
                        real_world.physics_body_vec[i].body->ApplyForce(middle_fan_pos, cirle_pos, true);
                    }
                }
                if(real_world.name_vec[i].name == "circle") {
                    if(real_world.sticky_id[i].on_platform == true) {
                       int id = real_world.sticky_id[i].platform_id;
                       b2Body* platform = real_world.physics_body_vec[id].body;
                       b2Body* circle = real_world.physics_body_vec[i].body;
                       b2Vec2 vel = platform->GetLinearVelocity();
                       circle->SetLinearVelocity(vel);
//                        b2WeldJointDef weld_joint;
//                        int id = real_world.sticky_id[i].platform_id;
//                        b2Body* platform = real_world.physics_body_vec[id].body;
//                        b2Body* circle = real_world.physics_body_vec[i].body;
//                        weld_joint.bodyA = platform;
//                        weld_joint.bodyB = circle;
//                        b2Vec2 anchor_point_a(0, -25.f * PTOM);
//                        b2Vec2 anchor_point_b(0, 0);
//                        weld_joint.localAnchorA = anchor_point_a;
//                        weld_joint.localAnchorB = anchor_point_b;
//                        world->CreateJoint(&weld_joint);
//                        real_world.sticky_id[i].on_platform == false;
                    }
                }
            }
        }
    }
}

int GLWidget::getScreen_width() const
{
    return screen_width;
}

bool GLWidget::getPause() const
{
    return pause;
}

void GLWidget::setPause(bool value)
{
    pause = value;
}

void GLWidget::textures(b2Vec2* points, b2Vec2 center, float angle, int id)
{
  int selectcted_texture = real_world.textures_vec[id].selected_texture;
  GLuint texture = real_world.textures_vec[id].texture_arr[selectcted_texture];
  glPushMatrix();
  glTranslatef(center.x * MTOP, center.y * MTOP, 0);
  glRotatef(angle * 180.0/M_PI, 0, 0, 1);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
      glTexCoord2f(  0.f,    0.f ); glVertex2f(points[0].x * MTOP, points[0].y * MTOP );
      glTexCoord2f( 1.f,    0.f );  glVertex2f(points[1].x * MTOP, points[1].y * MTOP );
      glTexCoord2f( 1.f, 1.f );     glVertex2f(points[2].x * MTOP, points[2].y * MTOP );
      glTexCoord2f(  0.f, 1.f );    glVertex2f(points[3].x * MTOP, points[3].y * MTOP );
  glEnd();
  glPopMatrix();
}

void GLWidget::circle_texture(b2Vec2 center, float radius, float angle, int id)
{

     GLuint texture = real_world.textures_vec[id].texture_arr[0];



    float a, x, y, xcos, ysin, tx, ty;       // values needed by drawCircleOutline
    float radian;
        glPushMatrix();
        glTranslatef(center.x * MTOP, center.y * MTOP, 0);
        glRotatef(angle * 180.0/M_PI, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_POLYGON);

        for (a=0.0; angle<360.0; angle+=2.0)
        {
            radian  = a * DTOR;
            xcos = (float)cos(radian);
            ysin = (float)sin(radian);
            x = xcos * radius  + center.x * MTOP;
            y = ysin * radius  + center.y * MTOP;
            tx = xcos * 0.5 + 0.5;
            ty = ysin * 0.5 + 0.5;

            glTexCoord2f(tx, ty);
            glVertex2f(x, y);
        }
        glEnd();
        glPopMatrix();
}

GameForm *GLWidget::getGf() const
{
    return gf;
}

void GLWidget::setGf(GameForm *value)
{
    gf = value;
}

Color* get_colors(int mask) {
    static Color color[3];
    switch (mask){
    case CIRCLE_COLOR_MASK: {
            color[0].r = 255.f;
            color[0].g = 255.f;
            color[0].b = 255.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.f;
            color[2].g = 255.f;
            color[2].b = 255.f;
            color[2].a = 0.6f;
            break;
        }
        case WOOD_COLOR_MASK: {
            color[0].r = 160.f;
            color[0].g = 121.f;
            color[0].b = 61.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 160.f;
            color[2].g = 121.f;
            color[2].b = 61.f;
            color[2].a = 0.6f;
            break;
        }
        case MOVING_PLATFORM_MIDDLE_COLOR_MASK: {
            color[0].r = 255.f;
            color[0].g = 0.f;
            color[0].b = 0.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.f;
            color[2].g = 0.f;
            color[2].b = 0.f;
            color[2].a = 0.6f;
            break;
        }
        case ELEVATOR_MOTOR_COLOR_MASK: {
            color[0].r = 255.f;
            color[0].g = 255.f;
            color[0].b = 255.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.f;
            color[2].g = 255.f;
            color[2].b = 255.f;
            color[2].a = 0.6f;
            break;
        }
        case ELEVATOR_PLATFORM_COLOR_MASK: {
            color[0].r = 242.f;
            color[0].g = 242.f;
            color[0].b = 13.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 242.f;
            color[2].g = 242.f;
            color[2].b = 13.f;
            color[2].a = 0.6f;
            break;
        }
        case SPRING_MOTOR_COLOR_MASK: {
            color[0].r = 255.f;
            color[0].g = 255.f;
            color[0].b = 255.f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.f;
            color[2].g = 255.f;
            color[2].b = 255.f;
            color[2].a = 0.6f;
            break;
        }
        case SPRING_PLATFORM_COLOR_MASK: {
            color[0].r = 136.0f;
            color[0].g = 185.0f;
            color[0].b =  145.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 136.0f;
            color[2].g = 185.0f;
            color[2].b = 145.0f;
            color[2].a = 0.5f;
            break;
        }
        case TELEPORT_A_COLOR_MASK: {
            color[0].r = 255.0f;
            color[0].g = 0.0f;
            color[0].b = 0.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.0f;
            color[2].g = 0.0f;
            color[2].b = 0.0f;
            color[2].a = 0.5f;
            break;
        }
        case TELEPORT_B_COLOR_MASK: {
            color[0].r = 0.0f;
            color[0].g = 0.0f;
            color[0].b = 255.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 0.0f;
            color[2].g = 0.0f;
            color[2].b = 255.0f;
            color[2].a = 0.6f;
            break;
        }
        case FAN_COLOR_MASK: {
            color[0].r = 0.0f;
            color[0].g = 255.0f;
            color[0].b = 0.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 0.0f;
            color[2].g = 255.0f;
            color[2].b = 0.0f;
            color[2].a = 0.6f;
            break;
        }
        case FAN_BUTTON_COLOR_MASK: {
            color[0].r = 255.0f;
            color[0].g = 0.0f;
            color[0].b = 0.0f;
            color[0].a = 1.f;
            color[1].r = 0.f;
            color[1].g = 255.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 255.0f;
            color[2].g = 0.0f;
            color[2].b = 0.0f;
            color[2].a = 0.6f;
            break;
        }
        case FAN_BODY_COLOR_MASK: {
            color[0].r = 0.0f;
            color[0].g = 0.0f;
            color[0].b = 255.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 0.0f;
            color[2].g = 0.0f;
            color[2].b = 255.0f;
            color[2].a = 0.6f;
            break;
        }
        case DOMINO_COLOR_MASK: {
            color[0].r = 242.0f;
            color[0].g = 242.0f;
            color[0].b = 255.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 242.0f;
            color[2].g = 242.0f;
            color[2].b = 255.0f;
            color[2].a = 0.6f;
            break;
        }
        case WRECKING_BALL_COLOR_MASK: {
            color[0].r = 102.0f;
            color[0].g = 102.0f;
            color[0].b = 153.0f;
            color[0].a = 1.f;
            color[1].r = 255.f;
            color[1].g = 0.f;
            color[1].b = 0.f;
            color[1].a = 1.f;
            color[2].r = 102.0f;
            color[2].g = 102.0f;
            color[2].b = 153.0f;
            color[2].a = 0.6f;
            break;
        }
    }
    return color;
}

unsigned int* get_textures(int texture_mask) {
    static unsigned int textures[7];
    switch (texture_mask){
    case CIRCLE_TEXURE_MASK: {
            textures[0] = create_texture_id("pics/ball.png");
            textures[1] = create_texture_id("pics/ball_wronge.png");
            textures[2] = create_texture_id("pics/ball_wronge.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_ball.png");;
            break;
        }
    case WOOD_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/wood.png");
            textures[1] = create_texture_id("pics/wood_wronge.png");
            textures[2] = create_texture_id("pics/wood_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_wood.png");;
            break;
        }
    case PLATFORM_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/platform.png");
            textures[1] = create_texture_id("pics/platform_wronge.png");
            textures[2] = create_texture_id("pics/platform_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_moving_platform.png");;
            break;
        }
    case ELEVATOR_MOTOR_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/motor.png");
            textures[1] = create_texture_id("pics/motor_wronge.png");
            textures[2] = create_texture_id("pics/motor_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    case ELEVATOR_PLATFORM_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/elevator_platform.png");
            textures[1] = create_texture_id("pics/elevator_platform_wronge.png");
            textures[2] = create_texture_id("pics/elevator_platform_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    case SPRING_MOTOR_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/motor.png");
            textures[1] = create_texture_id("pics/motor_wronge.png");
            textures[2] = create_texture_id("pics/motor_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_spring.png");;
            break;
        }
    case SPRING_PLATFORM_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/spring_platform.png");
            textures[1] = create_texture_id("pics/spring_platform_wronge.png");
            textures[2] = create_texture_id("pics/spring_platform_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_wrecking_ball.png");
            break;
        }
    case TELEPORT_A_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/teleportA.png");
            textures[1] = create_texture_id("pics/teleportA_wronge.png");
            textures[2] = create_texture_id("pics/teleportA_selected.png");
            textures[3] = create_texture_id("pics/teleportA_half.png");
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_teleport.png");;
            break;
        }
    case TELEPORT_B_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/teleportB.png");
            textures[1] = create_texture_id("pics/teleportB_wronge.png");
            textures[2] = create_texture_id("pics/teleportB_selected.png");
            textures[3] = create_texture_id("pics/teleportB_half.png");
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_teleport.png");
            break;
        }
    case FAN_BUTTON_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/fan_button_off.png");
            textures[1] = create_texture_id("pics/fan_button_wronge.png");
            textures[2] = create_texture_id("pics/fan_button_selected.png");
            textures[3] = create_texture_id("pics/fan_button_on.png");
            textures[4] = create_texture_id("pics/fan_button_wronge.png");
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_fan.png");;
            break;
        }
    case FAN_BODY_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/fan_body_left.png");
            textures[1] = create_texture_id("pics/fan_body_left_wronge.png");
            textures[2] = create_texture_id("pics/fan_body_left_selected.png");
            textures[3] = create_texture_id("pics/fan_body_right.png");
            textures[4] = create_texture_id("pics/fan_body_right_wronge.png");
            textures[5] = create_texture_id("pics/fan_body_right_selected.png");
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    case FAN_WIND_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/wave4.png");
            textures[1] = create_texture_id("pics/wave3.png");
            textures[2] = create_texture_id("pics/wave4_selected.png");
            textures[3] = create_texture_id("pics/wave4.png");
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    case DOMINO_TEXTURE_MASK: {
            textures[0] = create_texture_id("pics/domino.png");
            textures[1] = create_texture_id("pics/domino_wronge.png");
            textures[2] = create_texture_id("pics/domino_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_domino.png");;
            break;
        }
    case CATAPULT_PLATFORM_TEXTURE_MASK:{
            textures[0] = create_texture_id("pics/pin.png");
            textures[1] = create_texture_id("pics/pin_wronge.png");
            textures[2] = create_texture_id("pics/pin_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_wrecking_ball.png");//just using the slot
            break;
        }
    case CATAPULT_TEXTURE_MASK:{
            textures[0] = create_texture_id("pics/motor.png");
            textures[1] = create_texture_id("pics/motor_wronge.png");
            textures[2] = create_texture_id("pics/motor_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_catapult.png");
            break;
        }
    case GOAL_TEXTURE_MASK:{
            textures[0] = create_texture_id("pics/motor.png");
            textures[1] = create_texture_id("pics/motor_wronge.png");
            textures[2] = create_texture_id("pics/motor_selected.png");
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = create_texture_id("pics/template_goal.png");
            break;
        }
    case WHITE_TEXTURE_MASK:{
            textures[0] = create_texture_id("pics/white.png");
            textures[1] = create_texture_id("pics/white_wronge.png");
            textures[2] = EMPTY_TEXTURE;
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    case TEMPLATE_TEXTURE_MASK:{
            textures[0] = create_texture_id("pics/template_wood.png");
            textures[1] = create_texture_id("pics/template_ball.png");
            textures[2] = create_texture_id("pics/template_moving_platform.png");
            textures[3] = create_texture_id("pics/template_spring.png");
            textures[4] = create_texture_id("pics/template_teleport.png");
            textures[5] = create_texture_id("pics/template_fan.png");
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    default:{
            textures[0] = EMPTY_TEXTURE;
            textures[1] = EMPTY_TEXTURE;
            textures[2] = EMPTY_TEXTURE;
            textures[3] = EMPTY_TEXTURE;
            textures[4] = EMPTY_TEXTURE;
            textures[5] = EMPTY_TEXTURE;
            textures[6] = EMPTY_TEXTURE;
            break;
        }
    }

     return textures;
}


GLuint create_texture_id(QString path){
    QImage t;
    QImage b;


    if ( !b.load( path ) )
    {
        qDebug() << "loading fail";
    }
    b = b.mirrored();
    GLuint texture;
    t = QGLWidget::convertToGLFormat(b);
    t.mirrored();
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
   // glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , t.width(), t.height(),  GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, NULL );

    return texture;
}

std::vector<unsigned int> textures_ids;
void GLWidget::change_texture_on_collision(bool colide) {
    string name = real_world.name_vec[selected_element].full_name;

    if(texture_change_flag) {
        textures_ids.clear();
        textures_ids.push_back(real_world.textures_vec[selected_element].selected_texture);
        b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
         for (j; j; j = j->next){
             b2Body* other = j->other;
             int id = get_id(other);
             textures_ids.push_back(real_world.textures_vec[id].selected_texture);

         }
         texture_change_flag = false;
    }
    if(colide) {
        b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
        if(j){
            for (j; j; j = j->next) {
                b2Body* body = j->other;
                int id = get_id(body);
                if(name == "fan" && real_world.fan_vec[id].is_reversed){
                    real_world.textures_vec[id].selected_texture = 4;
                    real_world.textures_vec[selected_element].selected_texture = 4;
                    cout<<"Texture number = 4 is selected <-----------------------------------------" << endl;
                }else {
                    real_world.textures_vec[id].selected_texture = 1;
                    real_world.textures_vec[selected_element].selected_texture = 1;
                }
            }
        }else{
            real_world.textures_vec[selected_element].selected_texture = 1;
            cout<< "inside texture change \n";
        }
    }else {
         b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
         if(j) {
             int i = 1;
             for (j; j; j = j->next) {
                 b2Body* body = j->other;
                 int id = get_id(body);
//                 if((name == "fan" && real_world.inverted_vec[id].is_inverted) || name == "teleport"){
//                     real_world.textures_vec[id].selected_texture = 3;
//                     real_world.textures_vec[selected_element].selected_texture = 3;
//                 }else {
                     real_world.textures_vec[id].selected_texture = textures_ids[i];
                     cout << "texture id = " << textures_ids[i] << endl;
                     real_world.textures_vec[selected_element].selected_texture = textures_ids[0];
//                 }
                 i++;
             }
         }else{
             cout << "texture id = " << textures_ids[0] << endl;
             real_world.textures_vec[selected_element].selected_texture = textures_ids[0];
         }
         //std::cout<<"The place is valid \n";
    }
}

void GLWidget::set_defalt_texture() {
    if(real_world.name_vec[selected_element].full_name == "teleport"){
        b2JointEdge* j = real_world.physics_body_vec[selected_element].body->GetJointList();
        if(j) {
            for (j; j; j = j->next) {
                b2Body* body = j->other;
                int id = get_id(body);
                real_world.textures_vec[id].selected_texture = 0;
            }
        }
        real_world.textures_vec[selected_element].selected_texture = 0;
    }
}
