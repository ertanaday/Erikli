#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>

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

   //printf("%d, %s\n", N, filename);

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

   for(int i = 0; i<100; i++)
   {
     if(arr[i].address > N){
       printf("Requests cannot address clinders number above N\n");
       exit(-1);
     }
   }
////////////////////////////////////////////////////////////////////////
  fcfs(arr);

  sstf(arr, i);


}

//FCFS
void fcfs(struct request arr[]) {
    //printf("FCFS DISK SCHEDULING ALGORITHM:\n");
    int i;
    int totalTimeSpent = arr[0].arrive_time;
    int totalWaitTime = 0;
    int currentPosition = 1;
    int requestNum = 0;
    double averageWaitTime;
    double standardDeviation = 0.0;

    for (i = 0; i < sizeof(arr); i++) {
      if(arr[i].arrive_time <= totalTimeSpent){
          arr[i].wait_time = totalTimeSpent - arr[i].arrive_time;
          totalWaitTime += arr[i].wait_time;
          //printf("wait_time: %d\n", arr[i].wait_time);
          totalTimeSpent += abs(arr[i].address - currentPosition);
          currentPosition = arr[i].address;
          requestNum++;
    }
      else{
        i--;
        totalTimeSpent++;
        requestNum++;
      }

    }


        arr[i].wait_time = abs(totalTimeSpent - arr[i].arrive_time);
        totalWaitTime += arr[i].wait_time;
    totalTimeSpent += abs(arr[i].address - currentPosition);

    requestNum++;
    averageWaitTime = (totalWaitTime +0.0) / (requestNum + 0.0);

    int j = 0;
    for(j=0; j < sizeof(arr)+1; j++){
      standardDeviation += pow(arr[j].wait_time - averageWaitTime, 2);
    }


    double calculatedStandardDeviation = sqrt(standardDeviation/(requestNum-1));
    //printf("wait_time: %d\n", arr[i].wait_time);
    //printf("request number: %d\n",requestNum);
    printf("FCFS: %d\t%lf\t%lf\n", totalTimeSpent, averageWaitTime, calculatedStandardDeviation);
}

//SSTF
void sstf( struct request arr[], int size )
{
   int currentPosition = 1; //initial disk head position
   int totalTimeSpent = arr[0].arrive_time; //initial time
   int next_position = 0;//arr[0].address;
   int served_request = 0;
   //printf("totalTimeSpent: %d\n", totalTimeSpent );
   //printf("currentPosition: %d\n", currentPosition );
   //printf("next_position: %d\n", next_position );
   //printf("served_request: %d\n", served_request );
   struct request wait_arr[200];
   double averageWaitTime;
   double standard_dev = 0.0;

   int k = 0;
   while( k < size)
   {
     //printf("next_position: %d\n", next_position );
      int b = find_next( arr, currentPosition, totalTimeSpent, &next_position, &served_request );

      if( b == 1)
      {
        arr[served_request].wait_time = abs(totalTimeSpent - arr[served_request].arrive_time);
        wait_arr[k] = arr[served_request];
        totalTimeSpent += abs( next_position - currentPosition );
        //printf("totalTimeSpent: %d\n", totalTimeSpent );
        currentPosition = next_position;
        //printf("currentPosition: %d\n", currentPosition );
        for(int i = served_request; i < size; i++ ){
          arr[i] = arr[(i+1)];
        }
        //arr[served_request].arrive_time = -1;
        k++;
      }
      else
      {
         totalTimeSpent++;
      }
      //printf("b: %d\n", b );
      //printf("currentPosition: %d\n", currentPosition );
      //printf("totalTimeSpent: %d\n", totalTimeSpent );
      //printf("next_position: %d\n", next_position );
      //printf("served_request: %d\n", served_request );

   }
   //printf("result = %d\n", totalTimeSpent);
   int totalWaitTime;
   for(int i = 0; i < size; i++)
   {
     totalWaitTime += wait_arr[i].wait_time;
      //printf("%d\n", wait_arr[i].wait_time);
   }
   averageWaitTime = (totalWaitTime + 0.0)/ (size + 0.0);

   int w = 0;
   for(w = 0; w < size+1; w++){
     standard_dev += pow(wait_arr[w].wait_time - averageWaitTime, 2);
   }

   double calculatedStandardDeviation = sqrt(standard_dev/(size+1));

   printf("SSTF: %d\t%lf\t%lf\n", totalTimeSpent, averageWaitTime, calculatedStandardDeviation);
}

int find_next(struct request arr[], int currentPosition , int totalTimeSpent,
  int *next_position, int *served_request){

  int found_next = -1;
  int candidate_next_position;
  int j = 0;
  /*for(int i = 0; i<10; i++)
  {
     printf("%d %d\n",arr[i].arrive_time, arr[i].address);
  }*/

  while( arr[j].arrive_time <= totalTimeSpent )//while there is request in queue
  {
    if( arr[j].arrive_time != -1 )
    {
      found_next = 1;
      candidate_next_position = arr[j].address;
      //printf("candidate_next_position here: %d\n", candidate_next_position );
      //printf("next_position here: %d\n", *next_position );
      if( j == 0 ){
        *next_position = candidate_next_position;
        //printf("next_position 0 : %d\n", *next_position );
        *served_request = 0;
      }
      bool b1 = abs( candidate_next_position - currentPosition ) <= abs( *next_position - currentPosition );
      if(b1)
      {
         *next_position = candidate_next_position;
         //printf("next_position j : %d\n", *next_position );
         *served_request = j;
      }
    }
    j++;
  }
  return found_next;
}

//LOOK
void look( struct request arr[], int size )
{
   int currentPosition = 1; //initial disk head position
   int totalTimeSpent = arr[0].arrive_time; //initial time
   int next_position = 0;//arr[0].address;
   int served_request = 0;
   //printf("totalTimeSpent: %d\n", totalTimeSpent );
   //printf("currentPosition: %d\n", currentPosition );
   //printf("next_position: %d\n", next_position );
   //printf("served_request: %d\n", served_request );
   struct request wait_arr[200];
   double averageWaitTime;
   double standard_dev = 0.0;

   int k = 0;
   while( k < size)
   {
     //printf("next_position: %d\n", next_position );
      int b = find_next_look( arr, currentPosition, totalTimeSpent, &next_position, &served_request );

      if( b == 1)
      {
        arr[served_request].wait_time = abs(totalTimeSpent - arr[served_request].arrive_time);
        wait_arr[k] = arr[served_request];
        totalTimeSpent += abs( next_position - currentPosition );
        //printf("totalTimeSpent: %d\n", totalTimeSpent );
        currentPosition = next_position;
        //printf("currentPosition: %d\n", currentPosition );
        for(int i = served_request; i < size; i++ ){
          arr[i] = arr[(i+1)];
        }
        //arr[served_request].arrive_time = -1;
        k++;
      }
      else
      {
         totalTimeSpent++;
      }
      //printf("b: %d\n", b );
      //printf("currentPosition: %d\n", currentPosition );
      //printf("totalTimeSpent: %d\n", totalTimeSpent );
      //printf("next_position: %d\n", next_position );
      //printf("served_request: %d\n", served_request );

   }
   //printf("result = %d\n", totalTimeSpent);
   int totalWaitTime;
   for(int i = 0; i < size; i++)
   {
     totalWaitTime += wait_arr[i].wait_time;
      //printf("%d\n", wait_arr[i].wait_time);
   }
   averageWaitTime = (totalWaitTime + 0.0)/ (size + 0.0);

   int w = 0;
   for(w = 0; w < size+1; w++){
     standard_dev += pow(wait_arr[w].wait_time - averageWaitTime, 2);
   }

   double calculatedStandardDeviation = sqrt(standard_dev/(size+1));

   printf("SSTF: %d\t%lf\t%lf\n", totalTimeSpent, averageWaitTime, calculatedStandardDeviation);
}

int find_next_look(struct request arr[], int currentPosition , int totalTimeSpent,
  int *next_position, int *served_request){

  int found_next = -1;
  int candidate_next_position;
  int j = 0;
  /*for(int i = 0; i<10; i++)
  {
     printf("%d %d\n",arr[i].arrive_time, arr[i].address);
  }*/

  while( arr[j].arrive_time <= totalTimeSpent )//while there is request in queue
  {
    if( arr[j].arrive_time != -1 )
    {
      found_next = 1;
      candidate_next_position = arr[j].address;
      //printf("candidate_next_position here: %d\n", candidate_next_position );
      //printf("next_position here: %d\n", *next_position );
      if( j == 0 ){
        *next_position = candidate_next_position;
        //printf("next_position 0 : %d\n", *next_position );
        *served_request = 0;
      }
      bool b1 = abs( candidate_next_position - currentPosition ) <= abs( *next_position - currentPosition );
      if(b1)
      {
         *next_position = candidate_next_position;
         //printf("next_position j : %d\n", *next_position );
         *served_request = j;
      }
    }
    j++;
  }
  return found_next;
}
