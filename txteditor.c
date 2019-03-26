#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void die (const char *s) /*error handling and exits program*/
{
	perror(s);
	exit(1);
}

void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
		dir ("tcsetattr");
	}
}

void enableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
	{
		die("tcsetattr");
	}
	atexit(disableRawMode);

	struct termios raw = orig_termios; /*terminal attributes get read in this struct*/
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~ (OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); /*causes each inputed key to be printed*/
	raw.c_cc[VMIN] = 0; /*set to 0 so that read() is returned as soon as there is any input to be read*/
	rawc_cc[VTIME] = 1; /*sets mac time to wait before read() returns*/

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw))
	{
		die("tcseattr");
	} /*applies terminal attributes*/
}

int main()
{
	enableRawMode();

	while (1) 
	{
		char c = '\0';
		if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
		{
			die("read");
		}
		if (iscntrl(c))
		{
			printf("%d\r\n", c);
		}
		else
		{
			printf("%d ('%c')\r\n", c, c);
		}
		if(c == 'q') /*quits the text editor if user press 0 */
		{
			break;
		}
	}
	return 0;
}
