#include <stdio.h>
#include <stdlib.h>

#define HEAPSIZE 32768

#define hdr_payload(i)  ((struct HDR *) &heap[i])->payload
#define hdr_freeall(i)  ((struct HDR *) &heap[i])->freeall
#define hdr_succesr(i)  ((struct HDR *) &heap[i])->succesr
#define hdr_previus(i)  ((struct HDR *) &heap[i])->previus

char *heap ;
int  heapsize ;

int sizehdr ;

struct HDR       // free block header/footer/linked list
   {
   int  payload ;    // size of block (including headers)
   char freeall ;    // is this block allocated? 0=free/1=allocated
   int  succesr ;    // next free block ( heapsize if last free block )
   int  previus ;    // previous free block ( -1 if none )
   } anchor ;

struct HDR anc ;

void dump_heap( char *t )
   {
   int i ;

   int j=0 ;
   printf( "%s anchor= %d\n", t, anchor.succesr ) ;
   for( i=0; i<heapsize; i=i+sizehdr  )
      if ( hdr_payload(i) != 0 )
         {
         printf( "%6d %6d %6d %6d %6d\n", i, hdr_payload(i), hdr_freeall(i), 
                                         hdr_succesr(i), hdr_previus(i) ) ;
         if ( j++%2==1 ) printf( "\n" ) ;
         }
   }

void dump_free( char *t )
   {
   int i ;

   printf( "%s anchor= %d\n", t, anchor.succesr ) ;
   for( i = anchor.succesr ; i < heapsize ; i = hdr_succesr(i)  )
         {
         printf( "%6d %6d %6d %6d %6d\n", i, hdr_payload(i), hdr_freeall(i), 
                                         hdr_succesr(i), hdr_previus(i) ) ;
         }
   }

void init_heap()
   {
   heap = (char *) calloc( HEAPSIZE, 1 ) ;
   sizehdr =  sizeof(anchor) ;
   heapsize = HEAPSIZE ;

   anchor.payload = 0 ;
   anchor.freeall = 1 ;
   anchor.succesr = 0 ;

   hdr_payload(0) = heapsize-2*sizehdr ;   
   hdr_freeall(0) = 0 ;
   hdr_succesr(0) = heapsize ;
   hdr_previus(0) = -1 ;
   hdr_payload (heapsize-sizehdr) = heapsize-2*sizehdr ;
   hdr_freeall(heapsize-sizehdr) = 0 ;
   }

int lalloc( int amt )
   {
   int i,j,blocksize,free;
   j = anchor.succesr;
   while (j != heapsize) {
      blocksize = hdr_payload(j);
      free = hdr_freeall(j);
      if (blocksize >= amt && free == 0) {
         break;
      }
      j = hdr_succesr(j);
   }
   
   int reqsize,newpayload;
   if (amt % 16 != 0) {
      reqsize = amt/16 + 1;
      reqsize *= 16;
   } else {
      reqsize = amt;
   }
   reqsize += 2*sizehdr;
   newpayload = reqsize-2*sizehdr;

   if (j == heapsize) {
      heap = realloc (heap, reqsize+heapsize);
      //zeroing out added space;
      for (i=0;i<newpayload;i++) {
         heap[heapsize+i+1] = 0; 
      }
      heapsize += reqsize;
      //hdr_succesr(j) does not exist
   } 

   // j points to first fit block
   int footer = j + reqsize - sizehdr;
   // Setting header and footer of allocated block
   hdr_freeall(j) = 1;
   hdr_freeall(footer) = 1;
   hdr_payload(j) = newpayload;
   hdr_payload(footer) = newpayload;
   // Free List
   int prev = hdr_previus(j);
   if (prev == -1) {
      anchor.succesr = j + reqsize;
   } else {
      hdr_succesr(prev) = j + reqsize;;         
   }
   // Requested Space != Free Space
   if (blocksize > newpayload) {
      //Set header of new space
      int head = j + reqsize;
      int footer = j + blocksize + sizehdr;
      hdr_payload(head) = blocksize - reqsize;
      hdr_payload(footer) = blocksize - reqsize;
      hdr_freeall(head) = 0;
      hdr_freeall(footer) = 0;
      hdr_previus(head) = prev;
      hdr_succesr(head) = hdr_succesr(j);
   }

   return j+sizehdr;

   }

void lfree( int *aa ) 
   {
      int head = *aa;
      int payload = hdr_payload(head);
      int footer = head + sizehdr + payload;
      hdr_freeall(head) = 0;
      hdr_freeall(footer) = 0;    

      int prev = head - sizehdr;
      int next = footer + sizehdr;
      int existsFree = 0;

      // Free block adjacent to the left
      if (hdr_freeall(prev) == 0) {
         //merge left
         head = head - hdr_payload(prev) - 2*sizehdr;
         hdr_payload(head) += 2*sizehdr + payload;
         hdr_payload(footer) = hdr_payload(head);
         existsFree = 1;
      }
      if (hdr_freeall(next) == 0) {
         int tail = next + hdr_payload(next) + sizehdr;
         hdr_payload(head) += 2*sizehdr + hdr_payload(next);
         hdr_payload(tail) = hdr_payload(head);

         prev = head - sizehdr;
         int previus = hdr_previus(next);
         int succesr = hdr_succesr(next);
         if (existsFree) {
            // If already merged left
            // Removes right block from Free List
            hdr_succesr(previus) = succesr;
            hdr_previus(succesr) = previus;
         } else {
            // Else haven't merged left
            // Expands left block to include newly freed space
            hdr_succesr(previus) = head;            
            hdr_succesr(head) = succesr;
            hdr_previus(succesr) = head;
            existsFree = 1;
         }
      }
      // No adjacent free blocks
      if (existsFree == 0) {
         hdr_succesr(head) = anchor.succesr;
         anchor.succesr = head;   
         hdr_previus(head) = -1;
      }
   }

int main()
   {
   int i ;
   int j = 0  ;
   int blks[100] ;

   init_heap() ;
   for( i=0; i<20; i++ )
      blks[j++] = lalloc( rand()%500 ) ;

   dump_heap( "after alloc" ) ;

   lfree( &blks[10] ) ;
   lfree( &blks[11] ) ;

   dump_heap( "coalesce with upper" ) ;

   lfree( &blks[14] ) ;
   lfree( &blks[13] ) ;

   dump_heap( "coalesce with lower" ) ;

   lfree( &blks[5] ) ;
   lfree( &blks[7] ) ;
   lfree( &blks[6] ) ;

   dump_heap( "coalesce with both" ) ;

   for( i=0; i<20; i++ )
      if (blks[i] != 0 )
         {
         lfree( &blks[i] ) ;
         }

   dump_heap( "free everything " ) ;

   blks[0] = lalloc( 40000 ) ;

   dump_heap( "blew the top off" ) ;

   return 0 ;
   }
