#pragma once

enum class NodeState
{
    Default,
    Open,
    Closed,
    Path
};

class Node
{
public:
    int row;
    int col;

    bool walkable = true;

    NodeState state = NodeState::Default;

    float gCost = 0.0f;
    float hCost = 0.0f;
    Node* parent = nullptr;

    Node(int r = 0, int c = 0)
        : row(r), col(c) {
    }

    float fCost() const { return gCost + hCost; }
};
