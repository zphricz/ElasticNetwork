#include <algorithm>
#include <cmath>
#include "Network.h"

using std::vector;
using std::max;

Network::Network() {
}

Network::~Network() {
}

void Network::calculate_forces(double time_step, double repel_factor,
                               double attract_factor, double damp_factor) {
    for (int i = 0; i < num_nodes(); ++i) {
        // Repulsive force
        for (int j = i + 1; j < num_nodes(); ++j) {
            Vec2 delta_pos = nodes[j]->pos - nodes[i]->pos;
            double theta = delta_pos.angle();
            double force_mag = repel_factor / delta_pos.magnitude_square();
            Vec2 force_vec(force_mag * cos(theta), force_mag * sin(theta));
            nodes[i]->force -= force_vec;
            nodes[j]->force += force_vec;
        }
        // Elastic force
        for (Node*& connected_node: nodes[i]->connections) {
            Vec2 delta_pos = connected_node->pos - nodes[i]->pos;
            double theta = delta_pos.angle();
            double force_mag = delta_pos.magnitude() * attract_factor;
            Vec2 force_vec(force_mag * cos(theta), force_mag * sin(theta));
            nodes[i]->force += force_vec;
            connected_node->force -= force_vec;
        }

        // Dampening
        Vec2 force_vec = nodes[i]->vel * damp_factor;
        nodes[i]->force -= force_vec;
    }
}

void Network::move_nodes(double time_step, double damp_factor) {
    for (Node*& node: nodes) {
        node->vel += (node->force * time_step);
        node->pos += (node->vel * time_step);
        node->force.zero();
    }
}

void Network::add_node(Node* node) {
    nodes.push_back(node);
}

void Network::add_node(double x_pos, double y_pos) {
    nodes.push_back(new Node(x_pos, y_pos));
}

int Network::num_nodes() {
    return nodes.size();
}

void Network::step(double time_step, double repel_factor,
                   double damp_factor, double attract_factor) {
    calculate_forces(time_step, repel_factor, attract_factor, damp_factor);
    move_nodes(time_step, damp_factor);
}

void Network::connect(Node * node_a, Node* node_b) {
    node_a->connect(node_b);
    connections.push_back({node_a, node_b});
}

