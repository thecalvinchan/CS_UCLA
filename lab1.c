#include <stdio.h>

#include <math.h>

void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;

void test() {
   int test = -128; 
   int bit_length = 8;
   int bit[32];
   int err;
   to_binary(test,bit_length,bit,&err);
   if (err) {
      printf("Overflow in to_binary\n");
      return;
   }
   for (int i = 0; i < bit_length; i++) {
    printf("%d",bit[i]);
   } 
   printf("%c",'\n');
   int test2 = -128; 
   int bit_length2 = 8;
   int bit2[32];
   int err2;
   to_binary(test2,bit_length2,bit2,&err2);
   if (err) {
      printf("Overflow in to_binary\n");
      return;
   }
   for (int i = 0; i < bit_length2; i++) {
        printf("%d",bit2[i]);
   } 
   printf("%c",'\n');
   int bit3[32];
   adder(bit,bit2,bit3,&err2,bit_length2);
   if (err2 == 1) {
      printf("Overflow in adder\n");
      return;
   }
   for (int i = 0; i < bit_length2; i++) {
        printf("%d",bit3[i]);
   } 
   printf("%c",'\n');
}

void main()
   {
   int a,b,c,d,w,n,o ;
   int x[32], y[32], z[32] ;

   w = 8 ; /* word size in bits */
           /* note that x, y, and z must be at least as large as w and may be larger */

   b = 1 ; /* add one to each */
   for( c=125;c<135;c++ )
      {
      if ( c > 127 )  /* exceeds 7 bits */
         d = c-256 ;
      else
         d =  c ;
      printf( "%d %d ", d,b ) ;
      to_binary( d,w,x,&o ) ;
      if ( o )
         printf( "%d Overflow in to_binary! ",d ) ;
      to_binary( b,w,y,&o ) ;
      if ( o )
         printf( "%d Overflow in to_binary! ",b ) ;
      adder( x,y,z,&o,w ) ;
      from_binary( z,w,&a ) ;
      printf( "%d ", a ) ;
      if ( o )
         printf( "Overflow in adder! " ) ;
      printf( "\n" ) ;
      }

   b = -1 ; /* add -1 to each */
   for( c=125;c<135;c++ )
      {
      if ( c > 127 )
         d = c-256 ;
      else
         d =  c ;
      printf( "%d %d ", d,b ) ;
      to_binary( d,w,x,&o ) ;
      if ( o )
         printf( "%d Overflow in to_binary! ",d ) ;
      to_binary( b,w,y,&o ) ;
      if ( o )
         printf( "%d Overflow in to_binary! ",d ) ;
      adder( x,y,z,&o,w ) ;
      from_binary( z,w,&a ) ;
      printf( "%d ", a ) ;
      if ( o )
         printf( "Overflow in adder! " ) ;
      printf( "\n" ) ;
      }


   for( w=8; w<13; w=w+4 )          /* word sizes 8 and 12 bits */
      for( a=99; a >-100; a=a-99 )  /* 99, 0, -99 */
         for( b=4; b>-5; b=b-4 )    /* 4, 0, -4 */ 
            {
            to_binary( a,w,x,&o ) ;
            if ( o )
               printf( "%d Overflow in to_binary! ",a ) ;
            to_binary( b,w,y,&o ) ;
            if ( o )
               printf( "%d Overflow in to_binary! ",b ) ;
            mult( x,y,z,&o,w ) ;
            printf( "%d %d ", a,b ) ;
            from_binary( z,w,&n ) ;
            printf( "%d ", n ) ;
            if ( o )
               printf( "Overflow in multiplier! " ) ;
            printf( "\n" ) ;
            }
   }

void adder( int *x, int *y, int *z, int *o, int w )
   {
   /* x is an input array of w ints, either 0 or 1 representing the first addend */
   /* y is an input array of w ints, either 0 or 1 representing the second addend */
   /* z is an output array of w ints, either 0 or 1 representing the sum of x and y */
   /* o is an output = 1 if an overflow occurred */
   /* w is an input = to the width in bits of x, y, z */
      int carry = 0;
      int limit;
      if (x[w-1] == 0 &&  y[w-1] == 0) {
         limit = w - 1;
      } else {
         limit = w;
      }
      for (int i=0; i<limit; i++) {
         z[i] = (x[i] ^ y[i]) ^ carry;
         //printf("%d\n",z[i]);
         if ((z[i] == 0) && (x[i] | y[i])) {
            carry = 1;
         } else if ((z[i] == 1) && (x[i] & y[i])) {
            carry = 1;
         } else {
            carry = 0;
         }
      }
      // If both of the addends have the same 
      // greatest significant bit (same sign)
      // then it is possible to overflow if
      // there exists a carry
      if (!(x[w-1]^y[w-1]) & carry) {
         if (x[w-1] == 0 && y[w-1] == 0) {
             //printf("Overflow\n");
             *o = 1;
             return;
         } else if (z[w-1] == 0) {
             //printf(")verflow\n");
            return;
            *o = 1;
         }
      } 
      // It is not possible for the addition 
      // to overflow if there are two addends with
      // differing greatest significant bits
      *o = 0;
      return;
   }
void mult ( int *x, int *y, int *z, int *o, int w )
   {
   /* x is an input array of w ints, either 0 or 1 representing the first multiplicand */
   /* y is an input array of w ints, either 0 or 1 representing the second multicand */
   /* z is an output array of w ints, either 0 or 1 representing the product of x and y */
   /* o is an output = 1 if an overflow occurred */
   /* w is an input = to the width in bits of x, y, z */



   }
void to_binary( int n, int w, int *x, int *o )
   {
   /* n is an input and is the number to be converted to binary */
   /* w is an input = to the width in bits of x */
   /* x is an output array of w ints, either 0 or 1: the binary representation of n */
   /*           x[0] is the least significant bit   */
   /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
   /* o is an output = 1 if an overflow occurred ( n is too large for w bits ) */
      int temp, neg;
      if (n>=0) {
         neg = 0;
         x[w-1] = 0;
      } else {
         neg = 1;
         x[w-1] = 1;
      }
      temp = n;
      for (int i=0; i<w-1; i++) {
         x[i] = neg ? !(temp % 2) : temp % 2;
         //printf("%d\n",x[i]);
         temp = temp/2;
      }
      if (temp != 0 && !neg) {
         *o = 1;
         return;
      } else if (neg) {
         //Kinda dangerous 1 level recursiveness, but whatever
         //It shouldn't infinite loop because 1 is not negative...
         int bit[32];
         to_binary(1,w,bit,o);
         if (*o == 1) {
            //The program should never hit this block of code...
            return;
         } else {
            int result[32];
            adder(x,bit,result,o,w);
            if (*o == 1) {
               return;
            } else {
               for (int i=0;i<w;i++) {
                  x[i] = result[i];
               }
               x[w-1] = 1; 
               *o = 0;
               return;
            }
         }   
      } else {
         *o = 0;
         return;
      }
   }

void from_binary( int *x, int w, int *n )
   {
   /* x is an input array of w ints, either 0 or 1: the binary representation of n */
   /*           x[0] is the least significant bit   */
   /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
   /* w is an input = to the width in bits of x */
   /* n is an output and will be equal to the value stored in binary in x */


   }

