#include <filesystem>
#include <iostream>

#include "highfive/highfive.hpp"
#include "mpi.h"
#include "xtensor/xarray.hpp"
#include "yaml-cpp/yaml.h"

using namespace std;
/*
void listDatasets(const HighFive::Group& group, const string& prefix = "") {
    // List objects in the current group
    for (const auto& name : group.listObjectNames()) {
        // Construct the full path
        std::string fullPath = prefix + "/" + name;

        // Check if the object is a dataset or another group
        if (group.getObjectType(name) == HighFive::ObjectType::Dataset) {
            std::cout << "Dataset: " << fullPath << std::endl;
        } else if (group.getObjectType(name) == HighFive::ObjectType::Group) {
            std::cout << "Group: " << fullPath << std::endl;
            // Recursively list contents of sub-groups
            listDatasets(group.getGroup(name), fullPath);
        }
    }
}*/

int main(int argc, char** argv) {
    int comm_rank, comm_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    filesystem::path cwd = filesystem::current_path();
    filesystem::path relative_filepath("/src/dga_config.yaml");
    string config_path = (cwd / relative_filepath).string();

    YAML::Node config = YAML::LoadFile("dga_config.yaml");

    MPI_Finalize();
    return 0;
}