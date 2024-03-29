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

Memory map

	ffff0001-ffff0001	console printer transmit register

	f0000214-f0000217	memory card status register
	f0000210-f0000213	memory card control register
	f0000200-f0000203	memory card address register
	f0000000-f00001ff	512-byte memory card DMA buffer 
	
	00000040-0000ffff	RAM *
	00000000-0000003f	vectors

* The amount of RAM in the system can be specidfied by the -s and -l arguments to rvddt

A simplified 'printer' device that runs infinitely fast and that prints 
bytes that are sent to it on the terminal has a 8-bit transmit register 
at address 0xffff0001. 

The simplified 'memory card' is simulated by using a file to contain
the card's data bytes.

Writing data to the memory card:

* Wait for the memory card to become IDLE by checking the status register.
* Place 512 bytes to be written into the memory card buffer: f0000000-f00001ff.
* Write the memory card block number into the memory card address register.
* Write the 'write command' onto the memory card control register.

The data transfer simulation will take some number of instruction cycles to complete.
The busy/idle status is always available in the memory card status register.

Memory card control register commands:

	0x80000001 = write the 512-byte DMA buffer into the memory card at the address in the memory card address register*0x200.

	0x80000002 = read 512 bytes from the memory card at the address in the memory card address register*0x200 into the 512-byte DMA buffer.

Memory card status register:

	0x00000000 = idle
	0x00000001 = busy writing
	0x00000002 = busy reading
