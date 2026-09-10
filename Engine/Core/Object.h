#pragma once
#include "UUID.h"

namespace Vortex
{
class Object
{
public:
    virtual ~Object() = default;

    UUID GetID() const
    {
        return id;
    }

protected:
    UUID id;
};
}
