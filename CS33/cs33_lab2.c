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
    int *temp = (int *) (& x);
    *temp >>= 4;
    *temp <<= 4;

    int i,j,k;
    for (i=0;i<n;i++) {
        printf("%p  ",x+(sgn*16*i));
        for (j=0;j<4;j++) {
            for (k=3;k>=0;k--) {
                printf("%.2x",*(x+(sgn*16*i)+(4*j)+k));
            }
            printf("  ");
        }
        printf("\n");
    }
}
