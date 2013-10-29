int beg = 0;
char *min_ptr = &beg;
char *max_ptr = &beg;

void spray_paint( char *x, int sz, char tok, char *t ) 
{
/* **** your new code ****/
    int i = 0;
    if (*min_ptr == 0 && *max_ptr == 0) {
        min_ptr = x;
        //printf("min_pr %p\n",min_ptr);
        max_ptr = x;
        //printf("max_pr %p\n",max_ptr);
    }
    for (i=0;i<sz;i++) {
        //printf("size %d ",sz);
        //printf("%c \n",*(x+i));
        //printf("current ptr %p\n",x+i);
        *(x+i) = tok;
        if (min_ptr > (x+i*sizeof(char))) {
            min_ptr = x+i;
            //printf("min_pr %p\n",min_ptr);
        } 
        if (max_ptr < (x+i*sizeof(char))) {
            max_ptr = x+i;
            //printf("max_pr %p\n",max_ptr);
        }
    }
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

void sub2()
{
int i ;
char* a ;
char x[20] ;

printf("%p\n",&i);
printf("%p\n",&a);
printf("%p\n",&x);
spray_paint( (char *) &i, sizeof(i), 0xf1, "sub2.i" ) ;
spray_paint( (char *) &a, sizeof(a), 0xf2, "sub2.a" ) ;
spray_paint( (char *) &x, sizeof(x), 0xf3, "sub2.x" ) ;
printf("%p\n",min_ptr);
printf("%p\n",max_ptr);
int temp = max_ptr-min_ptr;
printf("%d\n",temp);
dumper(min_ptr-16,temp/16+3,1);
/* dumper(...) *** insert your code here to get the right answer*****/
}
void sub1()
{
float i ;
short a ;
int x[20] ;
printf("%p\n",&i);
printf("%p\n",&a);
printf("%p\n",&x);
spray_paint( (char *) &i, sizeof(i), 0xe1, "sub1.i" ) ;
spray_paint( (char *) &a, sizeof(a), 0xe2, "sub1.a" ) ;
spray_paint( (char *) &x, sizeof(x), 0xe3, "sub1.x" ) ;
sub2() ;
}
int main()
{
struct mine {
char* a ;
char x[20] ;
float z ;
} i;

union crazy {
float a ;
char b ;
int s ;
} a ;

char x[50] ;
printf("%d\n",sizeof(i));

printf("%p\n",&i);
printf("%p\n",&a);
printf("%p\n",&x);
spray_paint( (char *) &i, sizeof(i), 0xd1, "main.i" ) ;
spray_paint( (char *) &a, sizeof(a), 0xd2, "main.a" ) ;
spray_paint( (char *) &x, sizeof(x), 0xd3, "main.x" ) ;

sub1() ;

return 0 ;
}
