#pragma once
#include "UUID.h"

namespace Vortex
{
class Object
{
public:
    Object() = default;
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;
    virtual ~Object() = default;

    [[nodiscard]] UUID GetID() const noexcept
    {
        return id;
    }

protected:
    UUID id;
};
}
