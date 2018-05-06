#include <stdio.h>
#include <assert.h>

#include "rv32.h"
#include "main.h"

const insn rv32::insns[] =
{
    {0b0110111, insntype::U, 0, 0b000, 0, 0b0000000, "lui", rv32::exec_LUI },
    {0b0010111, insntype::U, 0, 0b000, 0, 0b0000000, "auipc", rv32::exec_AUIPC },
    {0b1101111, insntype::J, 0, 0b000, 0, 0b0000000, "jal", rv32::exec_JAL },
    {0b1100111, insntype::I, 0, 0b000, 0, 0b0000000, "jalr", rv32::exec_JALR },
    {0b1100011, insntype::B, 1, 0b000, 0, 0b0000000, "beq", rv32::exec_BEQ },
    {0b1100011, insntype::B, 1, 0b001, 0, 0b0000000, "bne", rv32::exec_BNE },
    {0b1100011, insntype::B, 1, 0b100, 0, 0b0000000, "blt", rv32::exec_BLT },
    {0b1100011, insntype::B, 1, 0b101, 0, 0b0000000, "bge", rv32::exec_BGE },
    {0b1100011, insntype::B, 1, 0b110, 0, 0b0000000, "bltu", rv32::exec_BLTU },
    {0b1100011, insntype::B, 1, 0b111, 0, 0b0000000, "bgeu", rv32::exec_BGEU },
    {0b0000011, insntype::I, 1, 0b000, 0, 0b0000000, "lb", rv32::exec_LB },
    {0b0000011, insntype::I, 1, 0b001, 0, 0b0000000, "lh", rv32::exec_LH },
    {0b0000011, insntype::I, 1, 0b010, 0, 0b0000000, "lw", rv32::exec_LW },
    {0b0000011, insntype::I, 1, 0b100, 0, 0b0000000, "lbu", rv32::exec_LBU },
    {0b0000011, insntype::I, 1, 0b101, 0, 0b0000000, "lhu", rv32::exec_LHU },
    {0b0100011, insntype::S, 1, 0b000, 0, 0b0000000, "sb", rv32::exec_SB },
    {0b0100011, insntype::S, 1, 0b001, 0, 0b0000000, "sh", rv32::exec_SH },
    {0b0100011, insntype::S, 1, 0b010, 0, 0b0000000, "sw", rv32::exec_SW },
    {0b0010011, insntype::I, 1, 0b000, 0, 0b0000000, "addi", rv32::exec_ADDI },
    {0b0010011, insntype::I, 1, 0b010, 0, 0b0000000, "slti", rv32::exec_SLTI },
    {0b0010011, insntype::I, 1, 0b011, 0, 0b0000000, "sltiu", rv32::exec_SLTIU },
    {0b0010011, insntype::I, 1, 0b100, 0, 0b0000000, "xori", rv32::exec_XORI },
    {0b0010011, insntype::I, 1, 0b110, 0, 0b0000000, "ori", rv32::exec_ORI },
    {0b0010011, insntype::I, 1, 0b111, 0, 0b0000000, "andi", rv32::exec_ANDI },
    {0b0010011, insntype::R, 1, 0b001, 1, 0b0000000, "slli", rv32::exec_SLLI },
    {0b0010011, insntype::R, 1, 0b101, 1, 0b0000000, "srli", rv32::exec_SRLI },
    {0b0010011, insntype::R, 1, 0b101, 1, 0b0100000, "srai", rv32::exec_SRAI },
    {0b0110011, insntype::R, 1, 0b000, 1, 0b0000000, "add", rv32::exec_ADD },
    {0b0110011, insntype::R, 1, 0b000, 1, 0b0100000, "sub", rv32::exec_SUB },
    {0b0110011, insntype::R, 1, 0b001, 1, 0b0000000, "sll", rv32::exec_SLL },
    {0b0110011, insntype::R, 1, 0b010, 1, 0b0000000, "slt", rv32::exec_SLT },
    {0b0110011, insntype::R, 1, 0b011, 1, 0b0000000, "sltu", rv32::exec_SLTU },
    {0b0110011, insntype::R, 1, 0b100, 1, 0b0000000, "xor", rv32::exec_XOR },
    {0b0110011, insntype::R, 1, 0b101, 1, 0b0000000, "srl", rv32::exec_SRL },
    {0b0110011, insntype::R, 1, 0b101, 1, 0b0100000, "sra", rv32::exec_SRA },
    {0b0110011, insntype::R, 1, 0b110, 1, 0b0000000, "or", rv32::exec_OR },
    {0b0110011, insntype::R, 1, 0b111, 1, 0b0000000, "and", rv32::exec_AND },
};

const char * rv32::regNames[2][32] = 
{
    { "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  
      "x8",  "x9", "x10", "x11", "x12", "x13", "x14", "x15",
     "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", 
     "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31" },

    { "zero", "ra",  "sp",  "gp", "tp", "t0", "t1", "t2",
      "s0",   "s1",  "a0",  "a1", "a2", "a3", "a4", "a5", 
      "a6",   "a7",  "s2",  "s3", "s4", "s5", "s6", "s7",
      "s8",   "s9", "s10", "S11", "t3", "t4", "t5", "t6" }
};


rv32::rv32()
{
	trace = 1;
	reset();
	setRegNamesABI(0);
}


rv32::~rv32()
{
}

void rv32::reset()
{
	pc = 0;

	for (uint8_t i=1; i<sizeof(reg)/sizeof(*reg); ++i)
		reg[i] = 0xf0f0f0f0;		// a recognizable pattern
	reg[0] = 0;
}

void rv32::dump()
{
	if(getRegNamesABI())
	{
		for (uint8_t i=0; i<sizeof(reg)/sizeof(*reg); ++i)
		{
			if (i%4 == 0)
				fprintf(ddtout, "%s", i==0?"":"\n");
			
			fprintf(ddtout, "%*s %8.8x", i%4==0?6:4, getRegName(i), getReg(i));
		}
	}
	else
	{
		for (uint8_t i=0; i<sizeof(reg)/sizeof(*reg); ++i)
		{
			if (i%8 == 0)
				fprintf(ddtout, "%s  %sx%d", i==0?"":"\n", i<10?" ":"", i);
			
			fprintf(ddtout, "%c%8.8x", (i%8!=0 && i%4==0)?'-':' ', getReg(i));
		}
	}

	fprintf(ddtout, "\n   pc %8.8x\n", getPc());
}

int32_t rv32::getReg(uint8_t r)
{
	assert(r<32 && "invalid register number");
	return reg[r];
}

int32_t rv32::getPc()
{
	return pc;
}

void rv32::setReg(uint8_t r, int32_t val)
{
	assert(r<32 && "invalid register number");
	if (r > 0)
		reg[r] = val;
}

void rv32::setPc(int32_t val)
{
	pc = val;
}

int rv32::exec()
{
	// make sure the pc is legal
	if (pc%4)
	{
		fprintf(ddtout, "ERROR: The program counter (0x%8.8x) is not a multiple of 4\n", pc);
		return 1;
	}

	// Get the insn at the address in the pc
	int32_t insn = mem->get32(pc);

	if (insn == 0x00100073) 
	{
		fprintf(ddtout, "%8.8x: ebreak\n", getPc());
		return 1;
	}
	if (insn == 0x00000073) 
	{
		fprintf(ddtout, "%8.8x: ecall (unimplemented)\n", getPc());
		return 1;
	}

	return execInsn(insn);
}

/**
* Exec the given instruction & post any side-effects.
***********************************************************/
int rv32::execInsn(int32_t insn)
{
	uint8_t opcode = getInsnOpcode(insn);
	uint8_t funct3 = getInsnFunct3(insn);
	uint8_t funct7 = getInsnFunct7(insn);

    uint16_t i;
    for (i=0; i<sizeof(insns)/sizeof(*insns); ++i)
    {
		if (opcode==insns[i].opcode
			&& (!insns[i].funct3_flag || (insns[i].funct3_flag && funct3==insns[i].funct3))
			&& (!insns[i].funct7_flag || (insns[i].funct7_flag && funct7==insns[i].funct7)) )
        {
			if (trace)
				fprintf(ddtout, "%8.8x: ", getPc());
			(*insns[i].func)(this, insn);
			if (trace)
				fprintf(ddtout, "\n");
            break;
        }
    }

	if (i==sizeof(insns)/sizeof(*insns))
	{
       	fprintf(ddtout, "(illegal)\n");
		return 1;	// tell the CLI that there was a problem
	}

	return 0;
}

int rv32::setTrace(int i)
{
	int r = trace;
	trace = i;
	return r;
}


const char *rv32::getRegName(uint8_t r)
{
	assert(r <= 31 && "illegal reg number");

	return regNames[regNamesABI][r];
}


//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************


#define rn(r) theCpu->getRegName(r)
#define COMMENT_OFFSET	(32)

void rv32::exec_LUI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmU(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lui     %s, 0x%x", insn, rn(rd), (imm>>12)&0x0fffff);
		fprintf(ddtout, "%*s# %s = 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), imm);
	}

	theCpu->setReg(rd, imm);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_AUIPC(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmU(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t target = theCpu->getPc()+imm;
	
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  auipc   %s, 0x%x", insn, rn(rd), (imm>>12)&0x0fffff);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x + 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), target, theCpu->getPc(), imm);
	}

	theCpu->setReg(rd, target);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_JAL(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmJ(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t target = theCpu->getPc()+imm;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  jal     %s, .%+d", insn, rn(rd), imm);
		fprintf(ddtout, "%*s# pc = 0x%8.8x = 0x%8.8x + 0x%8.8x", COMMENT_OFFSET-len, "", target, theCpu->getPc(), imm);
	}

	theCpu->setReg(rd, theCpu->getPc()+4);
	theCpu->setPc(target);
}

void rv32::exec_JALR(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rd = theCpu->getInsnRd(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);

	int32_t target = (theCpu->getReg(rs1)+imm) & ~1;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  jalr    %s, %+d(x%d)", insn, rn(rd), imm, rs1);
		fprintf(ddtout, "%*s# %s=%8.8x, pc = 0x%8.8x = %d(0x%8.8x)&~1", COMMENT_OFFSET-len, "", rn(rd), theCpu->getPc()+4, target, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, theCpu->getPc()+4);
	theCpu->setPc(target);
}

void rv32::exec_BEQ(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = (theCpu->getReg(rs1) == theCpu->getReg(rs2));
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  beq     %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_BNE(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = (theCpu->getReg(rs1) != theCpu->getReg(rs2));
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  bne     %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_BLT(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = (theCpu->getReg(rs1) < theCpu->getReg(rs2));		// signed compare
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  blt     %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_BGE(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = (theCpu->getReg(rs1) >= theCpu->getReg(rs2));		// signed compare
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  bge     %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_BLTU(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = ((uint32_t)theCpu->getReg(rs1) < (uint32_t)theCpu->getReg(rs2));		// UN-signed compare
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  bltu    %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_BGEU(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmB(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	int cond = ((uint32_t)theCpu->getReg(rs1) >= (uint32_t)theCpu->getReg(rs2));		// UN-signed compare
	int32_t ttarget = theCpu->getPc()+imm;
	int32_t ftarget = theCpu->getPc()+4;

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  bgeu    %s, %s, %d", insn, rn(rs1), rn(rs2), imm);
		fprintf(ddtout, "%*s# pc = (0x%x == 0x%x) ? 0x%x : 0x%x", COMMENT_OFFSET-len, "", theCpu->getReg(rs1), theCpu->getReg(rs2), ttarget, ftarget);
	}

	theCpu->setPc(cond ? ttarget : ftarget);
}

void rv32::exec_LB(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t m8 = mem->get8(addr);
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lb      %s, %d(%s)", insn, rn(rd), imm, rn(rs1));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = %d(0x%8.8x)", COMMENT_OFFSET-len, "", rn(rd), m8, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, m8);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_LH(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t m16 = mem->get16(addr);
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lh      %s, %d(%s)", insn, rn(rd), imm, rn(rs1));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = %d(0x%8.8x)", COMMENT_OFFSET-len, "", rn(rd), m16, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, m16); //mem->get16(theCpu->getReg(rs1)+imm));
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_LW(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t m32 = mem->get32(addr);
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lw      %s, %d(%s)", insn, rn(rd), imm, rn(rs1));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = %d(0x%8.8x)", COMMENT_OFFSET-len, "", rn(rd), m32, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, m32);	// mem->get32(theCpu->getReg(rs1)+imm));
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_LBU(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t m = mem->get8(addr)&0x000000ff;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lbu     %s, %d(%s)", insn, rn(rd), imm, rn(rs1));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = %d(0x%8.8x)", COMMENT_OFFSET-len, "", rn(rd), m, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, m); 
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_LHU(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t m = mem->get16(addr)&0x0000ffff;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  lhu     %s, %d(%s)", insn, rn(rd), imm, rn(rs1));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = %d(0x%8.8x)", COMMENT_OFFSET-len, "", rn(rd), m, imm, theCpu->getReg(rs1));
	}

	theCpu->setReg(rd, m);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SB(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmS(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t src = theCpu->getReg(rs2) & 0x000000ff;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sb      %s, %d(%s)", insn, rn(rs2), imm, rn(rs1));
		fprintf(ddtout, "%*s# %d(0x%8.8x) = 0x%8.8x", COMMENT_OFFSET-len, "", imm, theCpu->getReg(rs1), src);
	}

	mem->set8(addr, src);	//theCpu->getReg(rs2));
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SH(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmS(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t src = theCpu->getReg(rs2) & 0x0000ffff;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sh      %s, %d(%s)", insn, rn(rs2), imm, rn(rs1));
		fprintf(ddtout, "%*s# %d(0x%8.8x) = 0x%8.8x", COMMENT_OFFSET-len, "", imm, theCpu->getReg(rs1), src);
	}

	//mem->set16(theCpu->getReg(rs1)+imm, theCpu->getReg(rs2));
	mem->set16(addr, src);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SW(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmS(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);

	uint32_t addr = theCpu->getReg(rs1)+imm;
	uint32_t src = theCpu->getReg(rs2);
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sw      %s, %d(%s)", insn, rn(rs2), imm, rn(rs1));
		fprintf(ddtout, "%*s# %d(0x%8.8x) = 0x%8.8x", COMMENT_OFFSET-len, "", imm, theCpu->getReg(rs1), src);
	}

	mem->set32(addr, src);	//theCpu->getReg(rs2));
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_ADDI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint32_t sum = theCpu->getReg(rs1) + imm;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  addi    %s, %s, %d", insn, rn(rd), rn(rs1), imm);
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x + 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), sum, theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, sum);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLTI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int cond = (theCpu->getReg(rs1) < imm);		// signed compare

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  slti    %s, %s, %d", insn, rn(rd), rn(rs1), imm);
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = (0x%8.8x < 0x%8.8x) ? 1 : 0", COMMENT_OFFSET-len, "", rn(rd), cond, theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, cond);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLTIU(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);		// yes, we want to sign-extend the imm
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int cond = ((uint32_t)theCpu->getReg(rs1) < (uint32_t)imm);		// UN-signed compare

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sltiu   %s, %s, %d", insn, rn(rd), rn(rs1), imm);
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = (0x%8.8x < 0x%8.8x) ? 1 : 0", COMMENT_OFFSET-len, "", rn(rd), cond, theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, cond);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_XORI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) ^ imm);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  xori    %s, %s, %d", insn, rn(rd), rn(rs1), imm);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x ^ 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_ORI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) | imm);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  ori     %s, %s, %d", insn, rn(rd), rn(rs1), imm); 
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x | 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_ANDI(rv32 *theCpu, uint32_t insn)
{
	int32_t imm = theCpu->getInsnImmI(insn);
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) & imm);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  andi    %s, %s, %d", insn, rn(rd), rn(rs1), imm);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x & 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), imm);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLLI(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t shamt = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) << shamt);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  slli    %s, %s, %d", insn, rn(rd), rn(rs1), shamt);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x << %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SRLI(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t shamt = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = ((uint32_t)theCpu->getReg(rs1) >> shamt);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  srli    %s, %s, %d", insn, rn(rd), rn(rs1), shamt);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x >> %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SRAI(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t shamt = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) >> shamt);	

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  srai    %s, %s, %d", insn, rn(rd), rn(rs1), shamt);
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x >> %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_ADD(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = theCpu->getReg(rs1) + theCpu->getReg(rs2);

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  add     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x + 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SUB(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = theCpu->getReg(rs1) - theCpu->getReg(rs2);

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sub     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x - 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLL(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	uint8_t shamt = theCpu->getReg(rs2)&0x01f;
	int result = theCpu->getReg(rs1) << shamt;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sll     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x << %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLT(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int result = (theCpu->getReg(rs1) < theCpu->getReg(rs2));

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  slt     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = (0x%8.8x < 0x%8.8x) ? 1 : 0", COMMENT_OFFSET-len, "", rn(rd), result, theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SLTU(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int result = ((uint32_t)theCpu->getReg(rs1) < (uint32_t)theCpu->getReg(rs2));
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sltu    %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = (0x%8.8x < 0x%8.8x) ? 1 : 0", COMMENT_OFFSET-len, "", rn(rd), result, theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_XOR(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) ^ theCpu->getReg(rs2));

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  xor     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x ^ 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result, theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SRL(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int shamt = theCpu->getReg(rs2)&0x01f;
	int result = ((uint32_t)theCpu->getReg(rs1)) >> shamt;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  srl     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x >> %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_SRA(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int shamt = theCpu->getReg(rs2)&0x01f;
	int result = theCpu->getReg(rs1) >> shamt;
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  sra     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
		fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x >> %d", COMMENT_OFFSET-len, "", rn(rd), result,  theCpu->getReg(rs1), shamt);
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_OR(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int32_t result = (theCpu->getReg(rs1) | theCpu->getReg(rs2));

	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  or      %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x | 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result, theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}

void rv32::exec_AND(rv32 *theCpu, uint32_t insn)
{
	int8_t rs1 = theCpu->getInsnRs1(insn);
	int8_t rs2 = theCpu->getInsnRs2(insn);
	int8_t rd = theCpu->getInsnRd(insn);

	int result = (theCpu->getReg(rs1) & theCpu->getReg(rs2));
	if (theCpu->trace)
	{
		int len = fprintf(ddtout, "%8.8x  and     %s, %s, %s", insn, rn(rd), rn(rs1), rn(rs2));
    	fprintf(ddtout, "%*s# %s = 0x%8.8x = 0x%8.8x & 0x%8.8x", COMMENT_OFFSET-len, "", rn(rd), result, theCpu->getReg(rs1), theCpu->getReg(rs2));
	}

	theCpu->setReg(rd, result);
	theCpu->setPc(theCpu->getPc()+4);
}







int32_t rv32::getInsnImmI(uint32_t insn)
{
	return ((int32_t)insn)>>20;
}

int32_t rv32::getInsnImmS(uint32_t insn)
{
	return ((((int32_t)insn)>>20)&0xffffffe0) 
		| ((insn>>7)&0x0000001f)
		;
}

int32_t rv32::getInsnImmB(uint32_t insn)
{
	return ((insn&0x00000f00)>>7) 
		| ((insn&0x00000080)<<4) 
		| ((insn&0x7e000000)>>20) 
		| ((insn&0x80000000)>>19)
		| ((insn&0x80000000)?0xfffff000:0)
		;
}

int32_t rv32::getInsnImmU(uint32_t insn)
{
	return insn&0xfffff000;
}

int32_t rv32::getInsnImmJ(uint32_t insn)
{
	return (insn&0x7fe00000)>>20			// 10:1
		| (insn&0x00100000)>>9				// 11
		| (insn&0x000ff000)					// 19:12
		| (insn&0x80000000)>>11				// 20
		| ((insn&0x80000000)?0xfff00000:0)
		;
}

//*****************************************************************
//*****************************************************************
//*****************************************************************
//*****************************************************************
#if 0
void rv32::printInsnArgs(const insn *pi, uint32_t insn)
{
	uint8_t rd = getInsnRd(insn);
	uint8_t rs1 = getInsnRs1(insn);
	uint8_t rs2 = getInsnRs2(insn);
	//int32_t imm;

	//printf("%d %d %d  ", rd,rs1,rs2);

	switch(pi->itype)
	{
	case insntype::R:
		fprintf(ddtout, "x%d, x%d, x%d", rd, rs1, rs2);
		break;

	case insntype::I:
		//imm = ((int32_t)insn)>>20;
		fprintf(ddtout, "x%d, x%d, %d", rd, rs1, getInsnImmI(insn));
		break;

	case insntype::S:
#if 0
		imm = (((int32_t)insn)>>20)&0xffffffe0;;
		imm |= (insn>>7)&0x0000001f;
#endif
		fprintf(ddtout, "x%d, %d(x%d)", rs2, getInsnImmS(insn), rs1);
		break;

	case insntype::B:
#if 0
		imm = (insn&0x00000f00)>>7;
		imm |= (insn&0x00000080)<<4;
		imm |= (insn&0x7c000000)>>20;
		imm |= (((int32_t)insn)>>19)&0xfffff000;
#endif
		fprintf(ddtout, "x%d, x%d, %d", rs1, rs2, getInsnImmB(insn));
		break;

	case insntype::U:
		//imm = insn&0xfffff000;
		fprintf(ddtout, "x%d, 0x%x", rd, getInsnImmU(insn));
		break;

	case insntype::J:
		fprintf(ddtout, "x%d, %d", rd, getInsnImmJ(insn));
		break;
	}
}


void rv32::printInsn(uint32_t insn)
{
	uint8_t opcode = getInsnOpcode(insn);
	uint8_t funct3 = getInsnFunct3(insn);
	uint8_t funct7 = getInsnFunct7(insn);

	//printf("%8.8x  %2x %1x  ", insn, opcode, funct3);

	fprintf(ddtout, "%8.8x ", insn);

	uint16_t i;
	for (i=0; i<sizeof(insns)/sizeof(*insns); ++i)
	{
		if (opcode==insns[i].opcode
			&& (!insns[i].funct3_flag || (insns[i].funct3_flag && funct3==insns[i].funct3))
			&& (!insns[i].funct7_flag || (insns[i].funct7_flag && funct7==insns[i].funct7)) )
		{
			fprintf(ddtout, "%-8.8s", insns[i].mneumonic);
			printInsnArgs(&insns[i], insn);

			// XXX dump resolved imm and reg values as comments

			fprintf(ddtout, "\n");
			break;
		}
	}

	if (i==sizeof(insns)/sizeof(*insns))
		fprintf(ddtout, "(illegal)\n");
}
#endif
