#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "main.h"
#include "memory.h"
#include "rv32.h"

#include "dev/con.h"

FILE	*ddtout;

rv32	*cpu;
devices	*dev;

static void cli();

static void usage()
{
	printf("Usage: rvddt [-s <memstart>] [-l <memlen>] [-f memimage]\n");
}

int main(int argc, char **argv)
{
	uint64_t	memstart = 0;		// default = 0
	uint64_t	memlen = 0x10000;	// default = 64k
	const char *loadfile = 0;

	int opt;
	while ((opt = getopt(argc, argv, "s:l:f:")) != -1) 
	{
		switch (opt)
		{
		case 's':	// memstart
			memstart = strtoul(optarg, 0, 0);
			break;

		case 'l':	// memory segment length
			memlen = strtoul(optarg, 0, 0);
			break;

		case 'f':	// load memory image from file
			loadfile = optarg;
			break;

		default:
			usage();
			exit(1);
		}
	}
 
	ddtout = stdout;

	mem = new memory(memstart, memlen);
	cpu = new rv32();
	dev = new devices();

	dev->addDevice(new con());

	uint32_t start = memstart;

	// Consider providing a method to bootstrap the pc & sp regs akin to an ARM?
	// prime the program counter
	cpu->setPc(start);					

	// Prime the stack pointer 
	uint32_t stackTop = memstart+memlen-16;		// newlib crt0 accesses beyond sp?
	cpu->setReg(2, stackTop);	
	mem->set64(stackTop, 0);					//  XXX zero some space
	mem->set64(stackTop+8, 0);					//  ... I never checked to see how argv & argc work

	printf("sp initialized to top of memory: 0x%8.8x\n", cpu->getReg(2));

	if (loadfile)
	{
		printf("Loading '%s' to 0x%x\n", loadfile, start);
		mem->readRaw(loadfile, start);
	}
	

	cli();

	delete cpu;
	delete mem;

	fflush(ddtout);
	if (ddtout!=stdout)
		fclose(ddtout);

	return 0;
}

/**
* Run until we hit an EBREAK.
*************************************************************************/
static void run()
{
	int save = cpu->setTrace(0);
	while(!cpu->exec())
		;
	cpu->setTrace(save);
}

/**
* Set the pc to the given address and execute silently, continuously.
*************************************************************************/
static void cli_g(uint64_t pc)
{
	cpu->setPc(pc);
	run();
}
static void cli_g()
{
	run();
}

/**
* Dump the CPU status and then execute one instruction.
*************************************************************************/
static int cli_t(int regs)
{
	if (regs)
		cpu->dump();
	return cpu->exec();
}
static void cli_t(uint64_t count, int regs)
{
	int s = 0;
	while(count-- && !s)
		s = cli_t(regs);
}
static void cli_t(uint64_t pc, uint64_t count, int regs)
{
	cpu->setPc(pc);
	cli_t(count, regs);
}


/**
* Examine the CPU regs/status.
*************************************************************************/
static void cli_r()
{
	cpu->dump();
}

/**
* Dump memory contents.
*************************************************************************/
static void cli_d(uint64_t start, uint64_t len)
{
	mem->dump(start, len);
}

static void redirect(const char *fname)
{
	if (ddtout != stdout)
		fclose(ddtout);

    if (fname)
        ddtout = fopen(fname, "a");
    else
        ddtout = stdout;
}

/**
* Command line interpreter.
*************************************************************************/
static void cli()
{
	int running = 1;
	char		buf[2048];
	char		last[2048];

	uint64_t	d_next = 0;
	char		p1[1024];
	char		p2[64];

	int echo = !isatty(fileno(stdin));

	last[0] = '\0';

	printf("This is rvddt.  Enter ? for help.\n");
	while (running)
	{
		// display a prompt
		printf("ddt> ");

		// get command line
		if (fgets(buf, sizeof(buf), stdin) == 0)
		{
			// got EOF, process it like an 'x' command
			buf[0] = 'x';
			buf[1] = '\0';
		}

		// trim any white junk off the end of the line
		int len = strlen(buf);
		while (len>0 && isspace(buf[len-1]))
			buf[--len] = '\0';		// trim junk newline

		// if user hit return, repeat the last repeatable command
		if (buf[0] == '\0')
			strcpy(buf, last);
		last[0] = '\0';

		if (echo)
			puts(buf);

		char cmd[2048];
		char* args;
		int i = 0;
		len = strlen(buf);
		while (i<len && (isalpha(buf[i]) || buf[i]=='?' || buf[i]=='>'))
		{
			cmd[i] = buf[i];
			++i;
		}
		cmd[i] = '\0';
		args = &buf[i];

		// exec command
		if (cmd[0] == '\0')				// empty line (with no repeatable command in the cache)
			;
		else if (!strcmp(cmd, "x"))		// exit the simulator
			running = 0;
		else if (!strcmp(cmd, "r"))		// dump the regs
			cli_r();
		else if (!strcmp(cmd, "t"))		// trace insns
		{
			if (sscanf(args, " %63s %63s", p1, p2) == 2)
				cli_t(strtoul(p1, 0, 0), strtoul(p2, 0, 0), 1);
			else if (sscanf(args, " %63s", p1) == 1)
				cli_t(strtoul(p1, 0, 0), 1);
			else
				cli_t(1);
			strcpy(last, "t");			// suppress the args when repeating the command
		}
		else if (!strcmp(cmd, "ti"))	// trace insns w/o reg dumps
		{
			if (sscanf(args, " %63s %63s", p1, p2) == 2)
				cli_t(strtoul(p1, 0, 0), strtoul(p2, 0, 0), 0);
			else if (sscanf(args, " %63s", p1) == 1)
				cli_t(strtoul(p1, 0, 0), 0);
			else
				cli_t(0);
			strcpy(last, "ti");			// suppress the args when repeating the command
		}
		else if (!strcmp(cmd, "g"))		// go [entry]
		{
			if (sscanf(args, " %63s", p1) == 1)
				cli_g(strtoul(p1, 0, 0));
			else
				cli_g();
			strcpy(last, buf);
		}
		else if (!strcmp(cmd, "d"))		// dump memory
		{
			int mw = mem->getMemoryWarnings();
			mem->setMemoryWarnings(0);

			uint64_t addr = d_next;
			uint64_t count = 0x100;
			if (sscanf(args, " %63s %63s", p1, p2) == 2)
			{
				addr = strtoul(p1, 0, 0);
				count = strtoul(p2, 0, 0);
			}
			else if (sscanf(args, " %63s", p1) == 1)
			{
				addr = strtoul(p1, 0, 0);
			}
			cli_d(addr, count);
			d_next = addr+count;
			mem->setMemoryWarnings(mw);
			strcpy(last, "d");			// suppress the args when repeating the command
		}
		else if (!strcmp(cmd, ">"))		// redirect output
		{
			if (sscanf(args, " %1023s", p1) != 1)
			{
				printf("Invalid redirect (missing filename?)\n");
				break;
			}
			if (!strcmp(p1, "-"))
				redirect(0);
			else
				redirect(p1);
			last[0] = '\0';
		}
		else if (!strcmp(cmd, "a"))		// change register naming mode
		{
			cpu->setRegNamesABI(!cpu->getRegNamesABI());
			last[0] = '\0';
		}
		else if (!strcmp(cmd, "?"))		// help
		{
			printf("commands:\n"
				"   a                 toggle the display of register ABI and x-names\n"
				"   d [addr [len]]    dump memory starting at addr for len bytes\n"
				"   g [addr]          set pc=addr and silently execute qty instructions\n"
				"   r                 dump the contents of the CPU regs\n"
				"   t [[addr] qty]    set pc=addr and trace qty instructions\n"
				"   ti [[addr] qty]   set pc=addr and trace qty instructions w/o reg dumps\n"
				"   x                 exit\n"
				"   > filename        redirect output to 'filename' (use - for stdout)\n"
				);
		}
		else
		{
			printf("Illegal command.  Press ? for help.\n");
		}
	}
}
