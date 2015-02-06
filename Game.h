#ifndef GAME_H
#define GAME_H

#include "Network.h"
#include "Screen.h"

class Game {
    private:
        Network net; 
        Screen* scr;
        Vec2 center;
        double scale; // A scaling factor for drawing in units (meters per pixel)
        double max_scale;
        bool simulate;
        bool running;
        enum {AVERAGE_POSITION, BOUNDS} center_option;
        enum {NO_SCALE, SCALE, MAX_SCALE} scale_option;
        double time_step;
        int steps_per_frame;
        int selected_node;

        void handle_input();
        void set_scale_variables();
        void draw_network();
    public:
        Game(Screen* scr);
        ~Game();
        void run();
};

#endif
