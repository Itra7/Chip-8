//
// Created by filip on 1/13/25.
//

#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <string>

// 0x200-0xFFF reserved memory for programs, 0x000-0x1FF reserved for interpreter
const int memory_size = 4096;
const int registers_size = 16;


class Chip8 {
  public:
    Chip8();
    void LoadRom(const std::string pathtofile);
    void Cycle();
    void getMemory(int index);
	uint8_t memory[memory_size];



  private:
//
	void opcode_0nnn();
//
//	// clear screen
//	void 00E0();
//
//	// RET
//	void 00EE();
//
//	// JP address
//	void 1nnn();
//
//	// CALL address
//	void 2nnn();
//
//	// SE Vx, byte
//	void 3xkk();
//
//	// SNE Vx, byte
//	void 4xkk();
//
//	// SE Vx, Vy
//	void 5xy0();
//
//	// LD Vx, byte
//	void 6xkk();
//
//	// ADD Vx, byte
//	void 7xkk();
//
//	// LD Vx, Vy
//	void 8xy0();
//
//	// OR Vx, Vy
//	void 8xy1();
//
//	// AND Vx, Vy
//	void 8xy2();
//
//	// XOR Vx, Vy
//	void 8xy3();
//
//	// ADD Vx, Vy
//	void 8xy4();
//
//	// SUB Vx, Vy
//	void 8xy5();
//
//	// SHR Vx
//	void 8xy6();
//
//	// SUBN Vx, Vy
//	void 8xy7();
//
//	// SHL Vx
//	void 8xyE();
//
//	// SNE Vx, Vy
//	void 9xy0();
//
//	// LD I, address
//	void Annn();
//
//	// JP V0, address
//	void Bnnn();
//
//	// RND Vx, byte
//	void Cxkk();
//
//	// DRW Vx, Vy, height
//	void Dxyn();
//
//	// SKP Vx
//	void Ex9E();
//
//	// SKNP Vx
//	void ExA1();
//
//	// LD Vx, DT
//	void Fx07();
//
//	// LD Vx, K
//	void Fx0A();
//
//	// LD DT, Vx
//	void Fx15();
//
//	// LD ST, Vx
//	void Fx18();
//
//	// ADD I, Vx
//	void Fx1E();
//
//	// LD F, Vx
//	void Fx29();
//
//	// LD B, Vx
//	void Fx33();
//
//	// LD [I], Vx
//	void Fx55();
//
//	// LD Vx, [I]
//	void Fx65();

    // 16 8-bit data registers V0-VF
    uint8_t V[registers_size];
    // we will use mask 0x0FFF to ensure 12bits
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t key[16];
};



#endif //CHIP8_HPP
