#include <unistd.h>
#include <stdio.h>

int main()
{
	char *const argv[] = {"-e", "Hello", "World"};
	int ya =  execve("/bin/echo", argv, NULL);
	return (0);
}
