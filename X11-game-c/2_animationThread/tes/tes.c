#include <stdio.h>

int matrix32[][2] =
{
	{2, 1},
	{3, 4},
};

int main()
{
	int x = matrix32[0][0];

	printf("%d\n", x);
	return 0;
}
