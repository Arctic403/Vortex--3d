#include <iostream>
#include <string>
#include "mesh.h"

int main()
{
    Mesh mesh;

    std::cout << "Vortex3D Modeler v1\n";
    std::cout << "Terminal mode\n";

    std::string cmd;

    while(true)
    {
        std::cout << "vtx> ";
        std::getline(std::cin, cmd);

        if(cmd == "exit")
            break;

        if(cmd == "cube")
        {
            mesh.createCube(2.0f);
            std::cout << "Created cube: "
                      << mesh.vertices.size()
                      << " vertices\n";
        }
        else if(cmd == "stats")
        {
            std::cout << "Vertices: "
                      << mesh.vertices.size()
                      << " Faces: "
                      << mesh.faces.size()
                      << "\n";
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}
