#pragma once
#include <vector>
#include "Node.h"

class Grid
{
public:
    std::vector<std::vector<Node>> nodes;
    int rows;
    int cols;

    Grid(int r, int c)
        : rows(r), cols(c)
    {
        nodes.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            nodes[i].reserve(cols);
            for (int j = 0; j < cols; j++)
            {
                nodes[i].emplace_back(i, j);
            }
        }
    }

    std::vector<Node*> GetNeighbors(Node* node)
    {
        std::vector<Node*> neighbors;

        int r = node->row;
        int c = node->col;

        if (r > 0)           neighbors.push_back(&nodes[r - 1][c]);
        if (r < rows - 1)    neighbors.push_back(&nodes[r + 1][c]);
        if (c > 0)           neighbors.push_back(&nodes[r][c - 1]);
        if (c < cols - 1)    neighbors.push_back(&nodes[r][c + 1]);

        return neighbors;
    }

    void ResetNodes()
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                Node& node = nodes[r][c];

                node.state = NodeState::Default;
                node.gCost = 0.0f;
                node.hCost = 0.0f;
                node.parent = nullptr;
            }
        }
    }
};
