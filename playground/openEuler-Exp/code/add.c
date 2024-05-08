/* 

add.c
gcc thisfile

R0-R30 31 general-purpose registers, R0 to R30. Each register can be accessed as:
o	A 64-bit general-purpose register named X0 to X30.
o	A 32-bit general-purpose register named W0 to W30.

63                             32 31                              0
|================================|================================|
                                 |<--------------Wn-------------->|
|<-------------------------------Xn------------------------------>|

*/

#include <stdio.h>
#include <stdlib.h>

int add(int i, int j) {
        int res = 0;
        asm (
                "ADD %w[result], %w[input_i], %w[input_j]"
                : [result] "=r" (res)
                : [input_i] "r" (i), [input_j] "r" (j) );

        return res;
}

int main(int argc, char *argv[]) {
        if (argc < 3) {
                printf("Usage: %s <int int>\n", argv[0]);
                return -1;
        }

        int i = atoi(argv[1]);
        int j = atoi(argv[2]);

        printf("%d + %d = %d\n", i, j, add(i, j));
        return 0;
}
