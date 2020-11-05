/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mymemory.h"


// our memory
Byte        mymemory [MAXMEM] ;
Segment_t * segmenttable = NULL;


void initialize ()
{
   printf ( "initialize> start\n");
   // set memory to 0
    int mymemory [MAXMEM];
   // create segment table
   // contains one segment description that declares
   //  the whole memory as one free segment
   //
   // create a single segment descriptor
    segmenttable = (Segment_t *) malloc(sizeof(Segment_t));
   // initialise the segment
    segmenttable->allocated = FALSE;
    segmenttable->size = MAXMEM;
    segmenttable->start = &mymemory[0];

   printf ( "initialize> end\n");
}

//void myfree ( void * ptr )

void * mymalloc ( size_t size )
{
   printf ( "mymalloc> start\n");
   Segment_t* Finder = findFree(segmenttable, size);
   // implement the mymalloc functionality
   if (Finder != NULL){
      size_t NewFreeSize = Finder->size - size;
      Segment_t* NewFree = (Segment_t *)malloc(sizeof(Segment_t));
      Segment_t* OldAlloc = Finder;
      OldAlloc->size = size;
      insertAfter(OldAlloc,NewFree);
      NewFree->size = NewFreeSize;
      for (int i = 0; i < size; i++){
         mymemory[MAXMEM-NewFree->size -size +i] = '\1';
      }
      return Finder->start;
   }
   return ;
}

void myfree ( void * ptr )
{
   printf ( "myfree> start\n");

}

void mydefrag ( void ** ptrlist)
{
   printf ( "mydefrag> start\n");

}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
   printf ( "findFree> start\n");
   for (int i = 0; i<MAXSEGMENTS; ++i) //searching free segment
   {    
      if (list->allocated==FALSE && list->size >= size)
      {
         return list; //found the free segment
      }
      else
      {
         list = list->next; //going for the next segment
         if (list==NULL){
            return NULL;//the next segment does not exist
         }
         else{
            continue; //im not sure if i need this continue
         }             //as it should continue without anyways
      }   
   }
   return NULL;//over max segment, memeory is full or 
               //not enough free space in any segments
}

void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
   oldSegment->next = newSegment;
   oldSegment->allocated = TRUE;
   newSegment->allocated = FALSE;
   newSegment->start = oldSegment->start + oldSegment->size;
}

Segment_t * findSegment ( Segment_t * list, void * ptr )
{
}

int isPrintable ( int c )
{
   if ( c >= 0x20 && c <= 0x7e ) return c ;

   return 0 ;
}

void printmemory ()
{
   int i;
   int j;
   int c;
    for (i=0 ,j=10; i<MAXMEM; j+=10)
    {
        printf("\n[%4d]",j-10);
        for (i,j; i<j && i<MAXMEM; ++i)
        {
            c = isPrintable(mymemory[i]);
            printf(" %02x",mymemory[i]);
            printf("|%s ",(c == 0 ? "." : mymemory[i]));
        }
    }
}

void printsegmenttable()
{
   int i = 0;
   while (i<MAXSEGMENTS){ //nem NULL csak az elsö
      printf("\n");
      printf("\nSegment %d\n", i);
      printsegmentdescriptor(segmenttable);
      segmenttable = segmenttable->next;
      ++i;
      if (segmenttable->next == NULL){
         printf("\n");
         printf("\nSegment %d\n", i);
         printsegmentdescriptor(segmenttable);
         break;
      }
   }
}

void printsegmentdescriptor ( Segment_t * descriptor )
{
      printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) ) ;
      printf ( "\tstart     = %p\n" , descriptor->start ) ;
      printf ( "\tsize      = %lu\n", descriptor->size  ) ;
}