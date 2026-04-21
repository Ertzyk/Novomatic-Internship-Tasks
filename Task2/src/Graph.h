#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Node.h"

class Graph {
private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<int>> adj_list;
    std::unordered_map<int, int> in_degree;

public:
    void addNode(int id, const std::string& name, int x = 0, int y = 0);
    void addEdge(int from_id, int to_id);
    void autoLayout();
    
    const std::unordered_map<int, Node>& getNodes() const;
    const std::unordered_map<int, std::vector<int>>& getEdges() const;
};