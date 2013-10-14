#include <stdio.h>

void dumper( unsigned char *x, int n, int sgn ) ;

int main() {
    int i;
    //printf("%.8x\n",i);
    dumper( (unsigned char *) &i, 4, 1 ) ;
}

void dumper( unsigned char *x, int n, int sgn ) {
/* *x is any address in accessible memory */
/* n is the number of 4 x 4 byte words to print */
/* sgn is +/-1 to indicate whether to go ascending (+1) or descending (-1) */
    //Rounds down by 16
    int *temp = (int *) (& x);
    *temp >>= 4;
    *temp <<= 4;

    int i,j,k;
    //Iterates through the rows of 4 x 4-byte words
    for (i=0;i<n;i++) {
        printf("%p  ",x+(sgn*16*i));
        //Iterates through the 4-byte words in each row
        for (j=0;j<4;j++) {
            //Iterates through each byte in a 4-byte word and accounts for little endian
            for (k=3;k>=0;k--) {
                //x+(sgn*16*i) accounts for cur row and pos/neg sgn
                //4+j accounts for current word in the row
                //k accounts for current byte in the word
                printf("%.2x",*(x+(sgn*16*i)+(4*j)+k));
            }
            printf("  ");
        }
        printf("\n");
    }
}
