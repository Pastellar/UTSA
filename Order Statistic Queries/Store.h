#ifndef STORE_H
#define STORE_H

//libraries and constants
#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//struct for each Store
typedef struct{
	int id;
	char address[70];
	char city[20];
	char state[20];
	int zipCode;
	double latitude;
	double longitude;
	double distance;
} Store;

//function declarations
Store addStore(int id, char *a, char *c, char *s, int zip, double lat, double lon);

double toRadians(double num);

void calculateDistance(Store *store, double myLat, double myLon);

void printStoreInfo(Store store);

#endif