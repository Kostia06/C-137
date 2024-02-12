#pragma once

#include "../utils/include.hpp"


class ErrorHandler{
    private:
        std::vector<std::string> errors;
        std::map<std::string, std::string> files;
    public:
        ErrorHandler();
        
        std::string get_error(std::string file_name, size_t index, size_t length);
        void one_line(std::string error, std::string file_name, size_t index, size_t length);
        bool print_errors();
        void set_file(std::string file_name, std::string file);
};
