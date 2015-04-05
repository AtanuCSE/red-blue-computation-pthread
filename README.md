A solution for infamous red blue computation problem using Pthread

#Problem Definition:
The Red/Blue computation simulates two interactive flows: an n by n board is initialized so cells have one of three colours: red, white, and blue, where white is empty, red moves right, and blue moves down. (The board may be initialized with 1/3 cells in read, 1/3 in white and 1/3 in blue and colours should be interleaved and spread across the board)
Colours wraparound to the opposite side when reaching the edge. In the first half step of an iteration, any red colour can move right one cell if the cell to the right is unoccupied (white); on the second half step, any blue colour can move down one cell if the cell below it is unoccupied; the case where red vacates a cell (first half) and blue moves into it (second half) is okay. Viewing the board as overlaid with t by t tiles (t divides n), the computation terminates if any tile’s coloured squares are more than c% one colour (blue or red).
Two-Stage Computation:
1.	Red squares may move to the right into an empty (white) square
2.	Blue squares may move down into an empty (white) square

<a href="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swenNzSGF1VjgzZXc"><img src="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swenNzSGF1VjgzZXc" style="max-width: 100%; height: auto" title="Click for the larger version." /></a>


Convergence:
1.	Split the grid into NxN tiles
2.	 Compute % for each color in a given tile
3.	If that % reaches a threshold, we're done, otherwise, keep stepping

<a href="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swZTlwVDZWR1U0ZDQ"><img src="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swZTlwVDZWR1U0ZDQ" style="max-width: 100%; height: auto" title="Click for the larger version." /></a>

	Use of Pthreads to write a solution to the Red/Blue computation.
	Implementing a simple Barrier and using it to synchronize the computation between consecutive iteration steps.
	It is fine to use a square grid, i.e. n by n grid for n being grid size.
	Program needs to take 5 user defined parameters (integers) as inputs: grid size n, tile size t, terminating threshold c, maximum number of iterations max_iters, and the number of threads nthrds.
	Program needs to store the grid after each iteration in a file and print out which tile (or tiles if more than one) has the coloured squares more than c% one colour (blue or red).

#Requirement:
•	pthread library, dll, header
•	Language: C++
•	Microsoft Visual Studio 2010
•	Windows Command Prompt
•	Windows 7 environment

#Parallel Algorithm design:
We’ve used pthread.h to create and join threads. In our design if only one input is given, then it will be trivial case and simply serialized calculation will be done. If there are more than 1 thread than parallelism will be applied by creating several threads. First of all we took few inputs such as grid size (n), then total grid will be n*n, tile size t, total tile size for each tile will be t*t, max number of iterations, convergence amount in percentage (c%). Then we randomly assign 1/3rd cells in 0(white), 1/3rd cells in 1(red), 1/3rd cells in 2(blue) to n*n grid.  Next thread barrier is created. We divide the n*n grid into small groups and assign them to each thread parameter. After that we create threads and send those parameters with thread id to methods which are handling computations. We send few rows to each thread for red move computation. After that we join all the thread and barrier is being taken out. Again we send few columns to each thread and join them at the end of the blue move computation. After a successive iteration with both red & blue move, convergence result is being checked. If it converge loop ends, otherwise loop continues until convergence is encountered or iteration exceeds maximum number of iteration. In the mean time we calculate total execution time.

<a href="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swUkV1bUxBMm1mSkE"><img src="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swUkV1bUxBMm1mSkE" style="max-width: 100%; height: auto" title="Click for the larger version." /></a>

#Implementation:
We’ve configured the pthread library into the Visual Studios 2010. For this we’ve needed to put the pthread header files in the “VC\include” directory inside visual studio 2010 installation directory. So as the pthread dlls and libraries in the respective ‘bin’ and ‘lib’ folders. Now visual studio is set for pthread compilation. Then we code for the program and run. Inside code pthread_create is used for thread creation. And to wait for completion of one step so that one thread doesn’t affect the other step we’ve used pthread.join . It ensures the completion of a step before next move began and convergence checking occurred. Thread barrier is created using lower row and upper row limit for RED move. Also with lower column and upper column limit for BLUE move. This barrier ensures computation limit within grid for each thread. We’ve checked both serial and parallel execution for comparison. Compared time is shown in the output.

Following is a graphical example of how moves are divided into threads.

<a href="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swUnh3R2VydXN2dUE"><img src="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swUnh3R2VydXN2dUE" style="max-width: 100%; height: auto" title="Click for the larger version." /></a>
<a href="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swTlhTSVhfN0hSeGc"><img src="https://drive.google.com/uc?export=view&id=0B7A_IjGiD0swTlhTSVhfN0hSeGc" style="max-width: 100%; height: auto" title="Click for the larger version." /></a>

#Testing:

We have tested the program with several input combinations. We applied same grid set for both serial and parallel process and compared the result. We found that for small input sometimes parallelism overhead is high. But for big input parallelism is efficient. Here is a sample result for 16 threads, grid size=128*128, tile size=8*8, c=65%, maximum iteration=100

Result for serial execution: 
Iteration = 23
Converged = RED
Time spent=10.931000 sec
begin = 18150.000000 end = 29081.000000

Result for parallel execution: 
Iteration = 23
Converged = RED
Time spent=1.306000 sec
begin = 29082.000000 end = 30388.000000

Final step example:
Tiles no=63
Red count = 42
Blue Count = 9
Tiles Size = 64
Red percentage = 65.625000 %
Blue percentage = 14.062500 %

#Manual (how to run the program, input and output)
1.	Extract the zip file (e.g. assignment.zip)
2.	The extracted folder will be a complete visual studio 2010 project folder.
3.	Build the project
4.	Run the program in visual studio 2010
5.	Enter the required inputs. (no. of threads, grid size, tile size, max no. of iteration, convergence threshold)
6.	Get output in the command prompt.
7.	Data from each steps of computation, comparison between serial and parallel process will be shown.

#Reference
1.	http://web.cs.du.edu/~sturtevant/pthread.html (Configuring visual studio 2010)
2.	https://sourceware.org/pthreads-win32/



