#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <assert.h>
#include <any>

#include "error.hpp"

using namespace std;

class Argument: public Error{
    private:
        vector<any> _valueTypes;
        vector<any> _values;
        int _nargs = 0;
        bool _required = false;
        bool _remaining = false;
        bool _order = false;
        string _help = "";
    public:
        Argument(vector<any> _valueTypes);
        Argument();
        // changing the parameters
        Argument* nargs(int _nargs);
        Argument* help(string _help);
        Argument* required();
        Argument* remaining();
        Argument* order();
        string help();
};
