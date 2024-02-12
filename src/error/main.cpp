#include "include.hpp"

ErrorHandler::ErrorHandler(){}

std::string ErrorHandler::get_error(std::string file_name, size_t index, size_t length){
    std::string file = files[file_name];
    std::string error = "";
    bool found = false;
    size_t line = 1;
    size_t start_of_line = 0;

    for(size_t i = 0; i < file.length(); i++){
        if(file[i] == '\n' && !found){ 
            error.erase(); 
            line++; 
            start_of_line = i; 
        }
        else if(file[i] == '\n' && found){ break; }
        else{ error += file[i]; }
        if(i == index){ found = true; }
    }

    size_t line_index = index - start_of_line + 3;
    size_t underline_size = std::min( error.size() - line_index + 3, length-1); 
    error = std::to_string(line) + " | " + error + "\n";
    error += RED;
    error +=  "" + std::string(line_index, ' ') + "^" + std::string(underline_size, '~') + "\n";
    error += RESET;
    return error;

}

void ErrorHandler::one_line(std::string error, std::string file_name, size_t index, size_t length){
    std::string error_msg = "";
    error_msg += RED;
    error_msg += "error: ";
    error_msg += WHITE;
    error_msg += error;
    if(file_name != ""){
        error_msg += " in file " + file_name + "\n";
        error_msg += get_error(file_name, index, length);     
    }
    errors.push_back(error_msg); 
}

bool ErrorHandler::print_errors(){
    bool has_errors = false;
    for(std::string error: errors){
        std::cout << error << std::endl;
        has_errors = true;
    }
    return has_errors;
}
    
void ErrorHandler::set_file(std::string file_name, std::string file){
    files[file_name] = file;
}
