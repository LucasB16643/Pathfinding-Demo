// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "Grid.h"
#include "AStar.h"
#include "IPathfinder.h"
#include "BFS.h"

//void CreatePathfinder(Grid& grid, Node* start, Node* target);

enum class Algorithm
{
    AStar,
    BFS
};

IPathfinder* pathfinder = nullptr;
Algorithm currentAlgo = Algorithm::AStar;

void CreatePathfinder(Grid& grid, Node* start, Node* target)
{
    delete pathfinder;

    grid.ResetNodes();

    switch (currentAlgo)
    {
    case Algorithm::AStar:
        pathfinder = new AStarPathfinder(&grid, start, target);
        break;

    case Algorithm::BFS:
        pathfinder = new BFSPathfinder(&grid, start, target);
        break;
    }
};

const int numRows = 20;
const int numCols = 20;
const int tileSize = 40;




int main()
{
    InitWindow(numCols * tileSize, numRows * tileSize, "A* Pathfinding");

    Grid grid(numRows, numCols);

    Node* start = &grid.nodes[0][0];
    Node* target = &grid.nodes[19][19];

    std::vector<Node*> path;
    //Pathfinder* pathfinder = nullptr;

    Node* draggedNode = nullptr;
    bool isDraggingStart = false;
    bool isDraggingTarget = false;

    // Animation state
    int pathIndex = 0;
    float timer = 0.0f;
    const float delay = 0.05f;
 

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        Vector2 mouse = GetMousePosition();
        int gridX = mouse.x / tileSize;
        int gridY = mouse.y / tileSize;

        // Toggle obstacles

        {
            if (IsKeyPressed(KEY_ONE))
            {
                currentAlgo = Algorithm::AStar;
                CreatePathfinder(grid, start, target);
            }

            if (IsKeyPressed(KEY_TWO))
            {
                currentAlgo = Algorithm::BFS;
                CreatePathfinder(grid, start, target);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (gridX >= 0 && gridX < numCols &&
                    gridY >= 0 && gridY < numRows)
                {
                    Node* clicked = &grid.nodes[gridY][gridX];

                    if (clicked == start)
                    {
                        draggedNode = start;
                        isDraggingStart = true;
                    }
                    else if (clicked == target)
                    {
                        draggedNode = target;
                        isDraggingTarget = true;
                    }
                    else
                    {
                        // normal obstacle toggle
                        clicked->walkable = !clicked->walkable;

                        grid.ResetNodes();
                        CreatePathfinder(grid, start, target);
                        pathIndex = 0;
                        timer = 0.0f;
                    }
                }
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && draggedNode != nullptr)
            {
                if (gridX >= 0 && gridX < numCols &&
                    gridY >= 0 && gridY < numRows)
                {
                    Node* hovered = &grid.nodes[gridY][gridX];

                    if (hovered->walkable)
                    {
                        if (isDraggingStart && hovered != target)
                        {
                            start = hovered;
                        }
                        else if (isDraggingTarget && hovered != start)
                        {
                            target = hovered;
                        }
                    }
                }
            }

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                if (draggedNode != nullptr)
                {
                    grid.ResetNodes();
                    CreatePathfinder(grid, start, target);

                    pathIndex = 0;
                    timer = 0.0f;
                }

                draggedNode = nullptr;
                isDraggingStart = false;
                isDraggingTarget = false;
            }

            // Animate path drawing
            if (pathfinder && !pathfinder->IsFinished())
            {
                pathfinder->Step();
            }
            else if (pathfinder && pathfinder->PathFound())
            {
                auto path = pathfinder->GetPath();

                for (Node* node : path)
                    node->state = NodeState::Path;
            }

            // Draw
            BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int y = 0; y < numRows; y++)
            {
                for (int x = 0; x < numCols; x++)
                {
                    Node& node = grid.nodes[y][x];

                    Color color = LIGHTGRAY;

                    if (!node.walkable)
                        color = BLACK;
                    else if (&node == start)
                        color = GREEN;
                    else if (&node == target)
                        color = RED;
                    else if (node.state == NodeState::Open)
                        color = SKYBLUE;
                    else if (node.state == NodeState::Closed)
                        color = ORANGE;
                    else if (node.state == NodeState::Path)
                        color = DARKBLUE;

                    DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, color);
                    DrawRectangleLines(x * tileSize, y * tileSize, tileSize, tileSize, GRAY);
                }
            }
            const char* algoName =
                (currentAlgo == Algorithm::AStar) ? "A*" : "BFS";

            DrawText(algoName, 10, 10, 20, BLACK);

            EndDrawing();
        }
    }

        CloseWindow();
        return 0;
    }

   


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
