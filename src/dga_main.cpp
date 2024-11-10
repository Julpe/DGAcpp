#include <hamiltonian.h>
#include <input_parser.h>
#include <mpi.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <highfive/highfive.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // INITIALIZE MPI
    int comm_rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    // INITIALIZE LOGGING, MAX FILE SIZE IS 5MB
    std::shared_ptr<spdlog::logger> logger = spdlog::rotating_logger_st(
        "dga_logger", "dga_log.log", 5 * 1024 * 1024, 5);
    spdlog::set_default_logger(logger);

    // PARSING COMMAND LINE ARGUMENTS
    InputParser parser(argc, argv);

    // DGA CONFIG
    std::string dga_config_path =
        (std::filesystem::current_path() / "dga_config.yaml").string();
    if (parser.cmd_option_exists("-c")) {
        dga_config_path = std::filesystem::path(parser.get_cmd_option("-c"));
        logger->info("Custom config filepath provided, using \'{}\'.",
                     dga_config_path);
    } else {
        logger->info("Custom config filepath not set, defaulting to \'{}\'.",
                     dga_config_path);
    }

    try {
        YAML::Node config = YAML::LoadFile(dga_config_path);
    } catch (const YAML::BadFile& e) {
        logger->info("File {} could not be found.", dga_config_path);
        logger->debug(e.what());
        exit(1);
    } catch (const YAML::ParserException& e) {
        logger->info("File has invalid format.");
        logger->debug(e.what());
        exit(1);
    }

    // DGA INPUT DATA
    std::string one_p_data_path =
        (std::filesystem::current_path() / "1p-data.hdf5");
    if (parser.cmd_option_exists("-d")) {
        one_p_data_path = std::filesystem::path(parser.get_cmd_option("-d"));
        logger->info("Custom path to 1-p data provided, using \'{}\'.",
                     one_p_data_path);
    } else {
        logger->info("Custom path to 1-p data not set, defaulting to \'{}\'.",
                     one_p_data_path);
    }

    try {
        HighFive::File one_p_data_file(one_p_data_path,
                                       HighFive::File::ReadOnly);
    } catch (const HighFive::FileException& e) {
        logger->info("File {} could not be found.", one_p_data_path);
        logger->debug(e.what());
        exit(1);
    }

    // DGA INPUT DATA
    std::string g4iw_data_path =
        (std::filesystem::current_path() / "g4iw_sym.hdf5");
    if (parser.cmd_option_exists("-g")) {
        one_p_data_path = std::filesystem::path(parser.get_cmd_option("-g"));
        logger->info("Custom path to g4iw provided, using \'{}\'.",
                     g4iw_data_path);
    } else {
        logger->info("Custom path to g4iw not set, defaulting to \'{}\'.",
                     g4iw_data_path);
    }

    try {
        HighFive::File g4iw_data_file(g4iw_data_path, HighFive::File::ReadOnly);
    } catch (const HighFive::FileException& e) {
        logger->info("File {} could not be found.", g4iw_data_path);
        logger->debug(e.what());
        exit(1);
    }

    MPI_Finalize();
    return 0;
}