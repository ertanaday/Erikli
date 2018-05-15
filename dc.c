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

   //sstf(arr, i);
   fcfs(arr);
}

//FCFS
void fcfs(struct request arr[]) {
    printf("FCFS DISK SCHEDULING ALGORITHM:\n");
    int i;
    int totalTimeSpent = arr[0].arrive_time;
    int currentPosition = 1;
    for (i = 0; i < sizeof(arr); i++) {
        //printf("%d, ", arr[i]);
      if(arr[i].arrive_time <= totalTimeSpent){
          arr[i].wait_time = totalTimeSpent - arr[i].arrive_time;
          printf("wait_time: %d\n", arr[i].wait_time);
          totalTimeSpent += abs(arr[i].address - currentPosition);
          currentPosition = arr[i].address;
    }
      else{
        i--;
        totalTimeSpent++;
      }

    }
    //printf("%d\n", arr[199]);
    totalTimeSpent += abs(arr[i].address - currentPosition);
    printf("FCFS: %d\n", totalTimeSpent);
}

//SSTF
void sstf( struct request arr[], int size )
{
   int head = 0; //initial disk head position
   int closest_request = NULL;
   int time = arr[0].arrive_time; //initial time
   printf("time = %d\n", time );
   int next_position = NULL;//arr[0].address;
   printf("next_position = %d\n", next_position );

   int k = 0;
   while( k < size)
   {
      int candidate_next_position = next_position;
      bool empty_queue = true;

      //find closest request if available
      int j = 0;
      while( arr[j].arrive_time <= time )//while there is request in queue
      {
        if( arr[j].arrive_time != -1 )
        {
          empty_queue = false;

          candidate_next_position = arr[j].address;
          printf("candidate_next_position = %d\n", candidate_next_position );
          if( abs( candidate_next_position - head ) < abs( next_position - head ) )
          {
             next_position = candidate_next_position;
             closest_request = j;
             printf("closest = %d\n", closest_request );
          }
        }
        j++;
      }
      printf("next_position2 = %d\n", next_position );
      printf("time2 = %d\n", time );
      //update head, time and array
      time += abs( next_position - head );
      head = next_position;
      arr[closest_request].arrive_time = -1;
      k++;
      printf("next_position2 = %d\n", next_position );
      printf("time2 = %d\n", time );
      for(int i = 0; i<10; i++)
      {
         printf("%d %d\n",arr[i].arrive_time, arr[i].address);
      }

      //if no request is available increment time
      if(empty_queue)
      {
         time++;
      }
   }
   printf("result = %d\n", time);
}
