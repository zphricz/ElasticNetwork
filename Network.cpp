#include <algorithm>
#include <cmath>
#include "Network.h"

using namespace Linear;
using namespace std;

Network::Network() {
}

Network::~Network() {
}

void Network::calculate_forces(float time_step, float repel_factor,
                               float attract_factor, float damp_factor) {
    for (int i = 0; i < num_nodes(); ++i) {
        // Repulsive force
        for (int j = i + 1; j < num_nodes(); ++j) {
            Vec2f delta_pos = nodes[j]->pos - nodes[i]->pos;
            float theta = delta_pos.theta();
            float force_mag = repel_factor / delta_pos.magnitude_square();
            Vec2f force_vec(theta);
            force_vec *= force_mag;
            nodes[i]->force -= force_vec;
            nodes[j]->force += force_vec;
        }

        // Dampening
        Vec2f force_vec = nodes[i]->vel * damp_factor;
        nodes[i]->force -= force_vec;
    }

    // Elastic force
    for (auto& connection: connections) {
        Node* n1 = connection.node_a;
        Node* n2 = connection.node_b;
        Vec2f delta_pos = n1->pos - n2->pos;
        float theta = delta_pos.theta();
        float force_mag = delta_pos.magnitude() * attract_factor;
        Vec2f force_vec(theta);
        force_vec *= force_mag;
        n2->force += force_vec;
        n1->force -= force_vec;
    }
}

void Network::move_nodes(float time_step) {
    for (Node*& node: nodes) {
        node->vel += node->force * time_step;
        node->pos += node->vel * time_step;
        node->force.zero();
    }
}

void Network::add_node(Node* node) {
    nodes.push_back(node);
}

void Network::add_node(float x_pos, float y_pos) {
    nodes.push_back(new Node(x_pos, y_pos));
}

int Network::num_nodes() {
    return nodes.size();
}

void Network::step(float time_step, float repel_factor,
                   float damp_factor, float attract_factor) {
    calculate_forces(time_step, repel_factor, attract_factor, damp_factor);
    move_nodes(time_step);
}

void Network::connect(Node * node_a, Node* node_b) {
    node_a->connect(node_b);
    connections.push_back({node_a, node_b});
}

