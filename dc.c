#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>

struct request
{
   int arrive_time, address, wait_time;
};

int main (int argc, char *argv[])
{
   int N; //number of cylinders
   char str1[4];
   char filename[100];

   strcpy( str1, "");
   if( argv[1] != NULL) strcat( str1, argv[1] );
   N = atoi(str1);

   strcpy( filename, "");
   if( argv[2] != NULL) strcat( filename, argv[2] );

   printf("%d, %s\n", N, filename);

   FILE* fp = fopen(filename, "r");

   if(fp == NULL)
   {
      printf("fopen failed to open the file\n");
      exit(-1);
   }

   char line[2048];
   struct request arr[200]; //maximum request number
   for(int i = 0; i < 200; i++){
      arr[i].arrive_time = -1;
      arr[i].address = -1;
      arr[i].wait_time = -1;
   }
   int i = 0; //number of requests

   //reads file and makes the queue
   while( fgets( line, sizeof(line), fp) != NULL)
   {
      struct request r;

      if( sscanf( line, "%d %d", &r.arrive_time, &r.address) != 2)
      {
         printf("Bad line deetected\n");
         exit(-1);
      }
      arr[i] = r;
      i++;
   }

   //shows queue
   for(int i = 0; i<10; i++)
   {
      printf("%d %d\n",arr[i].arrive_time, arr[i].address);
   }
////////////////////////////////////////////////////////////////////////
   
   sstf(arr, i);
}

void sstf( struct request arr[], int i )
{
   int head = 0; //initial disk head position
   int closest_request = 0;
   int time = arr[0].arrive_time; //initial time
   int next_position = arr[0].address;
   
   //SSTF

   int k = 0;
   while( i > k){

      int j = 0;
      int candidate_next_position;
      bool empty_queue = true;

      //find closest request if available
      while( arr[j].arrive_time != -1 && arr[j].arrive_time <= time)//while there is request in queue
      {
         empty_queue = false;
         candidate_next_position = arr[j].address;
         if( abs( candidate_next_position - head ) < abs( next_position - head ) )
         {
            next_position = candidate_next_position;
            closest_request = j;
         }
         j++;
      }

      //update head, time and array
      time += abs( next_position - head );
      head = next_position;
      arr[closest_request].arrive_time = -1;

      //if no request is available increment time
      if(empty_queue)
      {
         time++;
      }
      

      k++;
   }
   printf("%d\n", time);
}



