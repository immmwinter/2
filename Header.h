#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

const int WIDTH = 50;
const int HEIGHT = 20;
const int HISTORY_SIZE = 10;

void loadHistory(int history[], int graph[]) {
    ifstream inFile("graph_history.txt");
    if (inFile) {
        for (int i = 0; i < HISTORY_SIZE; i++) {
            inFile >> history[i];
            graph[i] = history[i];
        }
        inFile.close();
    }
    else {
        for (int i = 0; i < HISTORY_SIZE; i++) {
            graph[i] = HEIGHT / 2;
        }
    }
}

void saveHistory(int graph[]) {
    ofstream outFile("graph_history.txt");
    for (int i = WIDTH - HISTORY_SIZE; i < WIDTH; i++) {
        outFile << graph[i] << " ";
    }
    outFile.close();
}

void generateGraph(int graph[]) {
    for (int i = HISTORY_SIZE; i < WIDTH; i++) {
        int change = (rand() % 3) - 1;
        graph[i] = graph[i - 1] + change;
        graph[i] = max(0, min(graph[i], HEIGHT - 1));
    }
}

void displayGraph(int graph[]) {
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            cout << (graph[w] == h ? "|" : " ");
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));
    int graph[WIDTH], history[HISTORY_SIZE];

    loadHistory(history, graph);
    generateGraph(graph);
    displayGraph(graph);
    saveHistory(graph);

    return 0;
}
#pragma once
