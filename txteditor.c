#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode()
{
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios; /*terminal attributes get read in this struct*/
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~ (OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); /*causes each inputed key to be printed*/
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); /*applies terminal attributes*/
}

int main()
{
	enableRawMode();

	char c; /*characters being read*/
	while (read(STDIN_FILENO, &c, 1) == 1 && c!= 'q')
	{
		if (iscntrl(c))
		{
			printf("%d\r\n", c);
		}
		else
		{
			printf("%d ('%c')\r\n", c, c);
		}
	}
	return 0;
}
