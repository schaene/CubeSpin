#include <string.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include "CubeSpin_icons.h"
#include <math.h>

//setting stuff up
int scale = 15;
int xoffset = 64;
int yoffset = 32;

//get rotated dumbo lmao

double xrot = 0.0;
double yrot = 0.0;
double zrot = 0.0;

//get rotated (at a specific speed) dumbo lmao
double xrot_speed = .01;
double yrot_speed = .01;
double zrot_speed = .01;

//representation of c u b e
//         0---1
//        /|  /|
//       2---3 |
//       | 4-|-5
//       |/  |/
//       6---7
double cubePoints[8][3] = {
    {-1, -1, -1}, //point 0
    {1, -1, -1}, //point 1
    {-1, -1, 1}, //point 2
    {1, -1, 1}, //point 3
    {-1, 1, -1}, //point 4
    {1, 1, -1}, //point 5
    {-1, 1, 1}, //point 6
    {1, 1, 1} //point 7
};

double rotatedX;
double rotatedY;
double rotatedZ;
//function that does the rotating
void rotatePoints(double x, double y, double z, double ax, double ay, double az) {
    //rotate x axis
    rotatedY = (y * cos(ax)) - (z * sin(ax));
    rotatedZ = (y * sin(ax)) + (z * cos(ax));
    y = rotatedY;
    z = rotatedZ;

    //rotate y axis
    rotatedX = (z * sin(ay)) + (x * cos(ay));
    rotatedZ = (z * cos(ay)) - (x * sin(ay));
    x = rotatedX;
    z = rotatedZ;

    //rotate z axis
    rotatedX = (x * cos(az)) - (y * sin(az));
    rotatedY = (x * sin(az)) + (y * cos(az));
}

//the thing to draw to the screen, put draw commands here
static void app_draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);
    canvas_clear(canvas);

    canvas_draw_line(
        canvas,
        round(scale * cubePoints[0][0]) + xoffset,
        round(scale * cubePoints[0][1]) + yoffset,
        round(scale * cubePoints[1][0]) + xoffset,
        round(scale * cubePoints[1][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[0][0]) + xoffset,
        round(scale * cubePoints[0][1]) + yoffset,
        round(scale * cubePoints[2][0]) + xoffset,
        round(scale * cubePoints[2][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[2][0]) + xoffset,
        round(scale * cubePoints[2][1]) + yoffset,
        round(scale * cubePoints[3][0]) + xoffset,
        round(scale * cubePoints[3][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[3][0]) + xoffset,
        round(scale * cubePoints[3][1]) + yoffset,
        round(scale * cubePoints[1][0]) + xoffset,
        round(scale * cubePoints[1][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[0][0]) + xoffset,
        round(scale * cubePoints[0][1]) + yoffset,
        round(scale * cubePoints[4][0]) + xoffset,
        round(scale * cubePoints[4][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[1][0]) + xoffset,
        round(scale * cubePoints[1][1]) + yoffset,
        round(scale * cubePoints[5][0]) + xoffset,
        round(scale * cubePoints[5][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[2][0]) + xoffset,
        round(scale * cubePoints[2][1]) + yoffset,
        round(scale * cubePoints[6][0]) + xoffset,
        round(scale * cubePoints[6][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[3][0]) + xoffset,
        round(scale * cubePoints[3][1]) + yoffset,
        round(scale * cubePoints[7][0]) + xoffset,
        round(scale * cubePoints[7][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[4][0]) + xoffset,
        round(scale * cubePoints[4][1]) + yoffset,
        round(scale * cubePoints[5][0]) + xoffset,
        round(scale * cubePoints[5][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[5][0]) + xoffset,
        round(scale * cubePoints[5][1]) + yoffset,
        round(scale * cubePoints[7][0]) + xoffset,
        round(scale * cubePoints[7][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[7][0]) + xoffset,
        round(scale * cubePoints[7][1]) + yoffset,
        round(scale * cubePoints[6][0]) + xoffset,
        round(scale * cubePoints[6][1]) + yoffset);
    canvas_draw_line(
        canvas,
        round(scale * cubePoints[6][0]) + xoffset,
        round(scale * cubePoints[6][1]) + yoffset,
        round(scale * cubePoints[4][0]) + xoffset,
        round(scale * cubePoints[4][1]) + yoffset);
}

//flipper stuff i dont understand
static void app_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

int i = 0;
//main function
int32_t spin_moment(void* p) {
    UNUSED(p);
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Configure viewport
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, app_draw_callback, view_port);
    view_port_input_callback_set(view_port, app_input_callback, event_queue);

    // Register viewport in GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    InputEvent event;

    bool running = true;
    while(running) {
        //cube go nyoom
        xrot += xrot_speed;
        yrot += yrot_speed;
        zrot += zrot_speed;

        //attempting to rotate wish me luck
        for(i = 0; i < 8; i += 1) {
            rotatePoints(cubePoints[0][i], cubePoints[1][i], cubePoints[2][i], xrot, yrot, zrot);

            cubePoints[0][i] = rotatedX;
            cubePoints[1][i] = rotatedY;
            cubePoints[2][i] = rotatedZ;
        }
        view_port_update(view_port);
        furi_delay_ms(160);

        if(furi_message_queue_get(event_queue, &event, 100) == FuriStatusOk) {
            if((event.type == InputTypePress) || (event.type == InputTypeRepeat)) {
                switch(event.key) {
                case InputKeyUp:

                    break;

                case InputKeyDown:

                    break;

                case InputKeyLeft:

                    break;

                case InputKeyRight:

                    break;

                case InputKeyOk:

                    break;
                //exits the program if back is pressed
                default:
                    running = false;
                    break;
                }
            }
        }
    }
    //cleanup go brrrrr
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_record_close(RECORD_GUI);

    return 0;
}
