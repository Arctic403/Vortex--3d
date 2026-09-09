#pragma once
#include "UUID.h"

class Object
{
public:
    UUID id;

    virtual ~Object() = default;
};
