/* Annual Programming Efficiency Conference and Challenge
Compiled in Microsoft Visual Studio 2010
Submitted by Jithendar Anandan
CWID - 50154397
Texas A&M Univerity - Commerce */

#include <fstream>  // header file needed to read a file
#include<iostream>
#include<time.h>  
using namespace std;

//Function declaration
void mode(long data[], long size,long &numberOfmodes, long modeNumbers[],long &maxFrequency);

//Main Function
void main ()
{
 long double time1; // variables used to time the code
  clock_t start,stop;  // clock_t is a valid type
  double duration;

  long *data,*modeNums,size; // polonger needed to dynamically allocate memory
  long num_modes,maxFrequency;
  // the name of the file containing the test data is “TestData.bin”
  ifstream binaryin("TestData.bin", ios::binary);  // statement to open the binary file
  binaryin.read(reinterpret_cast<char *>(&size), 4);    // statement to read the size of the data array to allocate
  data = new(nothrow)long[size];  // statement to dynamically allocate the data array to have size elements
  modeNums = new(nothrow)long[size];
  if(!data)  // this statement will test the polonger to determine if memory was dynamically allocated
    { cout<<"Memory allocation error for data array, program will terminate\n";
       system("pause");  // this is a windows command to pause the program
       exit(0); }    // this statement will terminate the  program if memory was not allocated
  // statement to read the data/values in the file and store the numbers in the array data.
  binaryin.read(reinterpret_cast<char *>(data), size*sizeof(long));  

  start=clock(); // Start the benchmark

  mode(data,size,num_modes,modeNums,maxFrequency);

  stop=clock();  // stop the clock
  time1 = double(stop-start)/CLK_TCK; // compute/record the time

  //Display output
  cout<<"time to execute the mode : "<<time1<<endl;  // output the time to compute the mode(s)
  cout<<"The Modes are : ";
  for(int i = 0; i < num_modes; i++)
  {
    cout<<modeNums[i]<<" ";
  }
  cout<<endl;
  cout << "No. of modes : " << num_modes << endl; 
  cout << "Maximum frequency : " << maxFrequency << endl; 
  delete[] data; // free up the memory
  system("pause");
  return;
}

void mode(long data[], long size,long &numberOfmodes, long modeNumbers[],long &maxFrequency)
{
  long max, min, range = 0;
  max = min = data[0];

  //Find the largest and smallest number in the dataset - O(n)
  for (int i = 0; i < size; i++) 
  {
    if(data[i] > max) //compare biggest value with current element
        max=data[i];
    else if(data[i] < min) //compares smallest value with current element
        min=data[i];
  }

  //Deciding the range based on the minimum and maximum value 
  if(min >= 0) //Case if all the numbers in the dataset are positive
  {
    range = max - min + 1;
  }
  else if ( min < 0 && max > 0) //Case if it contains mixed values
  {
    range = max + abs(min) + 1;
  }
  else //Case if all the numbers in the dataset are negative
  {
    //Switch min and max
    max = abs(min);
    min = abs(max);
    range = max - min + 1;
  }
  
  //Create a new array called data_freq and dynamically allocate the data_freq array to have range elements
  auto data_freq = new(nothrow)long[range]; 
  long offset;

  //Initialize the data_freq array to 0 - O(range)
   fill(data_freq, data_freq + range, 0);

   //Reading the frequency of each number in the dataset to data_freq array - O(n)
   if (min >= 0 || max < 0) //Case if all values are positive or negative
   {
     for (int i = 0; i < size; i++)
     {
      offset = abs(data[i]) - min;
      data_freq[offset] = data_freq[offset] + 1;
     }
   }
   else //Incase of mixed values
   {
     for (int i = 0; i < size; i++)
     {
       if(data[i] < 0)
          offset = abs(data[i]);
        else 
          offset = data[i] + abs(min);
       data_freq[offset] = data_freq[offset] + 1;
     }
   }

   long modeOffset = 0; 
   numberOfmodes = 1;

   //Finding the modes of the data array - O(range)
   for (int i = 1; i < (range - 1); i++)
   {
      /*Enter loop if the frequency of the number 
      is greater or eaqual to the frequency of previous mode */
      if(data_freq[i] >= data_freq[modeOffset])
      {
        /* If the  frequency of the number is greater than the frequency of previous mode, 
          re-initialize modeNums array to erase the previous modes and store the new mode */
        if(data_freq[i] > data_freq[modeOffset])
        {
          modeOffset = i; 
          maxFrequency = data_freq[modeOffset];
          numberOfmodes = 0; 
          //store the new mode
          if (min >= 0)
             modeNumbers[numberOfmodes++] = min + modeOffset; 
          else if ( min < 0 && max > 0)
          {
            if(modeOffset < abs(min))
              modeNumbers[numberOfmodes++] = -modeOffset;
            else
              modeNumbers[numberOfmodes++] = modeOffset - abs(min);
          }
          else 
             modeNumbers[numberOfmodes++] = -modeOffset;
        }
        /* If the  frequency of the number is equal to the frequency of previous mode,
        record the new mode */
        else if (data_freq[i] == data_freq[modeOffset])
        {
          if (min >= 0)
            modeNumbers[numberOfmodes++] = min + i;
          else if (min < 0 && max > 0)
          {
            if(i < abs(min))
              modeNumbers[numberOfmodes++] = -i;
            else 
              modeNumbers[numberOfmodes++] = i - abs(min);
          }
          else
            modeNumbers[numberOfmodes++] = -i;
        }
      }
   }
  return;
}