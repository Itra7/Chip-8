//
// Created by filip on 1/13/25.
//

#include "Chip8.hpp"

#include <fstream>
#include <string>
#include <iostream>

Chip8::Chip8(){
}

void Chip8::LoadRom(const std::string pathtofile){

    std::ifstream file(pathtofile, std::ios::binary);
    std::streampos fileSize;

    if(file.is_open()){
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        char* buffer = new char[fileSize];
        file.read(buffer, fileSize);
        for(int i = 0; i < fileSize; i++){
          memory[0x200+i]=buffer[i];
        }

        delete [] buffer;
    }else{
        throw std::invalid_argument("Chip8::LoadRom: file not found");
    }
    file.close();
}

void Chip8::Cycle() {

}


void Chip8::opcode_0nnn(){

}



int main(int argc,char *argv[]){

    if(argc != 2){
      std::cout << "Chip8::main: Wrong number of arguments" << std::endl;
      return 0;
    }
    Chip8 chip8;
    try{
        chip8.LoadRom(std::string(argv[1]));
    }catch(std::invalid_argument& e){
         std::cout << e.what() << std::endl;
         return 1;
    }

    return 0;
}