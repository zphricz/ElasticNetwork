#ifndef GAME_H
#define GAME_H

#include "Network.h"
#include "Screen.h"

enum class CenterType {AVERAGE_POSITION, BOUNDS};
enum class ScaleType {NO_SCALE, SCALE, MAX_SCALE};

class Game {
    private:
        Network net; 
        SoftScreen* scr;
        Linear::Vec2f center;
        float scale; // A scaling factor for drawing in units (meters per pixel)
        float max_scale;
        bool simulate;
        bool running;
        CenterType center_option;
        ScaleType scale_option;
        float time_step;
        int steps_per_frame;
        int selected_node;

        void handle_input();
        void set_scale_variables();
        void draw_network();
    public:
        Game(SoftScreen* scr);
        ~Game();
        void run();
};

#endif
