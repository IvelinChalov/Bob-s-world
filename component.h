#ifndef COMPONENT
#define COMPONENT

#include <Box2D/Box2D.h>
#include <string>
enum {
    COMPONENT_NONE = 0,
    COMPONENT_PHYSICS_BODY = 1 << 0,
    COMPONENT_BODYTYPE = 1 << 1,
    COMPONENT_NAME = 1 << 2,
    COMPONENT_COORDINATES = 1 << 3,
    COMPONENT_BODYSIZE = 1 << 4,
    COMPONENT_SELECTED = 1 << 5,
    COMPONENT_VELOCITY = 1 << 6,
    COMPONENT_COLOR = 1 << 7,
    COMPONENT_MOTOR = 1 << 8,
    COMPONENT_TELEPORT = 1 << 9,
    COMPONENT_FAN = 1 << 10,
    SAVE_MASK = 1 << 11,
    ALREADY_CHECKED = 1 << 12,
    MOVE = 1 << 13,
    COLLISION = 1 << 14,
    RENDER_MASK = 1 << 15,
    QUANTITY_MASK = 1 << 16,
    BODY_DELETE = 1 << 17
} com;

enum {
    CIRCLE_COLOR_MASK = 0,
    WOOD_COLOR_MASK = 1,
    MOVING_PLATFORM_MIDDLE_COLOR_MASK = 2,
    ELEVATOR_MOTOR_COLOR_MASK = 3,
    ELEVATOR_PLATFORM_COLOR_MASK = 4,
    SPRING_MOTOR_COLOR_MASK = 5,
    SPRING_PLATFORM_COLOR_MASK = 6,
    TELEPORT_A_COLOR_MASK = 7,
    TELEPORT_B_COLOR_MASK = 8,
    FAN_COLOR_MASK = 9,
    FAN_BUTTON_COLOR_MASK = 10,
    FAN_BODY_COLOR_MASK = 11,
    DOMINO_COLOR_MASK = 12,
    WRECKING_BALL_COLOR_MASK = 13
}EntityColorMasks;

enum {
    EMPTY_TEXTURE = 16,
    CIRCLE_TEXURE_MASK = 0,
    WOOD_TEXTURE_MASK = 1,
    PLATFORM_TEXTURE_MASK = 2,
    ELEVATOR_MOTOR_TEXTURE_MASK = 3,
    ELEVATOR_PLATFORM_TEXTURE_MASK = 4,
    SPRING_MOTOR_TEXTURE_MASK = 5,
    SPRING_PLATFORM_TEXTURE_MASK = 6,
    TELEPORT_A_TEXTURE_MASK = 7,
    TELEPORT_B_TEXTURE_MASK = 8,
    FAN_WIND_TEXTURE_MASK = 9,
    FAN_BUTTON_TEXTURE_MASK = 10,
    FAN_BODY_TEXTURE_MASK = 11,
    DOMINO_TEXTURE_MASK = 12,
    WRECKING_BALL_TEXTURE_MASK = 13,
    WRECKING_BALL_PIN_TEXTURE_MASK = 14,
    CATAPULT_TEXTURE_MASK = 15,
    CATAPULT_PLATFORM_TEXTURE_MASK = 16,
    GOAL_TEXTURE_MASK = 17,
    WHITE_TEXTURE_MASK = 18,
    TEMPLATE_TEXTURE_MASK = 19
}EntityTextureMasks;

enum {
    START_FORM = 0,
    LEVEL_SELECT_FORM = 1,
    GAME_FORM = 2
}FormMasks;


struct Sticky{
    bool on_platform = false;
    int platform_id = 0;
};

struct Textures {
    int selected_texture = 0;
    int texture_mask;
    unsigned int texture_arr[7];
};

struct TemplateFlag{
    bool is_template = false;
};

struct JointLenght{
    float joint_lenght = 100;
};

struct EntityFlags {
    bool is_sensor = false;
    bool has_gravity = true;
    bool is_visible = true;
};

struct Color{
    float r = 1.f;
    float g = 1.f;
    float b = 1.f;
    float a = 1.f;
};

struct Inverted{
    bool is_inverted = false;
};

struct Colors {
    int selected_color = 0;
    int color_mask;
    Color color[3];
};

struct EntityFanFlags {
    bool is_inside = false;
    int fan_id;
};

struct Fan {
    bool is_reversed = false;
    bool is_on = false;
    int fan_body_id = 0;
    int fan_id = 0;
    int fan_button_id = 0;
};

struct Teleportation {
    bool is_teleporting = false;
    b2Vec2 coordinates;
    float angle = 0;
    b2Vec2 velocity;
};

struct Motor {
    bool has_platform = false;
};

struct Velocity{
    b2Vec2 vel;
};

struct Selected{
    bool selected = false;
};

struct BodyDelete{
    bool for_delete = false;
};

struct Coordinates {
    int x = 0;
    int y = 0;
    float angle = 0;
};

struct  PhysicsBody{
    b2Body *body;
};

struct Center {
    b2Vec2 center;
};

struct BodySize {
    int width = 0;
    int height = 0;
    int radius = 0;
};

struct BodyType {
    std::string type;
};

struct Name {
    std::string full_name;
    std::string name;
};

#endif // COMPONENT

