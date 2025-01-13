//
// Created by filip on 1/13/25.
//

#include "Chip8.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <random>





Chip8::Chip8(){
    for(uint8_t& element : memory){
        element = 0;
    }
    pc = 0x200;
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
        if(0x200 + fileSize >= memory_size){
            delete [] buffer;
            throw std::invalid_argument("File too big");            
        }
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
    // read from memory operation code and increment program counter
    opcode = (memory[pc & 0xFFF] << 8) | (memory[(pc+1)&0xFFF]);
    pc += 2;


    // TODO: execute that opcode?

    switch(opcode & 0xF000){
        case 0x0000:{
            switch(opcode & 0x000F){
                //00EE RET instruction
                case 0x000E:{       
                    pc = stack[sp];
                    sp--;
                    break;
                }
                //00E0 clear the display
                case 0x0000:{
                    // TODO
                    break;
                }

            }
            break;
        }
        // 1nnn jump to location 
        case 0x1000:{
            pc = (opcode&0x0FFF);
            break;
        }
        // 2nnn Call subroutine
        case 0x2000:{
            stack[sp]=pc;
            sp++;
            pc = (opcode & 0x0FFF);
            break;
        }
        //3xkk skip if Vx = kk
        case 0x3000:{
            uint8_t X = (opcode & 0x0F00) >> 8;
            if(V[X] == (opcode & 0x00FF)){
                pc+=2;
            }
            break;
        }
        //4xkk skip if Vx != kk
        case 0x4000:{
            uint8_t X = (opcode & 0x0F00) >> 8;
            if(V[X] != (opcode & 0x00FF)){
                pc+=2;
            }
            break;
        }
        //5xy0 skip if Vx=Vy
        case 0x5000:{
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            if(V[X] == V[Y]){
                pc+=2;
            }
            break;
        }
        //6xkk set Vx=kk;
        case 0x6000:{
            uint8_t X = (opcode & 0x0F00) >> 8;
            V[X]=(opcode&0x00FF);
            break;
        }
        //7xkk set Vx=Vx+kk
        case 0x7000:{
            uint8_t X = (opcode & 0x0F00) >> 8;
            V[X] = V[X] + (opcode & 0x00FF);
            break;
        }

        case 0x8000:{
            switch(opcode & 0x000F){
                //8xy0 set Vx = Vy
                case 0x0000:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    V[X] = V[Y];
                    break;
                }
                //8xy1 Vx = Vx OR Vy
                case 0x0001:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    V[X] = V[X] | V[Y];
                    break;
                }
                //8xy2 Vx=Vx AND Vy
                case 0x0002:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    V[X] = V[X] & V[Y];
                    break;
                }
                //8xy3 Vx = Vx XOR Vy
                case 0x0003:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    V[X] = V[X] ^ V[Y];
                    break;
                }
                //8xy4 - ADD Vx, Vy
                case 0x0004:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    if(V[X] + V[Y] > 255){
                        V[0xF]=1;
                    }else{
                        V[0xF]=0;
                    }
                    V[X] = (V[X] + V[Y])&0x00FF;
                    break;
                }
                //8xy5 Vx=Vx-Vy
                case 0x0005:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    if(V[X] > V[Y]){
                        V[0xF]=1;
                    }else{
                        V[0xF]=0;
                    }
                    V[X] = (V[X] - V[Y]);
                    break;
                }
                //8xy6 Vx=Vx >> 1
                case 0x0006:{
                    uint8_t X = (opcode & 0x0F00)>>8;
                    V[0xF]=(V[X]&0x1); // because of big endian
                    V[X] >>= 1;
                    break;
                }
                //8xy7 Vx=Vy-Vx
                case 0x0007:{
                    uint8_t X = (opcode & 0x0F00) >> 8;
                    uint8_t Y = (opcode & 0x00F0) >> 4;
                    if(V[Y] > V[X]){
                        V[0xF] = 1;
                    }else{
                        V[0xF] = 0;
                    }
                    V[X]=V[Y]-V[X];
                    break;
                }
                //8xyE Vx = Vx SHL 1
                case 0x000E:{
                    uint8_t X = (opcode & 0x0F00)>>8;
                    int mask = 1 << (sizeof(uint8_t)*8-1);
                    V[0xF]=V[X]&mask;
                    V[X] <<=1;
                    break;      
                }
            }

        }
        //9xy0 skip next instruction if Vx!=Vy
        case 0x9000:{
            uint8_t X = (opcode&0x0F00) >> 8;
            uint8_t Y = (opcode&0x00F0) >> 4;
            if(V[X] != V[Y]){
                pc+=2;
            }
            break;

        }
        //Annn set I=nnn
        case 0xA000:{
            uint16_t address = (opcode & 0x0FFF);
            I = address;
            break;
        }
        //Bnnn jump to nnn+V0
        case 0xB000:{
            uint16_t address = (opcode & 0x0FFF);
            pc=address+V[0];
            break;
        }
        //Cxkk Vx = random_byte & kk
        case 0xC000:{
            //generating random byte
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0,255);
            uint8_t randByte = dist(gen);
            
            uint8_t X = (opcode & 0x0F00)>>8;
            V[X] = randByte & (opcode & 0x00FF);
            break;
        }
        //Dxyn
        case 0xD000:{
            	uint8_t X = (opcode & 0x0F00) >> 8;
                uint8_t Y = (opcode & 0x00F0) >> 4;
                uint8_t height = opcode & 0x000Fu;

                // Wrap if going beyond screen boundaries
                uint8_t xPos = V[X] % SCREEN_WIDTH;
                uint8_t yPos = V[Y] % SCREEN_HEIGHT;

                V[0xF] = 0;
                for(int row = 0; row < height; row++){
                    uint8_t pixel = memory[I+height];
                    for(int col = 0; col < 8; col++){
                        if(pixel & (0x80 >> col) != 0)
                            if(video[(yPos+col)*SCREEN_WIDTH + (xPos+row)] == 1){
                                V[0xF] = 1;
                            }else{
                                V[0xF] = 0;
                            }
                            video[(yPos+col)*SCREEN_WIDTH + (xPos+row)] ^=1;
                    }
                }

                
                break;
        }
        case 0xE000:
        case 0xF000:

    }

    // TODO: delay timer (later sound timer)?




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

    /*
        while(not exist){
            chip8.cycle();
        }
    */

    return 0;
}