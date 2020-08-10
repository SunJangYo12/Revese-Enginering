#include <stdio.h>

int tes[][2] =
{
    {2, 5},
    {4, 3},
    {11, -11},
    {12, -12},
    {13, -13},
    {14, -14},
    {15, -15},
};

void draw(int state)
{
    int x = state;
    int y = sss;

    printf("%d %d\n", x, y);
}

int main() {
    int bilangan[2][2];
    int matrix[2][3] = 
    { 
        {1, 2, 3}, 
        {7, 8, 9} 
    };

    int matrix3d[2][3][4] =
    {
        { 
            {7, 4, 12, 3},
            {-9, 29, 3, 11}, 
            {6, 34, 23, 20} 
        },
        { 
            {6, 15, 1, 5}, 
            {17, 8, -3, 15}, 
            {99, -1, 44, 9} 
        }
	};

	bilangan[0][0] = 100;
	bilangan[0][1] = 101;
	bilangan[1][0] = 110;
	bilangan[1][1] = 111;

	printf("Isi array bilangan: \n");
	printf("%d, %d \n", bilangan[0][0], bilangan[0][1]);
	printf("%d, %d \n", bilangan[1][0], bilangan[1][1]);

	printf("\nIsi array matrix: \n");
	printf("%d  %d  %d\n", matrix[0][0], matrix[0][1], matrix[0][2]);
	printf("%d  %d  %d\n", matrix[1][0], matrix[1][1], matrix[1][2]);

	printf("Isi matrix3d[0][0][0]: %d\n", matrix3d[0][0][0]);
	printf("Isi matrix3d[0][1][0]: %d\n", matrix3d[0][1][0]);
	printf("Isi matrix3d[1][1][3]: %d\n", matrix3d[1][1][3]);
	printf("Isi matrix3d[1][2][3]: %d\n", matrix3d[1][2][3]);

    printf("\n\n%d\n", tes[2][1]);

	return 0;
}
