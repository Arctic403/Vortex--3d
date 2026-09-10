#pragma once
#include <memory>

namespace Vortex
{
class Scene;

class Importer
{
public:
    virtual ~Importer() = default;
    [[nodiscard]] virtual std::unique_ptr<Scene> Import(const char* path) = 0;
};
}
