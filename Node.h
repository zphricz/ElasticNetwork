#ifndef NODE_H
#define NODE_H

#include "Vec.h"

#include <vector>

class Node {
    friend class Network;
    friend class Game;
    private:
        Vec2f pos;
        Vec2f vel;
        Vec2f force;
        std::vector<Node*> connections;
        bool main_connection;
    public:
        Node(float x_pos, float y_pos);
        ~Node();

        void connect(Node* other);
        void set_main_connection(std::vector<Node*>& already_set);
        void disconnect(Node* other);
        void remove_self();

        //Node(Node&& other);
        Node& operator=(const Node& other);
};

#endif
