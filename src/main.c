/* includes */

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
/* define */ #define CTRL_KEY(k) ((k) & 0x1f)
/* data */
struct editor_state {
	int screencols;
	int screenrows;
	struct termios orig_termios;
};

struct editor_state E;

/* terminal */
int die(const char *s) {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	exit(1);
	return 1;
}

void disable_raw() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == 0 || die("tcgetattr"); 
}

void enable_raw() {
	tcgetattr(STDIN_FILENO, &E.orig_termios) == 0 || die("tcgetattr"); 
	atexit(disable_raw);

	struct termios raw = E.orig_termios;
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

int get_window_size(int *cols, int *rows) {
	struct winsize ws;

	if( ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0 ) {
		return -1;
	}
	
	*cols = ws.ws_col;
	*rows = ws.ws_row;

	return 0;
		
}

/* output */
void draw_rows() {
	for(int y = 0; y < E.screenrows; y++) {
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void refresh_screen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	draw_rows();

	write(STDOUT_FILENO, "\x1b[H", 3);
}


/* input */
int process_key() {
	char c = read_key();

	switch(c) {
		case CTRL_KEY('q'):
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
	}
}

/* init */

void init_editor() {
	if( get_window_size(&E.screencols, &E.screenrows) != 0 ) die("get_window_size");
}

int main(int argc, char *argv[]) {

	enable_raw();
	init_editor();

	char c;
	while ( 1 ) {
		refresh_screen();
		process_key();
	}

	return 0;
}
