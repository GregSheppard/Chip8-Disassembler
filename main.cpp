#include <cstdint>
#include <iostream>
#include "Disassembler.h"

int main() {
	Disassembler dasm;
	dasm.disassembleROM("ROMs/pong.ch8");
	dasm.toFile("asm/pong.asm");
}