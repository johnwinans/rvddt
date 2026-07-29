
#define COUT(X)		do {*(volatile unsigned char *) 0xffff0001 = (X);} while (0)
#define CIN()		(*(volatile unsigned char *) 0xffff0000)
#define KB_HIT()	(-1 != *(volatile unsigned char *) 0xffff0002)

#define PRHHEX(X) COUT("0123456789ABCDEF"[0x0F & (X)])
#define PRHEX(X)  do { PRHHEX(X >> 4); PRHHEX(X); } while (0)
#define STROUT(X) do { for (int i=0;(X)[i];i++) COUT((X)[i]);} while (0)

int main()
{
	char c;

	STROUT("? Enter:");
	while (!KB_HIT());
	c = CIN();
	COUT(c);
	STROUT(" 0x");
	PRHEX(c);
	COUT('\n');
	for (;;)
		asm volatile ("ebreak");
}


