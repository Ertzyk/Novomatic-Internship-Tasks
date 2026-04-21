#include "Graph.h"
#include <queue>
#include <algorithm>

void Graph::addNode(int id, const std::string& name, int x, int y){
    nodes[id] = {id, name, x, y};
    // Ensure every node is tracked in the in-degree map, even if it has no connections
    if(in_degree.find(id) == in_degree.end()){
        in_degree[id] = 0;
    }
}

void Graph::addEdge(int from_id, int to_id){
    // update adjacency list and increment the target's incoming connection count
    adj_list[from_id].push_back(to_id);
    in_degree[to_id]++;
}

void Graph::autoLayout() {
    const int START_X = 100;
    const int START_Y = 200;
    const int SPACING_X = 250;
    const int SPACING_Y = 150;

    std::queue<int> q;
    std::unordered_map<int, int> current_in_degree = in_degree; 
    std::unordered_map<int, int> layers;
    std::unordered_map<int, int> layer_counts;

    // Seed the queue with nodes with no incoming connections
    for(const auto& pair: current_in_degree){
        if(pair.second == 0){
            q.push(pair.first);
            layers[pair.first] = 0;
        }
    }

    while (!q.empty()){
        int curr = q.front();
        q.pop();

        int curr_layer = layers[curr];

        // X coordinate is determined by depth
        nodes[curr].x = START_X + (curr_layer*SPACING_X);
        
        // Y coordinate is determined by how many nodes are already in this layer
        int y_index = layer_counts[curr_layer];
        nodes[curr].y = START_Y + (y_index*SPACING_Y);
        
        layer_counts[curr_layer]++; 

        for(int neighbor: adj_list[curr]){
            layers[neighbor] = std::max(layers[neighbor], curr_layer + 1);

            // Push child node to one layer deeper than their deepest parent node
            current_in_degree[neighbor]--;
            if(current_in_degree[neighbor] == 0){
                q.push(neighbor);
            }
        }
    }
}

const std::unordered_map<int, Node>& Graph::getNodes() const {
    return nodes;
}

const std::unordered_map<int, std::vector<int>>& Graph::getEdges() const {
    return adj_list;
}