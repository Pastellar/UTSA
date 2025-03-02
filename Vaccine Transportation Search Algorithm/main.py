#!/usr/bin/python

import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import csv
from collections import defaultdict

# Used to create the graph
class Graph():
    def __init__(self):
        self.cities = defaultdict(list)
        self.distances = {}

    def addEdge(self, sourceCity, goalCity, distance):
        distance = float(distance)  # Convert distance to float
        self.cities[sourceCity].append(goalCity)
        self.cities[goalCity].append(sourceCity)
        self.distances[(sourceCity, goalCity)] = distance
        self.distances[(goalCity, sourceCity)] = distance  # Ensure undirected graph

# Main function
def main():
    basePath = "/content/drive/My Drive/Colab Notebooks/Artificial Intelligence/Data"
    cityPath = f"{basePath}/cities.csv"

    header = ["CityName", "Latitude", "Longitude"]
    citydf = pd.read_csv(cityPath, names=header)

    create_map(citydf)

    graph = Graph()
    create_graph(graph)

    spreadVirus(graph, 'Three Rivers')
    transportVaccine(graph, 'San Antonio', 'College Station')

# Function to plot cities
def create_map(citydf):
    plt.title('Map of Texas Cities')
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')

    imgPath = "/content/drive/My Drive/Colab Notebooks/Artificial Intelligence/Images/texasMap.png"
    map_img = plt.imread(imgPath)

    extent = citydf.Longitude.min() - 1.5, citydf.Longitude.max() + 2, citydf.Latitude.min() - 2, citydf.Latitude.max() + 3
    plt.rcParams["figure.figsize"] = (12, 20)
    plt.imshow(map_img, extent=extent, aspect='equal')
    plt.scatter(citydf.Longitude, citydf.Latitude)

    for i, txt in enumerate(citydf.CityName):
        plt.annotate(txt, (citydf.Longitude[i], citydf.Latitude[i]), horizontalalignment='left', size='medium', color='black', weight='semibold')

    plt.show()

# Function to create graph
def create_graph(graph):
    distancePath = "/content/drive/My Drive/Colab Notebooks/Artificial Intelligence/Data/distances.csv"
    
    with open(distancePath, 'r') as f:
        reader = csv.reader(f, delimiter=',')
        for edge in reader:
            if len(edge) == 3:
                graph.addEdge(edge[0], edge[1], edge[2])  # Ensure distance is added

# Function to spread virus using Dijkstra's Algorithm
def spreadVirus(graph, sourceCity):
    shortestDistances = {city: sys.maxsize for city in graph.cities}
    shortestDistances[sourceCity] = 0
    visited = set()
    parents = {sourceCity: None}

    while len(visited) < len(graph.cities):
        currentCity = min((city for city in graph.cities if city not in visited), key=lambda city: shortestDistances[city])
        visited.add(currentCity)

        for neighbor in graph.cities[currentCity]:
            if neighbor in visited:
                continue

            new_distance = shortestDistances[currentCity] + graph.distances[(currentCity, neighbor)]
            if new_distance < shortestDistances[neighbor]:
                shortestDistances[neighbor] = new_distance
                parents[neighbor] = currentCity

    print("City\t Distance\t Path")
    for city, distance in shortestDistances.items():
        if city == sourceCity:
            continue
        path = []
        step = city
        while step is not None:
            path.append(step)
            step = parents.get(step)
        path.reverse()
        print(f"{sourceCity} -> {city}\t {distance}\t {' -> '.join(path)}")

    print(f"Total Distance Traveled: {sum(shortestDistances.values())}\n")

# Function to transport vaccines using an informed search
def transportVaccine(graph, sourceCity, goalCity):
    shortestPath = {sourceCity: (None, 0)}
    visited = set()
    currentCity = sourceCity

    while currentCity != goalCity:
        visited.add(currentCity)
        destinations = graph.cities[currentCity]
        distanceToCurrCity = shortestPath[currentCity][1]

        for nextCity in destinations:
            distance = graph.distances[(currentCity, nextCity)] + distanceToCurrCity
            if nextCity not in shortestPath or shortestPath[nextCity][1] > distance:
                shortestPath[nextCity] = (currentCity, distance)

        nextPath = {city: shortestPath[city] for city in shortestPath if city not in visited}
        if not nextPath:
            print("Route not possible :(")
            return

        currentCity = min(nextPath, key=lambda k: nextPath[k][1])

    path = []
    while currentCity is not None:
        path.append(currentCity)
        currentCity = shortestPath[currentCity][0]
    path.reverse()

    print("City\t Distance\t Path")
    for i, city in enumerate(path):
        if i > 0:
            print(f"{path[i-1]} -> {city}\t {shortestPath[city][1]}")
    print(f"Total Distance Traveled: {shortestPath[goalCity][1]}\n")