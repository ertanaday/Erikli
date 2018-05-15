#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

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
   int head = 0; //initial disk head position

   //FCFS
   
   int total_time = 0;

   int k = 0;
   while( i > k){
      total_time = total_time + arr[k].arrive_time;
      k++;
   }
   printf("%d\n", total_time);




}
