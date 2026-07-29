#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO, read

#include "console.h"
//#include "dev/console.h"

/*
 * statusRegAddr
 * TxRegAddr 
 * RxRegAddr 
 */

console::console() : c{0}, s{-1}
{
    static struct termios newt;

    pOldt = new struct termios;
    /* tcgetattr gets the parameters of the current terminal STDIN_FILENO will tell tcgetattr that it should write the
     * settings of stdin to oldt */
    tcgetattr(STDIN_FILENO, pOldt);
    /*now the settings will be copied*/
    newt = *pOldt;

    /* ICANON normally takes care that one line at a time will be processed that means it will return if it sees a "\n"
     * or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);

    /* Those new settings will be set to STDIN TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
}

console::~console()
{
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, pOldt);
	delete pOldt;
}

void console::set8(uint64_t addr, uint8_t val)
{
	if (TxRegAddr == addr)
	{
        putchar(val);
		fflush(stdout);
	}
}
int8_t console::get8(uint64_t addr)
{
	if (RxRegAddr == addr)
	{
        s = (s < 0) ? read(0, &c, 1) : -1;
        return c;
	}
	else if	(statusRegAddr == addr)
        return s < 0 ? (s = read(0, &c, 1)) : s;
    else return -1;
}
