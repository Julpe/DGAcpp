#pragma once

#include <algorithm>
#include <string>
#include <vector>

class InputParser {
   private:
    std::vector<std::string> _tokens;

   public:
    InputParser();
    InputParser(int& argc, char** argv);
    const std::string& get_cmd_option(const std::string& option);
    bool cmd_option_exists(const std::string& option);
};