# rvddt
RISC-V Dynamic Debugging Tool

This is an RV32I simulator that was written to gernerate the figures for a book 
in progress here: https://github.com/johnwinans/rvalp that will (eventually) include 
several examples on how to use rvddt and other tools mentioned below.

To build everything:

    mkdir -p ~/projects/riscv
    cd ~/projects/riscv
    git clone https://github.com/johnwinans/rvddt.git
    cd rvddt/src
    make world
    cd ../examples
    make world
    echo "export PATH=$PATH:~/projects/riscv/rvddt/src" >> ~/.bashrc

Note that you will need a suitable compiler to build the examples.  
On Ubuntu, I installed mine like this:

	sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev \
		libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils \
		bc zlib1g-dev libexpat-dev
    mkdir -p ~/projects/riscv
    cd ~/projects/riscv
    git clone https://github.com/riscv/riscv-gnu-toolchain
    cd riscv-gnu-toolchain
    INS_DIR=~/projects/riscv/install/rv32i
    ./configure --prefix=$INS_DIR --with-arch=rv32i --with-abi=ilp32
    make
    echo "export PATH=$PATH:~/projects/riscv/install/rv32i/bin" >> ~/.bashrc

If you are new to building the GNU tools be careful about following the numerous 
opinions on how to configure them.  `rvddt` will ONLY execute instructions in the 
`rv32i` base ISA.  Configure and/or use the tools appropriately!

See the rvddt examples and their makefiles for help figuring out how to run the compiler.

After building, the rvddt executable will be left sitting in the `src` directory.  
Put it wherever you want to install it.  I just run it from the src directory like this:

    rvddt -f proggie.bin

To ask for command-line help, run it like this:

    rvddt -h
  
All the regs ('cept x0) are set to 0xf0f0f0f0 and the memory is filled with
0xa5 prior to reading the binary memory image into memory starting at address 
zero and initializing the sp register and priming the stack top as discussed below.

By default rvddt will create a memory space starting at 0 with a size of 0x10000, 
set the pc register to zero, the sp register to the last memory address +1.  You can see this
initialization in the pre-trace register dump in the same output below.
(Note that you can change the memory size with the`-l <memlen>` arg and the stack 
initialization will follow it.)

If your program reads or writes outside of the simulated memory region a warning 
will be printed to help you track down the instruction that did it.  In dumps, such 
'unimplemented' memory bytes are all 0xff.

The simulator will stop on an illegal instruction or an `ebreak`.

To see a list of commands type `?` at the prompt:

    ddt> ?
    commands:
       a                 toggle the display of register ABI and x-names
       d [addr [len]]    dump memory starting at addr for len bytes
       g [addr]          set pc=addr and silently execute qty instructions
       r                 dump the contents of the CPU regs
       t [[addr] qty]    set pc=addr and trace qty instructions
       ti [[addr] qty]   set pc=addr and trace qty instructions w/o reg dumps
       x                 exit
       > filename        redirect output to 'filename' (use - for stdout)
    ddt> 

The way you use rvddt is to create an executable binary memory image with 
its entry point (the first instruction to execute) located at address 0 and then
run it like this:

    $ rvddt -f examples/load4regs/load4regs.bin
    sp initialized to top of memory: 0x00010000
    Loading 'examples/load4regs/load4regs.bin' to 0x0
    This is rvddt.  Enter ? for help.
    ddt> t1000
       x0 00000000 f0f0f0f0 00010000 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       pc 00000000
    00000000: 00000e13  addi    x28, x0, 0    # x28 = 0x00000000 = 0x00000000 + 0x00000000
       x0 00000000 f0f0f0f0 00010000 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  00000000 f0f0f0f0 f0f0f0f0 f0f0f0f0
       pc 00000004
    00000004: 00000e93  addi    x29, x0, 0    # x29 = 0x00000000 = 0x00000000 + 0x00000000
       x0 00000000 f0f0f0f0 00010000 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  00000000 00000000 f0f0f0f0 f0f0f0f0
       pc 00000008
    00000008: 00000f13  addi    x30, x0, 0    # x30 = 0x00000000 = 0x00000000 + 0x00000000
       x0 00000000 f0f0f0f0 00010000 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  00000000 00000000 00000000 f0f0f0f0
       pc 0000000c
    0000000c: 00000f93  addi    x31, x0, 0    # x31 = 0x00000000 = 0x00000000 + 0x00000000
       x0 00000000 f0f0f0f0 00010000 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
       x8 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x16 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
      x24 f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0  00000000 00000000 00000000 00000000
       pc 00000010
    00000010: ebreak
    ddt>

If you prefer the ABI register names, then use the `a` command and run it like this:

    $ ./src/rvddt -f examples/counter/counter.bin
    sp initialized to top of memory: 0x00010000
    Loading 'examples/counter/counter.bin' to 0x0
    This is rvddt.  Enter ? for help.
    ddt> a
    ddt> t
      zero 00000000  ra f0f0f0f0  sp 00010000  gp f0f0f0f0
        tp f0f0f0f0  t0 f0f0f0f0  t1 f0f0f0f0  t2 f0f0f0f0
        s0 f0f0f0f0  s1 f0f0f0f0  a0 f0f0f0f0  a1 f0f0f0f0
        a2 f0f0f0f0  a3 f0f0f0f0  a4 f0f0f0f0  a5 f0f0f0f0
        a6 f0f0f0f0  a7 f0f0f0f0  s2 f0f0f0f0  s3 f0f0f0f0
        s4 f0f0f0f0  s5 f0f0f0f0  s6 f0f0f0f0  s7 f0f0f0f0
        s8 f0f0f0f0  s9 f0f0f0f0 s10 f0f0f0f0 S11 f0f0f0f0
        t3 f0f0f0f0  t4 f0f0f0f0  t5 f0f0f0f0  t6 f0f0f0f0
       pc 00000000
    00000000: 00300293  addi    t0, zero, 3   # t0 = 0x00000003 = 0x00000000 + 0x00000003
    ddt>

Note that when an instruction is traced a comment is displayed explaining what the
instruction did and what values it used.  I originally added this feature to debug rvddt. 
But I think it is very useful instructional tool... so I kept it in.

rvddt is more of a STATIC debugging tool at the moment. :smile:  It has no commands for the 
editing of registers or memory.
