#   Order Statistic Queries
    This program reads the IDs, Addresses, Latitude, and Longitude of each
    store location from csv files and stores them into a struct. Then
    performs user queries to find stores closest to the user.
    
    Using the Rand-Select() algorithm, each query:
    -   Computes the distance of the query point to each of the different
        stores using the Haversine Formula.
    -   Uses the order statistic query to find the farthest store from the 
        query that we care about.
    -   Once the #th closest store to the query is known, the other stores
        that are at least that distance to the query are sorted in increasing
        distance from the query point and printed to an output file.
    
    This program was written for our Analysis of Algorithms class.

##  Authors
    - Haven Kotara      https://github.com/Pastellar
    - Andrew McNeil     https://github.com/BelowAverageCoder

##  Note
    Due to the time constraints, the resulting formatting and writing
    for this program is a bit messy. Commented-out, broken code is
    left in for future review.