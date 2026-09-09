#pragma once

class Scene;

class Exporter
{
public:
    virtual ~Exporter() = default;
    virtual bool Export(Scene* scene, const char* path) = 0;
};
