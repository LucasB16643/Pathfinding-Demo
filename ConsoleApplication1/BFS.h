#pragma once
#include "Grid.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "IPathfinder.h"
#include <queue>
#include <unordered_set>

class BFSPathfinder : public IPathfinder
{
public:
    Grid* grid;
    Node* start;
    Node* target;

    std::queue<Node*> frontier;
    std::unordered_set<Node*> visited;

    bool finished = false;
    bool pathFound = false;

    BFSPathfinder(Grid* g, Node* s, Node* t)
        : grid(g), start(s), target(t)
    {
        frontier.push(start);
        visited.insert(start);
    }

    void Step() override
    {
        if (frontier.empty())
        {
            finished = true;
            return;
        }

        Node* current = frontier.front();
        frontier.pop();

        current->state = NodeState::Closed;

        if (current == target)
        {
            finished = true;
            pathFound = true;
            return;
        }

        for (Node* neighbor : grid->GetNeighbors(current))
        {
            if (!neighbor->walkable || visited.count(neighbor))
                continue;

            neighbor->parent = current;
            visited.insert(neighbor);
            frontier.push(neighbor);

            neighbor->state = NodeState::Open;
        }
    }

    bool IsFinished() const override { return finished; }
    bool PathFound() const override { return pathFound; }

    std::vector<Node*> GetPath() override
    {
        std::vector<Node*> path;
        Node* current = target;

        while (current != start)
        {
            path.push_back(current);
            current = current->parent;
        }

        std::reverse(path.begin(), path.end());
        return path;
    }
};
