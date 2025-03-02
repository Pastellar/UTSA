#include "Store.h"

//max length to read into string
#define MAX_LINE_LENGTH 150

//function declarations
void findStores(Store *stores, int sizeArray, double lat, double lon, int num);

Store randomizedSelection(Store *stores, int p, int r, int i);

int randomizedPartition(Store *stores, int p, int r);

int partition(Store *stores, int p, int r);

void quickSort(Store *stores, int p, int r);

void freeStore(Store *store);

//void countingSort(Store* stores, int sizeArray, int max);

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

		char input[MAX_LINE_LENGTH];

		
		int count = 0;
		//skips first line of the input file
		fgets(input, MAX_LINE_LENGTH, inputFile);
		//count number of lines in to file to allocate memory
		while(fgets(input, MAX_LINE_LENGTH, inputFile)!=NULL){
			count++;
		}

		//create and allocate memory for array of Stores
		Store *stores = (Store*)malloc(sizeof(Store) * count);
		//holds size of the array
		int sizeArray = count;

		//set pointer to beginning of the input file
		rewind(inputFile);

		//temp variables to store data read in from file
		int tempId = 0;
		char tempAddress[70];
		char tempCity[20];
		char tempState[20];
		int tempZip;
		double tempLat;
		double tempLong;
		//int prevId = 0;
		count = 0;

		//skips first line of the input file
		fgets(input, MAX_LINE_LENGTH, inputFile);

		//store data from each line then create new Store object and add the new object into the stores array
		while(fgets(input, MAX_LINE_LENGTH, inputFile)!=NULL){
			if(strchr(input, '\"') == NULL){
				sscanf(input, "%d,%[^,],%[^,],%[^,],%d,%lf,%lf", &tempId, tempAddress, tempCity, tempState, &tempZip, &tempLat, &tempLong);
			}
			else{
				char temptemp[50];
				sscanf(input, "%d,\"%[^\"]\",%[^,],%[^,],%d,%lf,%lf", &tempId, tempAddress, tempCity, tempState, &tempZip, &tempLat, &tempLong);
				//if(tempId == 11994){
				//	printf("%d, %s, %s %lf %lf\n", tempId, tempAddress, tempCity, tempLat, tempLong);
				//}
				
				//strcat(tempAddress, ",");
				//strcat(tempAddress, temptemp);
				//if(tempAddress[0] == '\"'){
				//	memmove(tempAddress, tempAddress+1, strlen(tempAddress));
				//}
				//int len = strlen(tempAddress);
				//tempAddress[len-1] = '\0';
			}
			//if(tempId == -98){
				//	printf("%d, %s, %s %lf %lf\n", tempId, tempAddress, tempCity, tempLat, tempLong);
				//	printf("%d\n", prevId);
				//}
			//prevId = tempId;
			stores[count] = addStore(tempId, tempAddress, tempCity, tempState, tempZip, tempLat, tempLong);
			count++;
		}

		//close the input file
		fclose(inputFile);

		//reset inputFile pointer
		inputFile = NULL;
		//open Queries.csv file
		inputFile = fopen("data/Queries.csv", "r");
		if(inputFile == NULL){

			perror("Error opening file");
			return -1;
		}

		//temp variables to store data read in from file
		double customerLatitude;
		double customerLongitude;
		int numStoresToFind;

		//skips first line of the input file
		fgets(input, MAX_LINE_LENGTH, inputFile);

		//store data from each line into temp variables, then call a method to fullfill the customer's query
		while(fgets(input, MAX_LINE_LENGTH, inputFile)!=NULL){
			sscanf(input, "%lf,%lf,%d", &customerLatitude, &customerLongitude, &numStoresToFind);
			findStores(stores, sizeArray, customerLatitude, customerLongitude, numStoresToFind);
		}

		//close the input file
		fclose(inputFile);

		//free stores array
		free(stores);
	}
	return 0;
}

//method that handles user query, calculates distances of all stores, and returns num closest stores in ascending order
void findStores(Store *stores, int sizeArray, double lat, double lon, int num){
	int i = 0;
	//calculate distances of all stores from the customer
	for(i = 0; i < sizeArray; i++){
		calculateDistance(&stores[i], lat, lon);
		//if(stores[i].id == 15){
		//	printStoreInfo(stores[i]);
		//}
	}
	//find the num closest store
	Store farthest = randomizedSelection(stores, 0, sizeArray-1, num);
	//prints num closest stores
	printf("The %d closest Stores to (%.7lf, %.7lf):\n", num, lat, lon);

	//int *sorted;
	//Store sort[num];
	//sorted = countingSort(stores, num, (int)(farthest.distance*100));
	quickSort(stores, 0, num-1);
	//printStoreInfo(farthest);
	for(i = 0; i < num; i++){
		printStoreInfo(stores[i]);
	}
	//free(sorted);
	printf("\n");
}

//Takes last element as the pivot, places the pivot element at its correct position in array, and places all smaller-than-pivot elements to the left of the pivot and all greater elements to the right of the pivot
//stores is the array of Store objects, p is the beginning index, r is the end index
//returns index of the pivot
int partition(Store *stores, int p, int r){
	//pivot element
	Store x = stores[r];
	//temp variable used to swap elements
	Store temp;
	//index of a smaller element and indicates the correct position of the pivot so far
	int i = p-1;
	//set j to the beginning element
	int j = p;
	//while j is less than the end index
	for(j = p; j < r; j++){
		//if the distance of the jth element is <= the distance of the last element, swap ith and jth elements
		if(stores[j].distance <= x.distance){
			//increment i by 1
			i++;
			temp = stores[i];
			stores[i] = stores[j];
			stores[j] = temp;
		}
	}
	//swap (i+1)th and end elements
	temp = stores[i+1];
	stores[i+1] = stores[r];
	stores[r] = temp;
	//returns pivot index
	return i+1;
}

//Randomly partition given array
//stores is the array of Store objects, p is the beginning index, r is the end index
//returns the index of the pivot
int randomizedPartition(Store *stores, int p, int r){
	//select random index as pivot index between p and r
	int i = rand()%(r-p)+p;
	//swap pivot element and r element
	Store temp = stores[i];
	stores[i] = stores[r];
	stores[r] = temp;
	//return pivot index
	return partition(stores, p, r);
}

//Makes the ith smallest element the ith element in the array, puts all elements smaller than the ith element before it
//stores is the array of Store objects, p is the beginning index, r is the end index, i is the ith smallest element to find
//returns the ith smallest element
Store randomizedSelection(Store *stores, int p, int r, int i){
	//return the only element if the partitioned array is size 1
	if(p == r){
		return stores[p];
	}
	//compute pivot index
	int q = randomizedPartition(stores,p,r);
	//number of elements <= pivot index
	int k = q-p+1;
	//if the pivot index is the ith smallest element, return the pivot element
	if(i == k){
		return stores[q];
	}
	//if there are more elements than you want below the pivot point, change end index to (pivot index)-1
	else if(i < k){
		return randomizedSelection(stores, p, q-1, i);
	}
	//if there are less elements than you want below the pivot point, change beginning index to (pivot index)+1
	else{
		return randomizedSelection(stores, q+1, r, i-k);
	}
}

//since every element below the ith smallest is smaller than that element, quick sort only the first i elements
//stores is the array of Store objects, p is the beginning index, r is the end index of what to sort
void quickSort(Store *stores, int p, int r){
	if(p < r){
		int i = partition(stores, p, r);

		quickSort(stores, p, i-1);
		quickSort(stores, i+1, r);
	}
}
/*
void countingSort(Store* stores, int size, int max){
	int counts[max+1];
	int i = 0;
	for(i = 0; i < max; i++){
		counts[i] = 0;
	}
	/*
	int numItemsBefore = 0;
	for(i = 0; i < max+1; i++){
		int count = counts[i];
		counts[i] = numItemsBefore;
		numItemsBefore += count;
	}
	
	for(i = 0; i < size; i++){
		sorted[counts[(int)stores[i].distance*100]] = stores[i];
		counts[(int)stores[i].distance*100]--;
	//}
	int sorted[size];
	for(i = 0; i < size; i++){
		counts[(int)(stores[i].distance*100)]++;
	}
	for(i = 1; i < max; i++){
		counts[i] = counts[i-1] + counts[i];
	}
	int j = 0;
	Store sort[size];
	for(j = size; j > 0; j--){
		printf("%d %d %d\n", (int)(stores[j].distance*100), counts[(int)(stores[j].distance*100)], (int)(stores[j].distance*100));
		sorted[counts[(int)(stores[j].distance*100)]] = (int)(stores[j].distance*100);
		counts[(int)(stores[j].distance*100)]--;
		sort[counts[(int)(stores[j].distance*100)]] = stores[j];
		printStoreInfo(stores[j]);
	}
	for(i = 0; i < size; i++){
		printStoreInfo(sort[i]);
	}
}*/

void freeStore(Store *store){
	free(store);
}