#ifndef VARIABLEINFO_HPP
#define VARIABLEINFO_HPP

#include <vector>
#include <iostream>
#include "Utils.hpp"

class VariableInfo{
    private:
        Utils util;
        std::string toString();
    public:
        Type _type;
        int arraySize = 0;
        std::string _name;
        std::string _value;
        std::string _iter;
        std::vector<int> _size;
        std::vector<std::string> _dim;
        VariableInfo(){;}
        VariableInfo(const VariableInfo &source)
            : VariableInfo(source._name, source._type, source._value, source._dim, source._size, source.arraySize){;}
        VariableInfo(std::string name, Type type)
            : VariableInfo(name, type, "0"){;}
        VariableInfo(std::string name, Type type, std::string value)
            : VariableInfo(name, type, value, "."){;}
        VariableInfo(std::string name, Type type, std::string value, std::string iter);
        VariableInfo(std::string name, Type type, std::string value, std::vector<std::string> dim, std::vector<int> size)
            : VariableInfo(name, type, value, dim, size, 0){;};
        VariableInfo(std::string name, Type type, std::string value, std::vector<std::string> dim, std::vector<int> size, int arrSize);
        
        void print();
        void calculateSizes();
        void pushDim(std::string value);
};

#endif