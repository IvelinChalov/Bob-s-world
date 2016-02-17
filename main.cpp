#include "mainwindow.h"
#include <QApplication>
#include <Box2D/Box2D.h>
#include "component.h"
#include "entity.h"
#include "glwidget.h"
#include <mycontactlistener.h>

b2Vec2 gravity(0.0f, 9.8f);
b2World* world = new b2World(gravity);
RealWorld real_world;
int global_id = 0;
int error_count = 0;
bool from_editor = true;
bool load_from_editor = false;
int ratio = 10;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MyContactListener lis;
    world->SetContactListener(&lis);


    w.show();

    return a.exec();
}
