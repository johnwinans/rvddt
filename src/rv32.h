//****************************************************************************
//
//    RVDDT (RISC-V Dynamic Debugging Tool)
//
//    Copyright (C) 2020  John Winans
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//    USA
//
//****************************************************************************

#ifndef rv32_H
#define rv32_H

#include <stdint.h>
#include <stdint.h>
#include "memory.h"

enum class insntype { R, I, S, B, U, J };

class rv32;
typedef void (*execInsnFunc)(rv32 *, uint32_t);

struct insn
{
	uint8_t		opcode;
	insntype	itype;
	uint8_t		funct3_flag;
	uint8_t		funct3;
	uint8_t		funct7_flag;
	uint8_t		funct7;
	const char	*mneumonic;
	execInsnFunc	func;
};

class rv32
{
public:
	rv32();
	~rv32();

	void reset();
	void dump();

	int32_t getReg(uint8_t reg);
	int32_t getPc();

	void setReg(uint8_t reg, int32_t val);
	void setPc(int32_t val);

	int exec();
	int setTrace(int);

	int getRegNamesABI() const { return regNamesABI; }
	void setRegNamesABI(int i) { regNamesABI = i?1:0; }
	const char *getRegName(uint8_t r);

	int32_t getInsnImmI(uint32_t insn);
	int32_t getInsnImmS(uint32_t insn);
	int32_t getInsnImmB(uint32_t insn);
	int32_t getInsnImmU(uint32_t insn);
	int32_t getInsnImmJ(uint32_t insn);

	int8_t getInsnRd(uint32_t insn) { return (insn&0x00000f80)>>7; }
	int8_t getInsnRs1(uint32_t insn) { return (insn&0x000f8000)>>15; }
	int8_t getInsnRs2(uint32_t insn) { return (insn&0x01f00000)>>20; } 
	int8_t getInsnOpcode(uint32_t insn) { return insn&0x000007f; }
	int8_t getInsnFunct3(uint32_t insn) { return (insn&0x0007000)>>12; }
	int8_t getInsnFunct7(uint32_t insn) { return (insn&0xfe000000)>>25; }

	static void exec_LUI(rv32 *, uint32_t insn);
	static void exec_AUIPC(rv32 *, uint32_t insn);
	static void exec_JAL(rv32 *, uint32_t insn);
	static void exec_JALR(rv32 *, uint32_t insn);
	static void exec_BEQ(rv32 *, uint32_t insn);
	static void exec_BNE(rv32 *, uint32_t insn);
	static void exec_BLT(rv32 *, uint32_t insn);
	static void exec_BGE(rv32 *, uint32_t insn);
	static void exec_BLTU(rv32 *, uint32_t insn);
	static void exec_BGEU(rv32 *, uint32_t insn);
	static void exec_LB(rv32 *, uint32_t insn);
	static void exec_LH(rv32 *, uint32_t insn);
	static void exec_LW(rv32 *, uint32_t insn);
	static void exec_LBU(rv32 *, uint32_t insn);
	static void exec_LHU(rv32 *, uint32_t insn);
	static void exec_SB(rv32 *, uint32_t insn);
	static void exec_SH(rv32 *, uint32_t insn);
	static void exec_SW(rv32 *, uint32_t insn);
	static void exec_ADDI(rv32 *, uint32_t insn);
	static void exec_SLTI(rv32 *, uint32_t insn);
	static void exec_SLTIU(rv32 *, uint32_t insn);
	static void exec_XORI(rv32 *, uint32_t insn);
	static void exec_ORI(rv32 *, uint32_t insn);
	static void exec_ANDI(rv32 *, uint32_t insn);
	static void exec_SLLI(rv32 *, uint32_t insn);
	static void exec_SRLI(rv32 *, uint32_t insn);
	static void exec_SRAI(rv32 *, uint32_t insn);
	static void exec_ADD(rv32 *, uint32_t insn);
	static void exec_SUB(rv32 *, uint32_t insn);
	static void exec_SLL(rv32 *, uint32_t insn);
	static void exec_SLT(rv32 *, uint32_t insn);
	static void exec_SLTU(rv32 *, uint32_t insn);
	static void exec_XOR(rv32 *, uint32_t insn);
	static void exec_SRL(rv32 *, uint32_t insn);
	static void exec_SRA(rv32 *, uint32_t insn);
	static void exec_OR(rv32 *, uint32_t insn);
	static void exec_AND(rv32 *, uint32_t insn);

	//void printInsn(uint32_t insn);

private:
	static const char *regNames[2][32];
	static const insn insns[];


	//void printInsnArgs(const insn *pi, uint32_t insn);
	int execInsn(int32_t insn);
	//void execInsn2(const insn *pi, uint32_t insn);

	int32_t		reg[32];
	uint32_t	pc;
	int			trace;
	int			regNamesABI;		// true = print ABI names
};

#endif
