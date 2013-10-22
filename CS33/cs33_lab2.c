#include <stdio.h>

void dumper( unsigned char *x, int n, int sgn ) ;
int endian();

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
    int *temp = (int *) (&x);
    *temp >>= 4;
    *temp <<= 4;

    int i,j,k;
    //Iterates through the rows of 4 x 4-byte words
    for (i=0;i<n;i++) {
        printf("%p  ",x+(sgn*16*i));
        //Iterates through the 4-byte words in each row
        for (j=0;j<4;j++) {
            //Iterates through each byte in a 4-byte word and accounts for little/big endian
            if (endian() == 0) {
                for (k=3;k>=0;k--) {
                    //x+(sgn*16*i) accounts for cur row and pos/neg sgn
                    //4+j accounts for current word in the row
                    //k accounts for current byte in the word
                    printf("%.2x",*(x+(sgn*16*i)+(4*j)+k));
                }
            } else {
                for (k=0;k<4;k++) {
                    //x+(sgn*16*i) accounts for cur row and pos/neg sgn
                    //4+j accounts for current word in the row
                    //k accounts for current byte in the word
                    printf("%.2x",*(x+(sgn*16*i)+(4*j)+k));
                }
            }
            printf("  ");
        }
        printf("\n");
    }
}

int endian() {
    //Modified code from hw1 solution 
    char *cpx,*cpy ;
    char c[4] ;
    int  i,j,k,l ;

    cpx = (char *) &c ;
    cpy = (char *) &i ;

    *(cpx+0) = 0x01 ; /* explicit assignment */
    *(cpx+1) = 0x23 ;
    *(cpx+2) = 0x45 ;
    *(cpx+3) = 0x67 ;
    i = 19088743; /* should be 0x01234567 */
    k = 0;
    l = 256*256*256 ;
    for ( j=0; j<4; j++ ) { /* prints i as c[4] */
        k = k+(char) *(cpy+j) * l ;
        l = l/256 ;
    }

    if ( k == i ) {
        //big endian
        return 1;
    } else {
        //little endian
        return 0;
    }
}
