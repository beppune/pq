/* includes */

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* define */
#define CTRL_KEY(k) ((k) & 0x1f)

/* data */
struct termios orig_termios;

/* terminal */
int die(const char *s) {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	exit(1);
	return 1;
}

void disable_raw() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == 0 || die("tcgetattr"); 
}

void enable_raw() {
	tcgetattr(STDIN_FILENO, &orig_termios) == 0 || die("tcgetattr"); 
	atexit(disable_raw);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0 || die("tcgetattr");
}

int read_key() {
	int nread;
	char c;
	while( (nread = read(STDIN_FILENO, &c, 1)) != 1 ) {
		if( nread == -1 && errno != EAGAIN ) die("read");
	}
	return c;
}

/* output */
void refresh_screen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

/* input */
int process_key() {
	char c = read_key();

	switch(c) {
		case CTRL_KEY('q'):
			refresh_screen();
			exit(0);
			break;
	}
}

/* init */
int main(int argc, char *argv[]) {

	enable_raw();

	char c;
	while ( 1 ) {
		refresh_screen();
		process_key();
	}

	return 0;
}
