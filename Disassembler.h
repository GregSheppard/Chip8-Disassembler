#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Disassembler {
public:
	std::vector<uint8_t> ROM;
	void disassembleROM(char const* filename);
	void toFile(char const* filename);

private:
	std::vector<std::string> dasm;

	void disassembleOp(uint16_t opcode);
	void loadROM(char const* filename);

	void op00E0(uint16_t opcode); //CLS
	void op00EE(uint16_t opcode); //RET
	void op1nnn(uint16_t opcode); //JP addr
	void op2nnn(uint16_t opcode); //CALL addr
	void op3xkk(uint16_t opcode); //SE Vx, byte
	void op4xkk(uint16_t opcode); //SNE Vx, byte
	void op5xy0(uint16_t opcode); //SE Vx, Vy
	void op6xkk(uint16_t opcode); //LD Vx, byte
	void op7xkk(uint16_t opcode); //ADD Vx, byte
	void op8xy0(uint16_t opcode); //LD Vx, Vy
	void op8xy1(uint16_t opcode); //OR Vx, Vy
	void op8xy2(uint16_t opcode); //AND Vx, Vy
	void op8xy3(uint16_t opcode); ///XOR Vx, Vy
	void op8xy4(uint16_t opcode); //ADD Vx, Vy
	void op8xy5(uint16_t opcode); //SUB Vx, Vy
	void op8xy6(uint16_t opcode); //SHR Vx, {, Vy}
	void op8xy7(uint16_t opcode); //SUBN Vx, Vy
	void op8xyE(uint16_t opcode); //SHL Vx {, Vy}
	void op9xy0(uint16_t opcode); //SNE Vx, Vy
	void opAnnn(uint16_t opcode); //LD I, addr
	void opBnnn(uint16_t opcode); //JP V0, addr
	void opCxkk(uint16_t opcode); //RND Vx, byte
	void opDxyn(uint16_t opcode); //DRW Vx, Vy, nibble
	void opEx9E(uint16_t opcode); //SKP Vx
	void opExA1(uint16_t opcode); //SKNP Vx
	void opFx07(uint16_t opcode); //LD Vx, DT
	void opFx0A(uint16_t opcode); //LD Vx, K
	void opFx15(uint16_t opcode); //LD DT, Vx
	void opFx18(uint16_t opcode); //LD ST, Vx
	void opFx1E(uint16_t opcode); //ADD I, Vx
	void opFx29(uint16_t opcode); //LD F, Vx
	void opFx33(uint16_t opcode); //LD B, Vx
	void opFx55(uint16_t opcode); //LD [I], Vx
	void opFx65(uint16_t opcode); //LD Vx, [I]
};