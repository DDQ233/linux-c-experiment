#include <stdio.h>
#include "mymath.h"

int main()
{
	int a, b;
	scanf("%d%d",&a, &b);
	printf("max = %d, min = %d\n", getMax(a, b), getMin(a, b));
	return 0;
}
