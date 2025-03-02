#include "Project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//max length to read into string
#define MAX_LINE_LENGTH 20

//function declarations
double mostProfit(double* array, int left, int right);

double highestPrice(double* array, int left, int right);

double lowestPrice(double* array, int left, int right);

double max(double num1, double num2);

double min(double num1, double num2);

int main(int argc, char *argv[])
{
	//check if a file name was given
	if(argc == 2){
		//open file
		FILE *inputFile;
		inputFile = fopen(argv[1], "r");
		if(inputFile == NULL){

			perror("Error opening file");
			return -1;
		}

		//holds value of each line, double parsed out
		char doubleString[MAX_LINE_LENGTH];

		//count number of lines in to file to allocate memory
		int count = 0;
		while(fgets(doubleString, MAX_LINE_LENGTH, inputFile)!=NULL){
			count++;
		}

		//create and allocate memory for array of doubles to hold each price as an element
		double *array = (double*)malloc(sizeof(double) * count);
		//holds size of the array
		int sizeArray = count;

		//set pointer to beginning of the input file
		rewind(inputFile);
		count = 0;

		//reads string from each line into doubleString
		//parse double from doubleString into the array
		while(fgets(doubleString, MAX_LINE_LENGTH, inputFile)!=NULL){
			//doubleString[strcspn(doubleString, "\n")] = 0;
			sscanf(doubleString, "%lf", &array[count]);
			count++;
		}

		//close the input file
		fclose(inputFile);

		//if there are 2+ elements in the array, recursively calculate the maximum buy/sell profit
		if(sizeArray > 1){
			double profit = mostProfit(array, 0, sizeArray-1);
			printf("The optimal solution for %s is %.2lf\n", argv[1], profit);
		}
		else{
			printf("Need 2+ prices\n");
		}

		//free memory allocated to array
		free(array);
	}
	return 0;
}

//recursive divide and conquer algorithm to calculate the maximum possible profit
//base case: if left and right are the same index, return 0
//calculate the middle index to split the array into a left and right side
//recursively call the method until arrays are only one element large
//keep returning the maximum profit for each split array until back at the first call of the method
//return maximum profit
//runtime is T(n) = 2T(n/2) + O(n)
//time complexity is O(nlogn)
double mostProfit(double* array, int left, int right){
	if(left == right){
		return 0;
	}
	else{
		int mid = left + (right - left) / 2;
		double mostProfitLeft = mostProfit(array, left, mid);
		double mostProfitRight = mostProfit(array, mid+1, right);
		double mostProfitMiddle = highestPrice(array, mid+1, right) - lowestPrice(array, left, mid);
		return max(max(mostProfitRight, mostProfitLeft), mostProfitMiddle);
	}
}

//finds the highest price in the given array
double highestPrice(double* array, int left, int right){
	double highest = array[left];
	while(++left <= right){
		highest = max(highest, array[left]);
	}
	return highest;
}

//finds the lowest price in the given array
double lowestPrice(double* array, int left, int right){
	double lowest = array[left];
	while(++left <= right){
		lowest = min(lowest, array[left]);
	}
	return lowest;
}

//find the largest of the given numbers, num1 & num2
double max(double num1, double num2){
	return (num1 > num2) ? num1 : num2;
}

//find the smallest of the given numbers, num1 & num2
double min(double num1, double num2){
	return (num1 > num2) ? num2 : num1;
}