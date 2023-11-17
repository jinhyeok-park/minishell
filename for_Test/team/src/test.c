#include "minishell.h"

int main(void)
{
    int fd = open("test", O_RDONLY);
    dup2(fd, 0);
    system("cat");

}