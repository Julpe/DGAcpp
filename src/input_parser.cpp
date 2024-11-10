#include <input_parser.h>

InputParser::InputParser() {}

InputParser::InputParser(int& argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        this->_tokens.push_back(std::string(argv[i]));
    }
}

const std::string& InputParser::get_cmd_option(const std::string& option) {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(this->_tokens.begin(), this->_tokens.end(), option);
    if (itr != this->_tokens.end() && ++itr != this->_tokens.end()) {
        return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
}

bool InputParser::cmd_option_exists(const std::string& option) {
    return std::find(this->_tokens.begin(), this->_tokens.end(), option) !=
           this->_tokens.end();
}
