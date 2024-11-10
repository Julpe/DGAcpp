#pragma once

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#include <tuple>

class ConfigBase {
   private:
    std::shared_ptr<spdlog::logger> _logger;

   public:
    ConfigBase();
};

class BoxSizeConfig : public ConfigBase {
   public:
    uint8_t niw_core, niv_core, niv_shell, niv_asympt;
    BoxSizeConfig(uint8_t niw_core, uint8_t niv_core, uint8_t niv_shell,
                  uint8_t niv_asympt);
};

class LatticeConfig : public ConfigBase {
   public:
    std::tuple<uint8_t, uint8_t, uint8_t> nk, nq;
    LatticeConfig(std::tuple<uint8_t, uint8_t, uint8_t> nk,
                  std::tuple<uint8_t, uint8_t, uint8_t> nq);
};