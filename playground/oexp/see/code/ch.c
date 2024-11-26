//
// ch.c
// gcc thisfile
//
// The default type of char is 'unsigned char' in the Kunpeng platform.
//
// gcc thisfile in both x86 and Kunpeng architecture. And try to 
// gcc -fsigned-char thisfile in Kunpeng architecture. And try to 
// gcc -funsigned-char thisfile in x86 architecture
//

#include <stdio.h>

// positive or non positive, is a problem
#define PNP(x) \
        ((x) < 0 ? printf("negative") : \
        ((x) == 0) ? printf("zero") : \
        printf("positive"))

int main() {
    //    original code of  1: 0000 0001b
    // ones' complement of  1: 1111 1110b
    // two's complement of -1: 1111 1111b
    char ch = -1;

    printf("sizeof ch is %zu, %zu, %zu, %zu\n\n", sizeof(char), sizeof(ch), sizeof(signed char), sizeof(unsigned char));
	
    printf("         char ch = %2xh, %+4d, ",                ch & 0xff,                ch);                PNP(ch); printf("\n\n");
    printf("  signed char ch = %2xh, %+4d, ",   (signed char)ch & 0xff,   (signed char)ch);   PNP((signed char)ch); printf("\n");
    printf("unsigned char ch = %2xh, %+4d, ", (unsigned char)ch & 0xff, (unsigned char)ch); PNP((unsigned char)ch); printf("\n");

    return 0;
}
