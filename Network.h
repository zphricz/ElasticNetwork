#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Screen.h"
#include "Node.h"

struct Connection {
    Node *node_a;
    Node *node_b;
};

class Network {
    friend class Game;

  private:
    std::vector<Node *> nodes;
    std::vector<Connection> connections;

    void calculate_forces(float time_step, float repel_factor,
                          float attract_factor, float damp_factor);
    void move_nodes(float time_step);

  public:
    Network();
    ~Network();

    void add_node(float x_pos, float y_pos);
    void add_node(Node *node);

    int num_nodes();
    void step(float time_step, float repel_factor, float damp_factor,
              float attract_factor);
    void connect(Node *node_a, Node *node_b);
};

#endif
