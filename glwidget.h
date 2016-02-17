#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Component.h"
#include "Entity.h"
#include "component.h"
#include "gameform.h"
#include <Box2D/Box2D.h>

extern b2World* world;
extern RealWorld real_world;
extern int global_id;
extern bool from_editor;
extern bool load_from_editor;
extern int ratio;
extern int error_count; //TODO remove later

Color* get_colors(int mask);
unsigned int* get_textures(int texture_mask);

GLuint create_texture_id(QString path);


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
   explicit GLWidget(QWidget *parent = 0);

   void initializeGL();
   void paintGL();//New render system
   void paintEvent(QPaintEvent *event);
   void draw_numbers(QPainter* painter);

   void resizeGL(int w, int h);
   void render_joint(b2Vec2* points, int id);
   void render_object(b2Vec2* points, b2Vec2 center, float angle, int ver_count, float r, float g, float b, float a, int id);
   void render_circle_object(b2Vec2 center, float radius, float angle, float r, float g, float b, float a, int id);
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void keyPressEvent(QKeyEvent *event);
   void changeObjectHeight(const int& step_w, const int &step_h);
   void rotateObject(const float32& vel);
   int getSelectedElement();
   void  move_body(b2Body* curr, b2Body* other, b2Vec2 pos);
   b2Vec2* get_vertexes(b2Body* body, int x, int y, bool is_second_body);
   bool testPlace(b2Vec2 selected_points[], b2Vec2 other_object_points[], int lenght);
   b2Vec2 getEdgeNormal(b2Vec2 ver_a, b2Vec2 ver_b);
   bool overlap(b2Vec2 p1, b2Vec2 p2);
   bool checkForMouseCollision(const int& x, const int& y, const int& id);
   b2Vec2 project(b2Vec2 pos[], b2Vec2 axis, int lenght);
   double dotProduct(const b2Vec2& a, const b2Vec2& b);
   void update();
   int getScreen_width() const;
   void change_texture_on_collision(bool colide);
   void set_defalt_texture();

   bool getPause() const;
   void setPause(bool value);

   void textures(b2Vec2 *points, b2Vec2 center, float angle, int id);
   void circle_texture(b2Vec2 center, float radius, float angle, int id);


   GameForm *getGf() const;
   void setGf(GameForm *value);

private:
   int screen_width;
   int screen_height;
   QTimer timer;
   QPoint mouse_coordinates;

   bool flag;
   bool clicked;
   int selected_element;
   bool colide;
   bool pause;
   GameForm* gf;
   bool texture_change_flag;
//font variables
   QString family;
   int id;

signals:

public slots:
};

#endif // GLWIDGET_H
