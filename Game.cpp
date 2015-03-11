#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include "Game.h"

using namespace std;

const int node_radius = 4;
const float BORDER_SIZE = 0.2;
const int move_factor = 5;

static float repel_factor = 100.0;
static float attract_factor = 1.0E-4;
static float damp_factor = .5;

void Game::handle_input() {
    static bool left_pressed = false;
    static bool right_pressed = false;
    static bool up_pressed = false;
    static bool down_pressed = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            running = false;
            break;
        }
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: {
                running = false;
                break;
            }
            case SDLK_BACKSPACE: {
                if (net.num_nodes() == 1) {
                    running = false;
                }
                net.nodes[selected_node]->remove_self();
                for (unsigned int i = 0; i < net.connections.size(); ++i) {
                    if (net.connections[i].node_a == net.nodes[selected_node]) {
                        net.connections.erase(net.connections.begin() + i);
                        i--;
                    }
                }
                net.nodes.erase(net.nodes.begin() + selected_node);
                if (selected_node >= net.num_nodes()) {
                    selected_node = net.num_nodes() - 1;
                }
                break;
            }
            case SDLK_RETURN: {
                simulate = !simulate;
                break;
            }
            case SDLK_LSHIFT:
            case SDLK_RSHIFT: {
                selected_node++;
                selected_node %= net.num_nodes();
                break;
            }
            case SDLK_LEFT: {
                left_pressed = true;
                break;
            }
            case SDLK_RIGHT: {
                right_pressed = true;
                break;
            }
            case SDLK_UP: {
                up_pressed = true;
                break;
            }
            case SDLK_DOWN: {
                down_pressed = true;
                break;
            }
            case SDLK_1: {
                time_step /= 2.0;
                break;
            }
            case SDLK_2: {
                time_step *= 2.0;
                break;
            }
            case SDLK_3: {
                scale_option = ScaleType::NO_SCALE;
                break;
            }
            case SDLK_4: {
                scale_option = ScaleType::SCALE;
                break;
            }
            case SDLK_5: {
                scale_option = ScaleType::MAX_SCALE;
                max_scale = scale;
                break;
            }
            case SDLK_6: {
                center_option = CenterType::AVERAGE_POSITION;
                break;
            }
            case SDLK_7: {
                center_option = CenterType::BOUNDS;
                break;
            }
            case SDLK_8: {
                damp_factor /= 2.0;
                if (damp_factor < 0.0) {
                    damp_factor = 0.0;
                }
                break;
            }
            case SDLK_9: {
                damp_factor *= 2.0;
                if (damp_factor > 1.0) {
                    damp_factor = 1.0;
                }
                break;
            }
            case SDLK_0: {
                scr->toggle_recording();
            }
            default: {
                break;
            }
            }
            break;
        }
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT: {
                left_pressed = false;
                break;
            }
            case SDLK_RIGHT: {
                right_pressed = false;
                break;
            }
            case SDLK_UP: {
                up_pressed = false;
                break;
            }
            case SDLK_DOWN: {
                down_pressed = false;
                break;
            }
            default: {
                break;
            }
            }
        }
        default: {
             break;
        }
        }
    }
    if (up_pressed) {
        net.nodes[selected_node]->pos.y += move_factor * scale;
    }
    if (down_pressed) {
        net.nodes[selected_node]->pos.y -= move_factor * scale;
    }
    if (left_pressed) {
        net.nodes[selected_node]->pos.x -= move_factor * scale;
    }
    if (right_pressed) {
        net.nodes[selected_node]->pos.x += move_factor * scale;
    }
}

Game::Game(SoftScreen* scr) : 
    scr(scr),
    simulate(true),
    running(true),
    center_option(CenterType::AVERAGE_POSITION),
    scale_option(ScaleType::SCALE),
    time_step(0.4),
    steps_per_frame(10),
    selected_node(0) {
    srand(time(0));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-400, 400);
    float x_pos = dis(gen);
    float y_pos = dis(gen);
    Node * main = new Node(x_pos, y_pos);
    main->main_connection = true;
    net.add_node(main);
    for (int i = 0; i < 125; ++i) {
        x_pos = dis(gen);
        y_pos = dis(gen);
        net.add_node(x_pos, y_pos);
    }
#if 0
    for (int i = 0; i < net.num_nodes(); ++i) {
        int j = rand() % net.num_nodes();
        if (i == j)
            continue;
        net.connect(net.nodes[i], net.nodes[j]);
    }
    for (int i = 0; i < net.num_nodes(); ++i) {
        if (!net.nodes[i]->main_connection) {
            int j = rand() % net.num_nodes();
            while (!net.nodes[j]->main_connection) {
                j = rand() % net.num_nodes();
            }
            net.connect(net.nodes[i], net.nodes[j]);
        }
    }
#else
    for (int i = 0; i < net.num_nodes(); ++i) {
        for (int j = i + 1; j < net.num_nodes(); ++j) {
            net.connect(net.nodes[i], net.nodes[j]);
        }
    }
#endif
    set_scale_variables();
    scr->set_recording_style("images", 5);
}

Game::~Game() {
}

void Game::set_scale_variables() {
    float right_bound = net.nodes[0]->pos.x;
    float left_bound = net.nodes[0]->pos.x;
    float up_bound = net.nodes[0]->pos.y;
    float down_bound = net.nodes[0]->pos.y;
    if (center_option == CenterType::AVERAGE_POSITION) {
        center = net.nodes[0]->pos;
    }
    for (int i = 1; i < net.num_nodes(); ++i) {
        if (net.nodes[i]->pos.x > right_bound) {
            right_bound = net.nodes[i]->pos.x;
        }
        if (net.nodes[i]->pos.x < left_bound) {
            left_bound = net.nodes[i]->pos.x;
        }
        if (net.nodes[i]->pos.y > up_bound) {
            up_bound = net.nodes[i]->pos.y;
        }
        if (net.nodes[i]->pos.y < down_bound) {
            down_bound = net.nodes[i]->pos.y;
        }
        if (center_option == CenterType::AVERAGE_POSITION) {
            center += net.nodes[i]->pos;
        }
    }
    if (right_bound == left_bound) {
        float diff = abs(right_bound) * .05;
        right_bound += diff + 10.0;
        left_bound -= diff + 10.0;
    }
    if (up_bound == down_bound) {
        float diff = abs(up_bound) * .05;
        up_bound += diff + 10.0;
        down_bound -= diff + 10.0;
    }
    if (center_option == CenterType::AVERAGE_POSITION) {
        center /= net.num_nodes();
    } else {
        center.x = (right_bound + left_bound) / 2.0;
        center.y = (up_bound + down_bound) / 2.0;
    }
    float max_x = max(abs(right_bound - center.x), abs(left_bound - center.x));
    float max_y = max(abs(up_bound - center.y), abs(down_bound - center.y));
    float scale_x = max_x * (2.0 + BORDER_SIZE) / scr->width;
    float scale_y = max_y * (2.0 + BORDER_SIZE) / scr->height;
    scale = max(scale_x, scale_y);
    max_scale = max(scale, max_scale);
    if (scale_option == ScaleType::MAX_SCALE) {
        scale = max_scale;
    }
}

void Game::draw_network() {
    scr->cls();
    // Perform Scaling
    if (scale_option != ScaleType::NO_SCALE) {
        set_scale_variables();
    }

    // Draw connections
    scr->set_color(100, 100, 100);
    for (Connection& connection: net.connections) {
        int x1 = lrint((connection.node_a->pos.x - center.x) / scale);
        int y1 = lrint((connection.node_a->pos.y - center.y) / scale);
        int x2 = lrint((connection.node_b->pos.x - center.x) / scale);
        int y2 = lrint((connection.node_b->pos.y - center.y) / scale);
        scr->draw_line(scr->width / 2 + x1, scr->height / 2 - y1, 
                       scr->width / 2 + x2, scr->height / 2 - y2);
    }
    // Fill in nodes
    scr->set_color(0, 255, 0);
    for (Node*& node: net.nodes) {
        int x = lrint((node->pos.x - center.x) / scale);
        int y = lrint((node->pos.y - center.y) / scale);
        scr->fill_circle(scr->width / 2 + x, scr->height / 2 - y, node_radius);
    }

    // Draw Selected Node with blinking crosshair
    int x = lrint((net.nodes[selected_node]->pos.x - center.x) / scale);
    int y = lrint((net.nodes[selected_node]->pos.y - center.y) / scale);
    static int i = 0;
    if (i >= 10) {
        scr->fill_circle(scr->width / 2 + x, scr->height / 2 - y, node_radius + 1,
                         {200, 200, 200});
    }
    if (++i == 20) {
        i = 0;
    }
}

// Currently the selected node is not gauranteed to remiain the same over a single
// step of simulation. Change this
void Game::run() {
    while (running) {
        if (simulate) {
            for (int i = 0; i < steps_per_frame; ++i) {
                net.step(time_step, repel_factor, damp_factor, attract_factor);
            }
        }
        draw_network();
        scr->commit();
        handle_input();
    }
}

