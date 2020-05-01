#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>

int trc = 0;
int trc_r = 0;

void trace (int trc, const char * fmt, ...) {
	
	if (trc == 0)
		return;

	if (trc == 1) {
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}

void trace_reg (int trc_r, const char * fmt, ...) {
	
	if (trc_r == 0)
		return;

	if (trc_r == 1) {
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}

void test_wr() {
	
	// пишем байт, читаем байт
	
	byte b0 = 0x0a;
	b_write(2,b0); 
	byte bres = b_read(2);
	printf ("%02hhx = %02hhx\n", b0, bres);
	assert(b0 == bres);
	
	//пишем слово, читаем слово
	
	Adress adr = 4;
	//byte b1 = 0xcb;
	word wr = 0xcb0a;
	//b_write(a,b0);
	//b_write(a+1, b1);
	w_write(adr, wr);
	word wres = w_read(adr);
	printf ("%04hx = %04hx\n", wres, wr);
	assert(wr == wres);
	
	//пишем слово, читаем 2 байта
	
	Adress a = 4;
	byte b1 = 0xcb;
	byte b2 = 0x0a;
	word w = 0xcb0a;
	word res1 = b_read(a+1);
	word res2 = b_read(a);
	printf("%04hx = %02hhx%02hhx\n", w, res1, res2);
	assert (b1 == res1);
	assert (b2 == res2);
	
	//пишем 2 байта, читаем слово
	
	Adress A = 4;
	byte b3 = 0xfd;
	byte b4 = 0xcd;
	word wor = 0xfdcd;
	b_write(A+1, b3);
	b_write(A, b4);
	word res = w_read(A);
	printf("%02hhx%02hhx = %04hx\n", b3, b4, res);
	assert(res == wor);

}
	

void usage(char * prog) {
    printf("USAGE: %s [-t] / [-T] FILE\n", prog);
    printf("FILE\t - s-record file\n");
    printf("-t\t - turn on tracing (turn off by default)\n");
    printf("-T\t - turn on enlarged tracing (turn off by default)\n");
}
int main (int argc, char * argv[]) {
	
	
	if (argc == 1) {
        usage(argv[0]);
		exit(1);
	}
	
	for (int i = 1; i < argc-1; i++) {
		
		if (0 == strcmp(argv[i], "-t"))
			trc = 1;
		else if (0 == strcmp(argv[i], "-T")) {
			trc_r = 1;
			trc = 1;
		}
		else {
			printf("Unknown option %s\n", argv[i]);
			usage(argv[0]);
			exit(1);
		}
	}
    // load program and run it
	load_file(argv[argc-1]);
	run();
	
	return 0;
}
