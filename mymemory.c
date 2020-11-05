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
   Segment_t* toAlloc = findFree(segmenttable, size);

   // implement the mymalloc functionality
   if (toAlloc != NULL){
      Segment_t* FirstSeg = (Segment_t*) malloc(sizeof(Segment_t));
   //creating new segment
   //insert the segment to the list and set values
      size_t LeftSize = toAlloc->size - size;
      FirstSeg->start = toAlloc->start + size;
      FirstSeg->size = LeftSize;
      toAlloc->size = size;
      insertAfter(toAlloc,FirstSeg);
      for (int i = 0; i < size; i++){
         mymemory[MAXMEM - LeftSize - size+i] = '\1';
      }
      return toAlloc->start;
   }
}
void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
   oldSegment ->next = newSegment;
   oldSegment->allocated = TRUE;
   newSegment->allocated = FALSE;

}

void myfree ( void * ptr )
{
   printf ( "myfree> start\n");
   int starter = 0;
   Segment_t* result = findSegment(segmenttable, ptr);
   //result is the pointer to the segment we want to free up
   if (result != NULL){
      result->allocated = FALSE;
      result->next = segmenttable->next;
      //setting values
      for (int i = 0; i < MAXMEM; i++)
         {
         if (result->start == &mymemory[i])
         {
            starter = i;
         }
         }
      for (int i = 0; i < result->size ; ++i){
         mymemory[i + starter] = '\0';
      }
   }
   else
   {
      printf("Not found");
   }
   
}

void mydefrag ( void ** ptrlist)
{
   printf ( "mydefrag> start\n");

}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
   printf ( "findFree> start\n");
   for (int i = 0; i<MAXSEGMENTS; ++i) //searching for free segment
   {    
      if (list->allocated==FALSE && list->size >= size)
      {
         return list; //found the free segment
      }
      else if(list==NULL)
      {
         return NULL;//the next segment does not exist
      } 
      list = list->next; //going for the next segment
   }
   return NULL;//over max segment, memeory is full or 
               //not enough free space in any segments
}


Segment_t * findSegment(Segment_t * list, void * ptr )
{  //had to declare this function in mymemory.h
   for (int i=0; i < MAXSEGMENTS ; i++){
	if ( list->start == ptr){ //correct segment
		return list;
	   }	
   else if (list->next == NULL){//no more segments to check
            printf("Could not find match\n");
      return NULL;
      }
      list = list->next; //going for the next segment
   }
   return NULL; //segment does not exist
}

int isPrintable ( int c )
{
   if ( c >= 0x20 && c <= 0x7e ) return c ;

   return 0 ;
}

void printmemory () //prints the whole memory
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

void printsegmenttable()//prints the whole segmentation table
{
   int i = 0;
   while (i<MAXSEGMENTS){
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
      printf ( "\tnext      = %p\n", descriptor->next );

}