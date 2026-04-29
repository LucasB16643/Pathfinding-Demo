#pragma once
#include "Grid.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "IPathfinder.h"

inline float Heuristic(Node* a, Node* b)
{
    return abs(a->row - b->row) + abs(a->col - b->col);
}

inline std::vector<Node*> RetracePath(Node* start, Node* end)
{
    std::vector<Node*> path;
    Node* current = end;

    while (current != start)
    {
        path.push_back(current);
        current = current->parent;
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

inline std::vector<Node*> FindPath(Grid& grid, Node* start, Node* target)
{
    grid.ResetNodes();

    std::vector<Node*> openSet;
    std::vector<Node*> closedSet;

    start->gCost = 0;
    start->hCost = Heuristic(start, target);

    openSet.push_back(start);

    while (!openSet.empty())
    {
        Node* current = openSet[0];

        for (Node* node : openSet)
        {
            if (node->fCost() < current->fCost() ||
                (node->fCost() == current->fCost() && node->hCost < current->hCost))
            {
                current = node;
            }
        }

        openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
        closedSet.push_back(current);

        if (current == target)
            return RetracePath(start, target);

        for (Node* neighbor : grid.GetNeighbors(current))
        {
            if (!neighbor->walkable ||
                std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end())
                continue;

            float newCost = current->gCost + 1.0f;

            if (newCost < neighbor->gCost ||
                std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
            {
                neighbor->gCost = newCost;
                neighbor->hCost = Heuristic(neighbor, target);
                neighbor->parent = current;

                if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
                    openSet.push_back(neighbor);
            }
        }
    }

    return {}; // no path found
}

class AStarPathfinder : public IPathfinder
{

public:
    Grid* grid;
    Node* start;
    Node* target;

    std::vector<Node*> openSet;
    std::vector<Node*> closedSet;

    bool finished = false;
    bool pathFound = false;

    AStarPathfinder(Grid* g, Node* s, Node* t)
        : grid(g), start(s), target(t)
    {
        openSet.push_back(start);
    }

    void Step() override {
            if (openSet.empty())
            {
                finished = true;
                return;
            }

            // Get lowest fCost node
            Node* current = openSet[0];
            for (Node* node : openSet)
            {
                if (node->fCost() < current->fCost() ||
                    (node->fCost() == current->fCost() && node->hCost < current->hCost))
                {
                    current = node;
                }
            }

            // Move current from open → closed
            openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
            closedSet.push_back(current);

            current->state = NodeState::Closed;

            // Reached goal
            if (current == target)
            {
                finished = true;
                pathFound = true;
                return;
            }

            for (Node* neighbor : grid->GetNeighbors(current))
            {
                if (!neighbor->walkable ||
                    std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end())
                    continue;

                float newCost = current->gCost + 1;

                bool inOpen = std::find(openSet.begin(), openSet.end(), neighbor) != openSet.end();

                if (newCost < neighbor->gCost || !inOpen)
                {
                    neighbor->gCost = newCost;
                    neighbor->hCost = abs(neighbor->row - target->row) + abs(neighbor->col - target->col);
                    neighbor->parent = current;

                    if (!inOpen)
                    {
                        openSet.push_back(neighbor);
                        neighbor->state = NodeState::Open;
                    }
                }
            }
        
    }
    std::vector<Node*> RetracePath()
    {
        std::vector<Node*> path;
        Node* current = target;

        while (current != start)
        {
            path.push_back(current);
            current = current->parent;
        }

        std::reverse(path.begin(), path.end());

        for (Node* node : path)
            node->state = NodeState::Path;

        return path;
    }

    bool IsFinished() const override { return finished; }
    bool PathFound() const override { return pathFound; }
    std::vector<Node*> GetPath() override { return RetracePath(); }
};
