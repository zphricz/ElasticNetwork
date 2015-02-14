#include "Node.h"

using namespace std;

Node::Node(float x_pos, float y_pos) :
    pos(x_pos, y_pos),
    vel(0.0, 0.0),
    force(0.0, 0.0),
    main_connection(false) {
}

Node::~Node() {
}

void Node::connect(Node* other) {
    vector<Node*> v;
    if (other->main_connection) {
        main_connection = true;
        other->set_main_connection(v);
    } else if (main_connection) {
        other->main_connection = true;
        other->set_main_connection(v);
    }
    connections.push_back(other);
    other->connections.push_back(this);
}

void Node::set_main_connection(vector<Node*>& already_set) {
    vector<Node*> temp;
    for (Node*& node: connections) {
        bool found = false;
        for (Node*& other_node: already_set) {
            if (node == other_node) {
                found = true;
                break;
            }
        }
        if (!found && !node->main_connection) {
            node->main_connection = true;
            already_set.push_back(node);
            temp.push_back(node);
        }
    }
    for (Node*& node: temp) {
        node->set_main_connection(already_set);
    }
}

void Node::disconnect(Node* other) {
    unsigned int i;
    for (i = 0; i < connections.size(); ++i) {
        if (connections[i] == other) {
            break;
        }
    }
    connections.erase(connections.begin() + i);
}

void Node::remove_self() {
    for (Node*& sibling: connections) {
        sibling->disconnect(this);
    }
    connections.clear();
}

