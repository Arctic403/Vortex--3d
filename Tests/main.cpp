#include <iostream>
#include "../Engine/Core/Logger.h"
#include "../Engine/Core/EngineVersion.h"

int main()
{
    Vortex::Logger::Info("Vortex3D foundation test");

    std::cout
        << "Version "
        << Vortex::EngineVersion::Major
        << "."
        << Vortex::EngineVersion::Minor
        << "."
        << Vortex::EngineVersion::Patch
        << std::endl;

    std::cout << "Foundation tests passed" << std::endl;

    return 0;
}
