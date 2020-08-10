#include <stdio.h>

struct TITIK
{
	int x;
	int y;
}T1;

struct TANGGAL
{
	char *tanggal;
	char *bulan;
	char *tahun;
}tgl_lahir = {"20", "03", "1991"};

struct STR1
{
	char a;
	char b;
};
struct STR2
{
	char a;
	int b;
};

int main()
{
	printf("koordinat T1:\n");
	printf("nilai x = ");scanf("%d", &T1.x);
	printf("nilai y = ");scanf("%d", &T1.y);
	printf("\nT1(%d, %d)\n", T1.x, T1.y);

	printf("Tanggal lahir : %s-%s-%s\n", tgl_lahir.tanggal, tgl_lahir.bulan, tgl_lahir.tahun);

	struct STR1 s1;
	struct STR2 s2;

	printf("sizeof(s1) = %ld\n", sizeof(s1));
	printf("sizeof(s2) = %ld\n", sizeof(s2));
}