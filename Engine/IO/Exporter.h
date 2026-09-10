#pragma once

namespace Vortex
{
class Scene;

class Exporter
{
public:
    virtual ~Exporter() = default;
    [[nodiscard]] virtual bool Export(const Scene& scene, const char* path) = 0;
};
}
