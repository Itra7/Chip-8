//
// Created by filip on 1/13/25.
//

#pragma once

#include <cstdint>
#include <string>

#define FONT_START_ADDRESS 0
#define FONT_SIZE 80


// 0x200-0xFFF reserved memory for programs, 0x000-0x1FF reserved for interpreter
const unsigned int memory_size = 4096;
const unsigned int registers_size = 16;
const unsigned int SCREEN_WIDTH = 64;
const unsigned int SCREEN_HEIGHT = 32;


class Chip8 {
  public:
    Chip8();
    void LoadRom(const std::string pathtofile);
    void Cycle();
    void getMemory(int index);
	 
    uint8_t memory[memory_size];
    uint16_t opcode;
    uint8_t video[64 * 32]{0};
    
  private:
    void print();
    void executeInstruction();
    // 16 8-bit data registers V0-VF
    uint8_t V[registers_size]{0};
    // we will use mask 0x0FFF to ensure 12bits
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16]{0};
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keys[16]{0};
};
