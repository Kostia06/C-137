#include "../include/argument.hpp"


Argument::Argument(vector<any> _valueTypes){
    this->_valueTypes = _valueTypes;
}

Argument::Argument(){
    this->_valueTypes = {};
}

Argument* Argument::nargs(int _nargs){
    this->_nargs = _nargs;
    return this;
}

Argument* Argument::help(string _help){
    this->_help = _help;
    return this;
}

Argument* Argument::required(){
    this->_required = true;
    return this;
}

Argument* Argument::remaining(){
    this->_remaining = true;
    return this;
}

Argument* Argument::order(){
    this->_order = true;
    return this;
}

string Argument::help(){
    return this->_help;
}
