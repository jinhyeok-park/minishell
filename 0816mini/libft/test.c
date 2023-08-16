#include "libft.h"
#include <stdio.h>

int main(void)
{
	char	*temp;
	char	*temp1;
	char	*temp2;
	char	**sp;
	char	**sp1;
	char	**sp2;
	
	temp = "$str";
	temp1 = "awe$str";
	temp2 = "awe$";
	sp = ft_split(temp, '$');
	sp1 = ft_split(temp1, '$');
	sp2 = ft_split(temp2, '$');
	for (int i = 0; sp[i] ; i++)
	{
		printf("%d, %s", i,sp[i]);
	}

	printf("\n");
	for (int i = 0; sp1[i] ; i++)
	{
		printf("%s", sp1[i]);
	}
	printf("\n");
	for (int i = 0; sp2[i] ; i++)
	{
		printf("%s", sp2[i]);
	}
}
