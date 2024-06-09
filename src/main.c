#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios orig_termios;

void disable_raw() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disable_raw);

	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char *argv[]) {
	
	enable_raw();

	char c;
	while ( read(STDIN_FILENO, &c, 1) == 1 && c != 'q' ) {
		if( iscntrl(c) ) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
	}

	return 0;
}