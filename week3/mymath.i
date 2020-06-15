# 1 "mymath.c"
# 1 "<built-in>"
# 1 "<命令行>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<命令行>" 2
# 1 "mymath.c"
# 1 "mymath.h" 1



int getMax(int x, int y);

int getMin(int x, int y);
# 2 "mymath.c" 2

int getMax(int x, int y)
{
 return x>y?x:y;
}

int getMin(int x, int y)
{
 return x<y?x:y;
}
