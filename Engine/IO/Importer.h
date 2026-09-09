#pragma once

class Scene;

class Importer
{
public:
    virtual ~Importer() = default;
    virtual Scene* Import(const char* path) = 0;
};
