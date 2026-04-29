#pragma once
#include <vector>
#include "Grid.h"
class IPathfinder
{
public:
    virtual void Step() = 0;
    virtual bool IsFinished() const = 0;
    virtual bool PathFound() const = 0;
    virtual std::vector<Node*> GetPath() = 0;
    virtual ~IPathfinder() {}
};