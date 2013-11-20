#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define S       16     //  (2^4 = 16 cache sets)
#define E       1      //  (direct mapped cache)
#define B       32     //  (2^5 = 32 elements in each block)
#define T       7      //  (7 tag bits)
#define M       65536  //  (65536 byte memory)
#define READ    1
#define WRITE   0

int s ;
int b ;
int m ;

//
//  read hits   read misses     write hits      write misses    dirty writes
int rhits = 0 ; int rmiss = 0 ; int whits = 0 ; int wmiss = 0 ; int dwrit = 0 ;

struct cache_t
   {
   short dirty ;
   short valid ;  
   int   tag   ;
   int   *block ;
   } cache[S] ;

int *memory ;

int callno = 0 ;

int x  =     0 ;
int y  = 16384 ;
int ni =    20 ;
int nj =    20 ;
 
void stats( char *t )
   { 
   int i ;

   for( i=0; i<S; i++ )     // count number of left over dirty writes
      if( cache[i].dirty )
         dwrit = dwrit+1 ;
 

   printf( "%8s y=%5d ni=%5d nj=%5d rh=%5d rm=%5d wh= %5d wm= %5d dw= %5d\n", t, y,ni,nj,
            rhits, rmiss, whits, wmiss, dwrit ) ;		
   rhits = 0 ; rmiss = 0 ;
   whits = 0 ; wmiss = 0 ;
   dwrit = 0 ;

   }

void initcache()
   {
   int i ;

//
// put code to calculate s, b, m here
//
   s = log(S)/log(2);
   b = log(B)/log(2);
   m = log(M)/log(2);
   printf( "S= %d E= %d B= %d T= %d M= %d s= %d b= %d m= %d\n", S,E,B,T,M,s,b,m ) ;

//
// put code to initialize cache and memory here
//
   memory = malloc(M*sizeof(int));
   for (i=0;i<M;i++) {
      memory[i] = i;
   }
   for (i=0;i<S;i++) {
      cache[i].dirty = 0;
      cache[i].valid = 0;
      cache[i].block = malloc(B*sizeof(int));
   }

   }

void readwritecache( int readwrite, int a, int *value, int *hitmiss, int voice )
//
// readwrite = READ (1) for read, WRITE (0) from write
// a = binary memory address ( 0 <= a < 65535 )
// *value is memory value returned when readwrite = READ
//           memory value to store when readwrite = WRITE
// hiutmis is set to 0 for a miss, 1 for a hit, in either read or write case
// voice is a debugging switch
//
   {
//
// put code to simulate read and write of cache here
// pseudo code follows
//
//   compute si, ta, and bo from the address a
//           si = stack index
//           ta = tag
//           bo = block offset
   int extra = sizeof(int) * 8 - m;
   unsigned int si = a << (extra+T); 
       si = si >> (T+b+extra);
   unsigned int ta = a >> (s+b);
   unsigned int bo = (a << (extra+T+s));
       bo = bo >> (extra+T+s);
   if (cache[si].valid && cache[si].tag == ta) {
      if (readwrite == WRITE) {
         cache[si].block[b] = *value;
         cache[si].dirty = 1;
         *hitmiss = 1;
      } else {
         *value = cache[si].block[b];
         *hitmiss = 1;
      }
   } else {
      if (cache[si].dirty) {
         int *address = cache[si].block + b;
         memory[a] = *address;
         //wtf does count dirty write mean?
         dwrit++;
         cache[si].dirty = 0;
      }
      cache[si].valid = 1;
      cache[si].tag = ta;
      cache[si].block[b] = memory[a];
      if (readwrite == READ) {
         cache[si].dirty = 0;
         *value = cache[si].block[b];
         *hitmiss = 0;
      } else {
         cache[si].block[b] = *value;
         cache[si].dirty = 1;
         *hitmiss = 0;
      }
   }
   if (*hitmiss) {
      if (readwrite == READ) {
         rhits++;
      } else {
         whits++;
      }
   } else {
      if (readwrite == READ) {
         rmiss++;
      } else {
         wmiss++;
      }
   }

//   if( cache[si].valid && cache[si].tag = ta
//      if( readwrite == WRITE )
//         store value in cache
//         set dirty bit
//         count write hit
//     else
//         return memory value in *value
//         count read hit
//   else
//       if( cache[si].dirty )
//          calculate address of data residing in block // **think this over, it is not a! //
//          copy block to memory
//          count dirty write
//        set valid to 1
//        set tag to ta
//        copy memory to block
//        if( readwrite == READ )
//          set dirty to 0
//          return memory value in *value
//          count read miss
//        else
//          place *value in block
//          set dirty to 1
//          count write miss
//
   }

void locationexample()
   {
   int i,j,k,z ;

   for( y=508; y< 528; y=y+1 )
      {
      for( i=0; i<ni; i++ )
         {
//
// these two statements implement x[i] = y[i] ; x starts at location 0, y starts as indicated.
//
         readwritecache( READ,  y+i, &z, &k, 0 ) ;
         readwritecache( WRITE, x+i, &z, &k, 0 ) ;
         }
      stats( "loc copy" ) ;
//
// add your transposition code here for row wise and column wise with calls to stats for each
//
      }
printf( "---------------------------------\n" ) ;		

   }
//
// add your working set test functions here
//

int main()
   {

   initcache() ;
   locationexample() ;
// 
// add calls to your working set test functions here
//

   return 0 ;
   }
	



