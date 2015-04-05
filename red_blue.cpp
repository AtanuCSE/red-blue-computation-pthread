#include "stdafx.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>

using namespace std;

#define white 0
#define red 1
#define blue 2

int grid[15000][15000]; 	/* grid[row][col] */
int grid2[15000][15000]; 	/* grid[row][col] */
bool finished = false;
int n_itrs = 0;
int redcount, bluecount;
int n,t,c,MAX_ITRS;
int num_of_thread;
int converged=0;

void* RedGridMove(void* arg);
void* BlueGridMove(void* arg);
void check_for_single_thread();

void insert_random_position()   
{
	/*Randomly fill grid with 1/3 red, 1/3 blue, 1/3 white*/
    
	long long total_board_piece=n*n;
	int arr[30000];
	for(int i = 0; i < 30000; ++i)
		arr[i] = i;

	for (int i =0; i < total_board_piece; ++i)
	{
	   int j = rand()%(total_board_piece-i); // Pick random number from 0 <= r < n-i.  Pick favorite method
	   // j == 0 means don't swap, otherwise swap with the element j away
	   if (j != 0) { 
		  std::swap(arr[i], arr[i+j]);
	   }
	}

	for(int p=0;p<=total_board_piece;p++)
	{
		int target=0;
		int percent=total_board_piece/3;
		if(p>=0 && p<percent)
		{
			target=red;
		}
		else if(p>=percent && p<percent*2)
		{
			target=blue;
		}
		else if(p>percent*2+1 && p<total_board_piece)
		{
			target=white;
		}

		int target_row=arr[p]/n;
		int target_col=arr[p]%n;
		grid[target_row][target_col]=target;
		grid2[target_row][target_col]=target;

	}

}

struct thread_variable
{
    pthread_t thread_id;
	int no_of_box_size;
    int row_lower;
    int row_upper;
    int col_lower;
    int col_upper;
};


int _tmain(int argc, _TCHAR* argv[])
{

	printf("Enter the number of thread = ");
	scanf("%d",&num_of_thread);
	printf("\n");

	printf("Enter the Grid size = ");
	scanf("%d",&n);
	printf("\n");

	printf("Enter the Tile size = ");
	scanf("%d",&t);
	printf("\n");

	printf("Enter the terminating thresold = ");
	scanf("%d",&c);
	printf("\n");

	printf("Enter the MAX iteration size = ");
	scanf("%d",&MAX_ITRS);
	printf("\n");

	insert_random_position();  //Fill the grid

	if(num_of_thread==1)   //1 thread scenario
	{
		clock_t begin, end;  //calculate execution time
		double time_spent;
		begin = clock();

		while (!finished && n_itrs < MAX_ITRS)
		{
		   n_itrs++; 
		   /* red color movement */
		   for (int i = 0; i < n; i++)
		   {
				if (grid[i][0] == 1 && grid[i][1] == 0)
				{
					grid[i][0] = 4;
					grid[i][1] = 3;
				}
				for (int j = 1; j < n; j++)
				{
					if (grid[i][j] == 1 && grid[i][(j+1)%n] == 0)
					{
						grid[i][j] = 0;
						grid[i][(j+1)%n] = 3;
					}
					else if (grid[i][j] == 3)
						grid[i][j] = 1;
				}
				if (grid[i][0] == 3)
					grid[i][0] = 1;
				else if (grid[i][0] == 4)
					grid[i][0] = 0;
			}

		   /* blue color movement */
		   for (int j = 0; j < n; j++)
		   {
				if (grid[0][j] == 1 && grid[1][j] == 0)
				{
					grid[0][j] = 4;
					grid[1][j] = 3;
				}
				for (int i = 1; i < n; i++)
				{
					if (grid[i][j] == 2 && grid[(i+1)%n][j]==0)
					{
						grid[i][j] = 0;
						grid[(i+1)%n][j] = 3;
					}
					else if (grid[i][j] == 3)
						grid[i][j] = 2;
				}
				if (grid[0][j] == 3)
					grid[0][j] = 2;
 				else if (grid[0][j] == 4)
					grid[0][j] = 0;
			}

		   /* count the number of red and blue in each tile and check if the computation can be terminated*/

		    int tilespart = n / t;
			redcount=0;
			bluecount=0;

			int tilesize = t * t;

			for (int p = 0; p < tilespart; p++)
			{
				for (int q = 0; q < tilespart; q++)
				{
					redcount = 0;
					bluecount = 0;
					for (int i = t * p; i < t * (p + 1); i++)
					{
						for (int j = t * q; j < t * (q + 1); j++)
						{
							if (grid[i][j] == 2) bluecount++;
							else if (grid[i][j] == 1) redcount++;
						}
					}
                    
                              //Print data to see different steps
					printf("iteration=%d \n",n_itrs);
					printf("Tiles=%d \n",p*tilespart+q);
					printf("Red count = %d\n",redcount);
					printf("Blue count = %d\n",bluecount);
					printf("Tiles size = %d\n",tilesize);
					printf("Red Percentage = %lf\n",(((double) redcount) / tilesize)*100);
					printf("Blue Percentage = %lf\n\n\n",(((double) bluecount) / tilesize)*100);


					if ( (((double) redcount) / tilesize)*100 >= c) 
					{
						converged = 1; //Red converge
						finished=true;
						break;
					}
					if ( (((double) bluecount) / tilesize)*100 >= c)
					{
						converged = 2; //Blue converge
						finished=true;
						break;
					}

				}

				if(finished) break;
			}

			if(finished)
			{
				printf("Result for serial execution: \n\n");
				printf("Iteration = %d\n\n",n_itrs);
				if(converged==1)
					printf("Converged = RED\n\n");
				else
					printf("Converged = Blue\n\n");

			}
		}

		if(!finished)
		{
			printf("Result for serial execution: \n\n");
			printf("Not Converged. Maximum iteration exceeded.\n\n");
		}

		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Time spent=%lf\n",time_spent);

		FILE *fp;
		fp = freopen("result.txt", "a+",stdout);
		printf("Time spent=%lf\n",time_spent);
		fclose(fp);
	}
	else                   /* multiple thread */
	{
		check_for_single_thread(); //check for serial processing for comparison

		struct thread_variable *thread_instance;
		thread_instance = (thread_variable*) calloc(num_of_thread, sizeof (struct thread_variable));
		int thread_divider= n / (num_of_thread-1);

		for(int i=0;i<num_of_thread;i++)  //initialize thread divider
		{
			int start=1;
			if(i*thread_divider<n-1)
			{
				thread_instance[i].row_lower=i*thread_divider;
			}
			else
			{
				thread_instance[i].row_lower=-1;
				thread_instance[i].row_upper=-1;
				start=0;
			}
			if(i*thread_divider<n-1 && start==1)
			{
				thread_instance[i].row_upper=i*thread_divider+thread_divider;
			}

			start=1;
			if(i*thread_divider<n-1)
			{
				thread_instance[i].col_lower=i*thread_divider;
			}
			else
			{
				thread_instance[i].col_lower=-1;
				thread_instance[i].col_upper=-1;
				start=0;
			}
			if(i*thread_divider<n-1 && start==1)
			{
				thread_instance[i].col_upper=i*thread_divider+thread_divider;
			}
			

		}

		n_itrs=0;
		converged=0;

		clock_t begin, end;  //calculate execution time
		double time_spent;
		begin = clock();

		while (!finished && n_itrs < MAX_ITRS)
		{
			n_itrs++;
			
			//Red iteration call
			for(int i=0;i<num_of_thread;i++)
			{
				int rc = pthread_create(&thread_instance[i].thread_id, NULL, RedGridMove, &thread_instance[i]);
				if (rc)
				{
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
				}

			}

			for(int i=0;i<num_of_thread;i++)  //wait for all thread result
			{
				pthread_join(thread_instance[i].thread_id, NULL);
			}


			//Blue iteration call
			for(int i=0;i<num_of_thread;i++)
			{
				int rc = pthread_create(&thread_instance[i].thread_id, NULL, BlueGridMove, &thread_instance[i]);
				if (rc)
				{
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
				}

			}

			for(int i=0;i<num_of_thread;i++) //wait for all thread result
			{
				pthread_join(thread_instance[i].thread_id, NULL);
			}

			/* count the number of red and blue in each tile and check if the computation can be terminated*/

		    int tilespart = n / t;
               /* number of tiles=grid size divided by tiles size */
      
			redcount=0;
			bluecount=0;
			int tilesize = t * t;

			for (int p = 0; p < tilespart; p++)
			{
				for (int q = 0; q < tilespart; q++)
				{
					redcount = 0;
					bluecount = 0;
					for (int i = t * p; i < t * (p + 1); i++)
					{
						for (int j = t * q; j < t * (q + 1); j++)
						{
							if (grid[i][j] == 2) bluecount++;
							else if (grid[i][j] == 1) redcount++;
						}
					}
                    
                    //Print data for different steps
					printf("iteration=%d \n",n_itrs);
					printf("Tiles=%d \n",p*tilespart+q);
					printf("Red count = %d\n",redcount);
					printf("Blue count = %d\n",bluecount);
					printf("Tiles size = %d\n",tilesize);
					printf("Red Percentage = %lf\n",(((double) redcount) / tilesize)*100);
					printf("Blue Percentage = %lf\n\n\n",(((double) bluecount) / tilesize)*100);


					if ( (((double) redcount) / tilesize)*100 >= c) 
					{
						converged = 1; //Red converge
						finished=true;
						break;
					}
					if ( (((double) bluecount) / tilesize)*100 >= c)
					{
						converged = 2; //Blue converge
						finished=true;
						break;
					}

				}

				if(finished) break;
			}

			if(finished)
			{
				printf("Result for parallel execution: \n\n");
				printf("Iteration = %d\n\n",n_itrs);
				if(converged==1)
					printf("Converged = RED\n\n");
				else
					printf("Converged = Blue\n\n");

			}
		}

		if(!finished)
		{
			printf("Result for parallel execution: \n");
			printf("Not Converged. Maximum iteration exceeded.\n\n");

		}

		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Time spent=%lf begin = %lf end = %lf\n",time_spent,(double)begin,(double)end);

	}

	getchar();
	getchar();
	
	return 0;
}

void* RedGridMove(void* arg) {
	struct thread_variable* t_instance = (struct thread_variable *) arg;
	int rl=t_instance->row_lower;
	int ru=t_instance->row_upper;

	for (int i = rl; i < ru; i++)  //inside thread boundary
	{
		if (grid[i][0] == 1 && grid[i][1] == 0)
		{
			grid[i][0] = 4;
			grid[i][1] = 3;
		}
		for (int j = 1; j < n; j++)
		{
			if (grid[i][j] == 1 && grid[i][(j+1)%n] == 0)
			{
				grid[i][j] = 0;
				grid[i][(j+1)%n] = 3;
			}
			else if (grid[i][j] == 3)
				grid[i][j] = 1;
		}
		if (grid[i][0] == 3)
			grid[i][0] = 1;
		else if (grid[i][0] == 4)
			grid[i][0] = 0;
	}

	return 0;
}


void* BlueGridMove(void* arg) {
	struct thread_variable* t_instance = (struct thread_variable *) arg;
	int cl=t_instance->col_lower;
	int cu=t_instance->col_upper;

	for (int j = cl; j < cu; j++)    //inside thread boundary
	{
		if (grid[0][j] == 1 && grid[1][j] == 0)
		{
			grid[0][j] = 4;
			grid[1][j] = 3;
		}
		for (int i = 1; i < n; i++)
		{
			if (grid[i][j] == 2 && grid[(i+1)%n][j]==0)
			{
				grid[i][j] = 0;
				grid[(i+1)%n][j] = 3;
			}
			else if (grid[i][j] == 3)
				grid[i][j] = 2;
		}
		if (grid[0][j] == 3)
			grid[0][j] = 2;
 		else if (grid[0][j] == 4)
			grid[0][j] = 0;
	}

	return 0;
}

void check_for_single_thread()  //result for serial calculation
{
		bool finished2=false;
		clock_t begin, end;  //calculate execution time
		double time_spent;
		begin = clock();
		int n_itrs2=0;

		while (!finished2 && n_itrs2 < MAX_ITRS)
		{
		   n_itrs2++; 

		   /* red color movement */
		   for (int i = 0; i < n; i++)
		   {
				if (grid2[i][0] == 1 && grid2[i][1] == 0)
				{
					grid2[i][0] = 4;
					grid2[i][1] = 3;
				}
				for (int j = 1; j < n; j++)
				{
					if (grid2[i][j] == 1 && grid2[i][(j+1)%n] == 0)
					{
						grid2[i][j] = 0;
						grid2[i][(j+1)%n] = 3;
					}
					else if (grid2[i][j] == 3)
						grid2[i][j] = 1;
				}
				if (grid2[i][0] == 3)
					grid2[i][0] = 1;
				else if (grid2[i][0] == 4)
					grid2[i][0] = 0;
			}

		   /* blue color movement */
		   for (int j = 0; j < n; j++)
		   {
				if (grid2[0][j] == 1 && grid2[1][j] == 0)
				{
					grid2[0][j] = 4;
					grid2[1][j] = 3;
				}
				for (int i = 1; i < n; i++)
				{
					if (grid2[i][j] == 2 && grid2[(i+1)%n][j]==0)
					{
						grid2[i][j] = 0;
						grid2[(i+1)%n][j] = 3;
					}
					else if (grid2[i][j] == 3)
						grid2[i][j] = 2;
				}
				if (grid2[0][j] == 3)
					grid2[0][j] = 2;
 				else if (grid2[0][j] == 4)
					grid2[0][j] = 0;
			}

		   /* count the number of red and blue in each tile and check if the computation can be terminated*/

		    int tilespart = n / t;
               /* number of tiles=grid size divided by tiles size */
			redcount=0;
			bluecount=0;
			int tilesize = t * t;

			for (int p = 0; p < tilespart; p++)
			{
				for (int q = 0; q < tilespart; q++)
				{
					redcount = 0;
					bluecount = 0;
					for (int i = t * p; i < t * (p + 1); i++)
					{
						for (int j = t * q; j < t * (q + 1); j++)
						{
							if (grid2[i][j] == 2) bluecount++;
							else if (grid2[i][j] == 1) redcount++;
						}
					}

                              //Print data for different step
					printf("iteration=%d \n",n_itrs);
					printf("Tiles=%d \n",p*tilespart+q);
					printf("Red count = %d\n",redcount);
					printf("Blue count = %d\n",bluecount);
					printf("Tiles size = %d\n",tilesize);
					printf("Red Percentage = %lf\n",(((double) redcount) / tilesize)*100);
					printf("Blue Percentage = %lf\n\n\n",(((double) bluecount) / tilesize)*100);



					if ( (((double) redcount) / tilesize)*100 >= c) 
					{
						converged = 1; //Red converge
						finished2=true;
						break;
					}
					if ( (((double) bluecount) / tilesize)*100 >= c)
					{
						converged = 2; //Blue converge
						finished2=true;
						break;
					}

					
				}

				if(finished2) break;
			}

			
			if(finished2)
			{
				printf("Result for serial execution: \n\n");

				printf("Iteration = %d\n",n_itrs2);
				if(converged==1)
					printf("Converged = RED\n\n");
				else
					printf("Converged = Blue\n\n");

			}
		}

		if(!finished2)
		{
			printf("Result for serial execution: \n\n");
			printf("Not Converged. Maximum iteration exceeded.\n\n");

		}

		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Time spent=%lf begin = %lf end = %lf\n",time_spent,(double)begin,(double)end);

}
