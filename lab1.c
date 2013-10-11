#include <stdio.h>

#include <math.h>

void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;

void main() {
   int test = 1024; 
   int bit_length = 12;
   int *bit, *err;
   to_binary(test,bit_length,bit,err);
   if (*err == 1) {
        return;
    } 
   for (int i = 0; i < bit_length; i++) {
    printf("%d",*(bit++));
   } 
    return;
}

void main_back()
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
      for (int i=0; i<w; i++) {
         *(z+i) = (*x ^ *y) ^ carry;
         if (*z == 0) {
            carry = 1;
         } else {
            carry = 0;
         }
      }
      if (carry) {
         *o = 1;
      }
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
      if (n>=0) {
         int temp = n;
         int neg = 0;
      } else {
         int temp = -n;
         int neg = 1;
      }
      for (int i=0; i<w-1; i++) {
         *(x+i) = neg ? ~(n % 2) : n % 2;
         temp = n/2;
      }
      if (temp != 0) {
         *o = 1
         return;
      } else if (neg) {
         //Kinda dangerous 1 level recursiveness, but whatever
         //It shouldn't infinite loop because 1 is not negative...
         int *bit = malloc(sizeof(int)*w);
         int *p;
         to_binary(1,w,bit,p);
         if (*p == 1) {
            //The program should never hit this block of code...
            o = p;
            return;
         } else {
            int *result = malloc(sizeof(int)*w);
            adder(x,bit,result,p,w-1)
            if (*p == 1) {
               o = p;
               return
            } else {
               x = result;
               x[w-1] = 1; 
               return;
            }
         }   
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

