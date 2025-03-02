#include "Store.h"

//function definitions
//creates and returns a new Store object, given all data members of Store excluding distance
Store addStore(int id, char *a, char *c, char *s, int zip, double lat, double lon){
	Store *newStore = (Store*)malloc(sizeof(Store));
	newStore->id = id;
	strcpy(newStore->address, a);
	strcpy(newStore->city, c);
	strcpy(newStore->state, s);
	newStore->zipCode = zip;
	newStore->latitude = lat;
	newStore->longitude = lon;
	//newStore->distance = NULL;

	return *newStore;
}

//convert double value into radians
double toRadians(double num){
	return num * (M_PI / 180.0);
}

//calculate distance from the store to the customer's location. Stores the result in the store's distance variable
void calculateDistance(Store *store, double myLat, double myLon){
	double radiusOfEarthInMiles = 3958.8;

	//convert each location to radians
	double lat1 = toRadians(store->latitude);
	double lon1 = toRadians(store->longitude);
	double lat2 = toRadians(myLat);
	double lon2 = toRadians(myLon);

	//formula to calculate distance
	double a = pow(sin((lat2-lat1)/2), 2) + cos(lat1)*cos(lat2)*pow(sin((lon2-lon1)/2), 2);
	double c = 2*atan2(sqrt(a), sqrt(1-a));
	double distance = radiusOfEarthInMiles*c;

	//store result in store's distance variable
	store->distance = distance;
}

//prints the given Store's info
void printStoreInfo(Store store){
	printf("Store #%d. %s, %s, %s, %d. - %.2lf miles.\n", store.id, store.address, store.city, store.state, store.zipCode, store.distance);
}