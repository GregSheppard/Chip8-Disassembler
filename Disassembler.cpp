#include "Disassembler.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

void Disassembler::loadROM(char const* filename) {
	std::streampos size;
	char* buffer;
	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate); //open at end of file to get file size

	if (file.is_open()) {
		std::cout << "opened ROM" << std::endl;
		size = file.tellg(); //get filesize
		buffer = new char[size]; //create buffer of size filesize
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; ++i) {
			ROM.push_back(buffer[i]);
		}
		delete[] buffer;
	}
	else { std::cout << "Unable to open " << filename << std::endl; }
}

void Disassembler::disassembleROM(char const* filename) {
	loadROM(filename);
	for (unsigned int op = 0; op < ROM.size(); op+=2) {
		disassembleOp((ROM.at(op) << 0x8u) + ROM.at(op+1));
	}
}

void Disassembler::toFile(char const* filename) {
	std::ofstream f(filename);
	std::ostream_iterator<std::string> oIterator(f, "\n");
	std::copy(dasm.begin(), dasm.end(), oIterator);
}

void Disassembler::disassembleOp(uint16_t opcode) {
	switch ((opcode & 0xF000u)) {
	case 0x0000u:
		switch ((opcode & 0x00FFu)) {
		case 0x00E0: op00E0(opcode); break;
		case 0x00EE: op00EE(opcode); break;
		}
		break;

	case 0x1000u: op1nnn(opcode); break;
	case 0x2000u: op2nnn(opcode); break;
	case 0x3000u: op3xkk(opcode); break;
	case 0x4000u: op4xkk(opcode); break;
	case 0x5000u: op5xy0(opcode); break;
	case 0x6000u: op6xkk(opcode); break;
	case 0x7000u: op7xkk(opcode); break;
	case 0x8000u:
		switch ((opcode & 0x000Fu)) {
		case 0x0000: op8xy0(opcode); break;
		case 0x0001: op8xy1(opcode); break;
		case 0x0002: op8xy2(opcode); break;
		case 0x0003: op8xy3(opcode); break;
		case 0x0004: op8xy4(opcode); break;
		case 0x0005: op8xy5(opcode); break;
		case 0x0006: op8xy6(opcode); break;
		case 0x0007: op8xy7(opcode); break;
		case 0x000E: op8xyE(opcode); break;
		}
		break;

	case 0x9000u: op9xy0(opcode); break;
	case 0xA000u: opAnnn(opcode); break;
	case 0xB000u: opBnnn(opcode); break;
	case 0xC000u: opCxkk(opcode); break;
	case 0xD000u: opDxyn(opcode); break;
	case 0xE000u:
		switch ((opcode & 0x00FFu)) {
		case 0x009E: opEx9E(opcode); break;
		case 0x00A1: opExA1(opcode); break;
		}
		break;

	case 0xF000u:
		switch ((opcode & 0x00FFu)) {
		case 0x0007: opFx07(opcode); break;
		case 0x000A: opFx0A(opcode); break;
		case 0x0015: opFx15(opcode); break;
		case 0x0018: opFx18(opcode); break;
		case 0x001E: opFx1E(opcode); break;
		case 0x0029: opFx29(opcode); break;
		case 0x0033: opFx33(opcode); break;
		case 0x0055: opFx55(opcode); break;
		case 0x0065: opFx65(opcode); break;
		}

		break;
	}
}

void Disassembler::op00E0(uint16_t opcode) { //CLS
	dasm.push_back("CLS");
}

void Disassembler::op00EE(uint16_t opcode) { //RET
	dasm.push_back("RET");
}

void Disassembler::op1nnn(uint16_t opcode) { //JP addr
	std::stringstream ss;
	ss << "JP 0x" << std::hex << (opcode & 0x0FFFu);
	dasm.push_back(ss.str());
}

void Disassembler::op2nnn(uint16_t opcode) { //CALL addr
	std::stringstream ss;
	ss << "CALL 0x" << std::hex << (opcode & 0x0FFFu);
	dasm.push_back(ss.str());
}

void Disassembler::op3xkk(uint16_t opcode) { //SE Vx, byte
	std::stringstream ss;
	ss << "SE V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", 0x" << (opcode & 0x00FFu);
	dasm.push_back(ss.str());
}

void Disassembler::op4xkk(uint16_t opcode) { //SNE Vx, byte
	std::stringstream ss;
	ss << "SNE V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", 0x" << (opcode & 0x00FFu);
	dasm.push_back(ss.str());
}

void Disassembler::op5xy0(uint16_t opcode) { //SE Vx, Vy
	std::stringstream ss;
	ss << "SE V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V"<< ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op6xkk(uint16_t opcode) { //LD Vx, byte
	std::stringstream ss;
	ss << "LD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", 0x" << (opcode & 0x00FFu);
	dasm.push_back(ss.str());
}

void Disassembler::op7xkk(uint16_t opcode) { //ADD Vx, byte
	std::stringstream ss;
	ss << "ADD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", 0x" << (opcode & 0x00FFu);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy0(uint16_t opcode) { //LD Vx, Vy
	std::stringstream ss;
	ss << "LD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy1(uint16_t opcode) { //OR Vx, Vy
	std::stringstream ss;
	ss << "OR V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy2(uint16_t opcode) { //AND Vx, Vy
	std::stringstream ss;
	ss << "AND V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy3(uint16_t opcode) { //XOR Vx, Vy
	std::stringstream ss;
	ss << "XOR V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy4(uint16_t opcode) { //ADD Vx, Vy
	std::stringstream ss;
	ss << "ADD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy5(uint16_t opcode) { //SUB Vx, Vy
	std::stringstream ss;
	ss << "SUB V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy6(uint16_t opcode) { //SHR Vx, Vy
	std::stringstream ss;
	ss << "SHR V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xy7(uint16_t opcode) { //SUBN Vx, Vy
	std::stringstream ss;
	ss << "SUBN V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op8xyE(uint16_t opcode) { //SHL Vx, Vy
	std::stringstream ss;
	ss << "SHL V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::op9xy0(uint16_t opcode) { //SNE Vx, Vy
	std::stringstream ss;
	ss << "SNE V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", " << "V" << ((opcode & 0x00F0u) >> 0x4u);
	dasm.push_back(ss.str());
}

void Disassembler::opAnnn(uint16_t opcode) { //LD I, addr
	std::stringstream ss;
	ss << "LD I, 0x" << std::hex << (opcode & 0x0FFFu);
	dasm.push_back(ss.str());
}

void Disassembler::opBnnn(uint16_t opcode) { //JP V0, addr
	std::stringstream ss;
	ss << "JP V0, 0x" << std::hex << (opcode & 0x0FFFu);
	dasm.push_back(ss.str());
}

void Disassembler::opCxkk(uint16_t opcode) { //RND Vx, byte
	std::stringstream ss;
	ss << "RND V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", 0x" << (opcode & 0x00FFu);
	dasm.push_back(ss.str());
}

void Disassembler::opDxyn(uint16_t opcode) { //DRW, Vx, Vy, nibble
	std::stringstream ss;
	ss << "DRW V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", V" << ((opcode & 0x00F0u) >> 0x4u) << ", 0x" << (opcode & 0x000Fu);
	dasm.push_back(ss.str());
}

void Disassembler::opEx9E(uint16_t opcode) { //SKP Vx
	std::stringstream ss;
	ss << "SKP V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opExA1(uint16_t opcode) { //SKP Vx
	std::stringstream ss;
	ss << "SKNP V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx07(uint16_t opcode) { //LD Vx, DT
	std::stringstream ss;
	ss << "LD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", DT";
	dasm.push_back(ss.str());
}

void Disassembler::opFx0A(uint16_t opcode) { //LD Vx, K
	std::stringstream ss;
	ss << "LD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", K";
	dasm.push_back(ss.str());
}

void Disassembler::opFx15(uint16_t opcode) { //LD Vx, DT
	std::stringstream ss;
	ss << "LD DT, V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx18(uint16_t opcode) { //LD Vx, DT
	std::stringstream ss;
	ss << "LD ST, V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx1E(uint16_t opcode) { //ADD I, Vx
	std::stringstream ss;
	ss << "ADD I, V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx29(uint16_t opcode) { //LD F, Vx
	std::stringstream ss;
	ss << "LD F, V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx33(uint16_t opcode) { //LD B, Vx
	std::stringstream ss;
	ss << "LD B, V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx55(uint16_t opcode) { //LD [I], Vx
	std::stringstream ss;
	ss << "LD [I], V" << std::hex << ((opcode & 0x0F00u) >> 0x8u);
	dasm.push_back(ss.str());
}

void Disassembler::opFx65(uint16_t opcode) { //LD [I], Vx
	std::stringstream ss;
	ss << "LD V" << std::hex << ((opcode & 0x0F00u) >> 0x8u) << ", [I]";
	dasm.push_back(ss.str());
}



