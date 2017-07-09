#include "VariableInfo.hpp"

VariableInfo::VariableInfo(std::string name, Type type, std::string value, std::string iter){
    _name = name;
    _type = type;
    _value = value;
    _iter = iter;
}

VariableInfo::VariableInfo(std::string name, Type type, std::string value, std::vector<std::string> dim, std::vector<int> size, int arrSize){
    _name = name;
    _type = type;
    _value = value;
    for(auto c :dim){
        _dim.push_back(c);
    }
    for(auto c :size){
        _size.push_back(c);
    }
    this->arraySize = arrSize;
}

std::string VariableInfo::toString(){
    return _name + "\t" + util.TypeToString(_type) + "\t" + _value+ "\t" + util.toString(arraySize)+"\n";
}

void VariableInfo::print(){
    std::cout <<"name\t type\tValue\tarraySize\n";
    std::cout << this->toString();
    std::cout <<"dim\t";
    for(auto c :_dim)
        std::cout <<c<<"\t";
    std::cout <<std::endl << "size*4\t";
    for(auto c :_size)
        std::cout <<c<<"\t";
    std::cout << std::endl;
}

void VariableInfo::calculateSizes(){
    for(int i = 0; i < _dim.size(); i++){
        int sizeTemp = 1;
        for(int j = i+1; j < _dim.size(); j++)
            sizeTemp *= std::atoi(_dim[j].c_str());
        _size.push_back(sizeTemp*4);
    }
    if(_dim.size() == 0) return;
    arraySize = 1;
    for(auto c : _dim){
        arraySize *= std::atoi(c.c_str());
    }
}

void VariableInfo::pushDim(std::string value){
    _dim.push_back(value);
}