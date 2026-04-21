#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Graph.h"


Graph loadGraphFromJson(const std::string& filepath){
    Graph g;
    std::ifstream file(filepath);
    if (!file.is_open()) return g;
    
    nlohmann::json j;
    file >> j; 
    for (const auto& node : j["nodes"]) {
        g.addNode(node["id"], node["name"], node["x"], node["y"]);
    }
    for (const auto& edge : j["edges"]) {
        g.addEdge(edge["from"], edge["to"]);
    }
    return g;
}

int main() {
    Graph blueprint = loadGraphFromJson("assets/data.json");
    blueprint.autoLayout();

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Blueprint Auto Layout Engine");
    window.setFramerateLimit(60);

    sf::Font font;
    if(!font.loadFromFile("assets/arial.ttf")){
        std::cerr << "Could not load arial.ttf\n";
    }

    const float NODE_WIDTH = 220.f;
    const float NODE_HEIGHT = 60.f;

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color(40, 40, 40));

        auto nodes = blueprint.getNodes();
        auto edges = blueprint.getEdges();

        for(const auto& pair : edges){
            int from_id = pair.first;
            for(int to_id : pair.second){
                Node fromNode = nodes[from_id];
                Node toNode = nodes[to_id];
                sf::Vector2f startPos(fromNode.x + NODE_WIDTH, fromNode.y + (NODE_HEIGHT / 2.f));
                sf::Vector2f endPos(toNode.x, toNode.y + (NODE_HEIGHT/2.f));

                sf::Vertex line[] = {
                    sf::Vertex(startPos, sf::Color(150, 150, 150)),
                    sf::Vertex(endPos, sf::Color(150, 150, 150))
                };
                window.draw(line, 2, sf::Lines);
            }
        }

        for(const auto& pair : nodes){
            Node n = pair.second;

            sf::RectangleShape rect(sf::Vector2f(NODE_WIDTH, NODE_HEIGHT));
            rect.setPosition(n.x, n.y);
            rect.setFillColor(sf::Color(60, 60, 60));
            rect.setOutlineColor(sf::Color(100, 150, 250));
            rect.setOutlineThickness(2.f);
            
            sf::Text text;
            text.setFont(font);
            text.setString(n.name);
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::White);
            text.setPosition(n.x + 10.f, n.y + 20.f);

            window.draw(rect);
            window.draw(text);
        }

        window.display();
    }

    return 0;
}