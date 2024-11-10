#include <hamiltonian.h>
#include <mpi.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <highfive/highfive.hpp>
#include <iostream>

void listDatasets(const HighFive::Group& group,
                  const std::string& prefix = "") {
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
}

int main(int argc, char** argv) {
    int comm_rank, comm_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path relative_filepath("/src/dga_config.yaml");
    std::string config_path = (cwd / relative_filepath).string();

    HighFive::File file(
        "/home/julpe/Documents/OneDrive/Desktop/Data/1p-data.hdf5",
        HighFive::File::ReadOnly);
    auto test = file.listAttributeNames();
    auto test2 = file.listObjectNames();

    /*
for (size_t i = 0; i < hopping_elements_size; ++i) {
    std::cout << _kinetic_r_grid(i, xt::all(), xt::all(), xt::all())
              << "\n";
}*/

    YAML::Node config = YAML::LoadFile("dga_config.yaml");

    MPI_Finalize();
    return 0;
}