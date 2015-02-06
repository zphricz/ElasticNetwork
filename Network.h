#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Screen.h"
#include "Node.h"

struct Connection {
    Node * node_a;
    Node * node_b;
};

class Network {
    friend class Game;
    private:
        std::vector<Node*> nodes;
        std::vector<Connection> connections;

        void calculate_forces(double time_step, double repel_factor,
                              double attract_factor, double damp_factor);
        void move_nodes(double time_step, double damp_factor);
    public:
        Network();
        ~Network();

        void add_node(double x_pos, double y_pos);
        void add_node(Node* node);

        int num_nodes();
        void step(double time_step, double repel_factor, double damp_factor,
                  double attract_factor);
        void connect(Node* node_a, Node * node_b);
};

#endif
