/*
Author:Kevin Hinson
Program: A Conway's Game of life program
*/
#include<iostream>
#include<cstdlib>
#include<ctime>    //seeding the random number generator
#include<unistd.h> //sleep
using namespace std;
//global variables
const int row = 20, col = 20;
enum status {alive = 1, dead = 0,nil = -1};//1 for alive, 0 for dead, nil for when we do nothing to a cell

struct cell{
	
	status state;//Wether or not the cell is alive or dead
	status next; //will keep track of what the cell will be next. Will be either -1(for when we do nothing to a cell), 0(when the cell dies), and 1(when the cell lives)
};
//prototypes
void initialize(cell conway[][col],int row);//initialize state to dead and next to nil
void print(cell conway[][col],int row);
void seed(cell conway[][col],int row,int rng);
void iter(cell conway[][col],int row);
void ngen(cell conway[][col],int row);
bool toperr(int r, int c, int s);
bool bottomerr(int r, int c, int s);
//--------------------------------------------------------------------MAIN FUNCTION-----------------------------------------------------------------------------------------------------
int main(){
//variables
cell conway[row][col];//status conway[row][col];
int pop = 0;//this is the population
int rng = 0;//random number generator
int gen = 0;//this will be the number of generations

	cout << "Enter number(an integer) of generations >> ";
	 cin >> gen;
	 
initialize(conway,row);
seed(conway,row,rng);

 for(int i = 0; i < gen; i++)//this will cycles through the generations of cells
 {
	print(conway,row);
	iter(conway,row);
	ngen(conway,row);
	sleep(1);
 }


return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------function definitions--------------------------------------------------------------------------------------------------
//Function description: This initializes all cells to dead and next to nil
void initialize(cell conway[][col],int row)
{
	for(int i = 0; i < row; i++)//cycles through rows
	{
		for(int j = 0; j < col; j++)//cycles through columns
		{
			conway[i][j].state = dead;//sets cell to dead
			conway[i][j].next = nil;//sets next to nil
		}
	}
return;
}
//Function description: This prints each generation
void print(cell conway[][col],int row)
{
	cout << "=====================================================================\n";
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			if(conway[i][j].state == dead)
			{
			 cout << " ";//just output a space if a cell is dead
			}
			else
			   {
				 cout << conway[i][j].state;
			   }
		}
		cout << endl;
	}
	cout << "=======================================================================\n";
return;
}
//Function description: Randomly sets cells to be alive for the first generation
void seed(cell conway[][col],int row, int rng)
{
srand(time(NULL));	

	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			rng = rand() % 20 + 1;
			if(!(rng%2))//Is rng divisible by 2?
			{
			  conway[i][j].state = alive;
			}
		}
	}
	//Example of a blinker
	/*
	conway[6][row-4].state = alive;
	conway[6][row-3].state = alive;
	conway[6][row-2].state = alive;*/
	
return;
}
//Funnction description: Decides which state each cell will be in the next generation
void iter(cell conway[][col],int row)
{
//variables
 int neighbors = 0;//keep track of how many neighbors surround a cell
	
 for(int i = 0; i < row; i++)//Cycles through rows
 {
	for(int j = 0; j < col; j++)//cycles through columns
	{
		if(conway[i][j].state == alive)//Check for a living cell
		{
			for(int t = 0; t < 3; t++)//checking if the 3 cells above the current cell are living
				{	//i is for the row, j is for the column, t(0,1,2) determiens which adjacent cell we are examining
					if((conway[i-1][j-1+t].state == alive)&&(toperr(i,j,t)))//check for living cell with out of bound error check
						neighbors++;
				}
				
			//checking if the cells to the side of the current cell are living with out of bounds error check
			if((conway[i][j-1].state == alive)&&(j-1>=0))//check for the cell directly to the left of the current cell
				neighbors++;
			if((conway[i][j+1].state == alive)&&(j+1<=row-1))//check for cell directly to the right of the current cell
				neighbors++;
				
			for(int b = 0; b < 3; b++)//checking if the 3 cells below the current cell are living
			    {
					if((conway[i+1][j-1+b].state == alive)&&(bottomerr(i,j,b)))
						neighbors++;
			    }
			
			if((neighbors < 2) || (neighbors > 3))//Death conditions: 1 neighbor or neighbors >= 4. 
			{
				conway[i][j].next = dead;
			}
		neighbors = 0;//resetting for the next cell to be analyzed
		}//end check for a living cells
		else//check a dead cell's neighbors
		{
// ******************Analyzing cells around the current dead cell to determine if it comes alive******
			for(int t = 0; t < 3; t++)//checking if the 3 cells above the current cell are living
				{
					if((conway[i-1][j-1+t].state == alive)&&(toperr(i,j,t)))
						neighbors++;
				}
				
			//checking if the cells to the side of the current cell are living
			if((conway[i][j-1].state == alive)&&(j-1>=0))//check for the cell directly to the left of the current cell
				neighbors++;
			if((conway[i][j+1].state == alive)&&(j+1<=row-1))//check for cell directly to the right of the current cell
				neighbors++;
				
			for(int b = 0; b < 3; b++)//checking if the 3 cells below the current cell are living
			    {
					if((conway[i+1][j-1+b].state == alive)&&(bottomerr(i,j,b)))
						neighbors++;
			    }
			
			if(neighbors == 3)//Life condition: if there are exactly 3 live cells around the current dead cell it comes alive
			{
			  conway[i][j].next = alive;
			}
		neighbors = 0;//resetting for next cell to be analyzed
		}
	}//end 2nd for loop
 }//end of first for loop
	
return;
}
//Function Description: This applies the states iter set to create the next generation
void ngen(cell conway[][col],int row)
{
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			if(conway[i][j].next != nil)//if next is not -1 then that means something needs to change in the current cell
			{	
			 conway[i][j].state = conway[i][j].next;
			 conway[i][j].next = nil;//reset for next iter call
			}
		}
	}
return;
}
//out of bounds error checking for adjacent cells above the cell being analysed
bool toperr(int r,int c, int s)
{
/*
r is row, c is column, s(can be 0,1, or 2) is the integer that allows us to check each cell
*/
	if((r-1>=0)&&(c-1+s>=0)&&(c-1+s<=row-1))
	{
		return true;
	}	
	
return false;
}
//Exact same thing as toperr but for adjacent cells below the cell being analysed
bool bottomerr(int r,int c, int s)
{
/*
r is row, c is column, s(can be 0,1, or 2) is the integer that allows us to check each cell
*/
	if((r+1<=(row-1))&&(c-1+s>=0)&&(c-1+s<=row-1))
	{
		return true;
	}	
	
return false;
}

/*
RULES:
1. Any live cell with less than 2 neighbors dies
2. Any live cell with 2 or 3 live neighbors lives
3. Any live cell with more than 3 live neighbors dies
4. Any dead cell with 3 live neighbors becomes alive

	Format:Cell(c) neighbors(n)
	
	nnn
	ncn
	nnn
*/