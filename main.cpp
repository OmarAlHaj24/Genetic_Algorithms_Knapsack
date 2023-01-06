#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#define POPULATION_SIZE 20
#define MUTATION_THRESHOLD 0.1
#define CROSSOVER_THRESHOLD 0.5
using namespace std;

void initialize(vector < vector <int> >& population, int numOfItems){
    for(int i = 0; i < POPULATION_SIZE; i++){
        vector <int> temp(numOfItems);
        for(int j = 0; j < numOfItems; j++){
            int r = round((double)rand() / RAND_MAX);
            temp[j] = r;
        }
        population.push_back(temp);
    }
}

vector <pair <int, int>> calcFitness(const vector < vector <int> >& population, int numOfItems,  vector <pair <int, int>>& vec, int& kSize){
    vector <pair <int, int>> fitness(population.size());

    for(int i = 0; i < population.size(); i++){
        int cumValue = 0, cumWeight = 0;
        for(int j = 0; j < numOfItems; j++){
            cumWeight += population[i][j] * vec[j].first;
            cumValue += population[i][j] * vec[j].second;
        }
        if(cumWeight > kSize){
            fitness[i] = {-1 * cumValue, i};
        }else{
            fitness[i] = {cumValue, i};
        }
    }
    return fitness;
}

pair <int, int> rankSelection(const vector <pair <int, int>>& fitness){
    vector <double> vec;
    vec.push_back(0);
    int n = POPULATION_SIZE * (POPULATION_SIZE + 1) / 2;
    double prev = 0;
    for(int i = 0; i < POPULATION_SIZE; i++){
        double curr = prev + 1.0 * (i + 1) / n;
        vec.push_back(curr);
        prev = curr;
    }
    vec[POPULATION_SIZE] += 0.1;
    double r1 = (double)rand() / RAND_MAX;
    double r2 = (double)rand() / RAND_MAX;

    int parent1, parent2;

    for(int i = 0; i < POPULATION_SIZE; i++){
        if(r1 >= vec[i] && r1 < vec[i + 1]){
            parent1 = fitness[i].second;
        }
        if(r2 >= vec[i] && r2 < vec[i + 1]){
            parent2 = fitness[i].second;
        }
    }

    return {parent1, parent2};
}

void mutation(vector < vector <int> >& population, int numOfItems){
    for(int i = 0; i < population.size(); i++){
        for(int j = 0; j < numOfItems; j++){
            double r = (double)rand() / RAND_MAX;
            if(r < MUTATION_THRESHOLD){
                population[i][j] = population[i][j] ^ 1;
            }
        }
    }

}

void replacement(vector < vector <int> >& population, int numOfItems,  vector <pair <int, int>>& vec, int& kSize){
    vector < vector <int> > newPop;
    vector <pair <int, int>> fitness = calcFitness(population, numOfItems, vec, kSize);
    sort(fitness.begin(), fitness.end());
    for(int i = fitness.size() - 20; i < fitness.size(); i++){
        newPop.push_back(population[fitness[i].second]);
    }
    population = newPop;
}

void parentSelection(vector < vector <int> >& population, int numOfItems,  vector <pair <int, int>>& vec, int& kSize){
    vector <pair <int, int>> fitness = calcFitness(population, numOfItems, vec, kSize);
    sort(fitness.begin(), fitness.end()); //reverse(fitness.begin(), fitness.end());
    vector <pair <int, int>> matched;
    for(int i = 0; i < 5; i++){
        double r = (double)rand() / RAND_MAX;
        pair <int, int> parents = rankSelection(fitness);
        if(r < CROSSOVER_THRESHOLD)
            matched.push_back(parents);
    }

    for(int i = 0; i < matched.size(); i++){
        vector <int> child1, child2;
        int r = rand() % numOfItems;
        for(int j = 0; j < r; j++){
            child1.push_back(population[matched[i].first][j]);
            child2.push_back(population[matched[i].second][j]);
        }
        for(int j = r; j < numOfItems; j++){
            child1.push_back(population[matched[i].second][j]);
            child2.push_back(population[matched[i].first][j]);
        }

        population.push_back(child1);
        population.push_back(child2);
    }
    mutation(population, numOfItems);
    replacement(population, numOfItems, vec, kSize);
}

int main() {
    freopen("input.in", "r", stdin); //freopen("output.out", "w", stdout);
    srand(time(0));
    int t; cin >> t;
    for(int n = 0; n < t; n++){
        int kSize, numOfItems; cin >> kSize >> numOfItems;
        vector <pair <int, int>> vec;
        for(int i = 0; i < numOfItems; i++){
            int x, y; cin >> x >> y;
            vec.push_back({x, y});
        }

        vector < vector <int> > population;

        initialize(population, numOfItems);

        for(int i = 0; i < 100; i++){
            parentSelection(population, numOfItems, vec, kSize);
        }

        int globalMax = 0;
        int globalNumItemsTaken = 0;
        int globalIdx = -1;
        for(int i = 0; i < POPULATION_SIZE; i++){
            int weight = 0, value = 0, numItemsTaken = 0;
            for(int j = 0; j < numOfItems; j++){
                weight += population[i][j] * vec[j].first;
                value += population[i][j] * vec[j].second;
                numItemsTaken += population[i][j];
            }
            if(weight <= kSize && globalMax < value){
                globalMax = max(globalMax, value);
                globalIdx = i;
                globalNumItemsTaken = numItemsTaken;
            }
        }
        cout << "== Test Case " << n+1 <<" =="<<endl;
        cout << "Number of Selected Items: " << globalNumItemsTaken <<endl;
        cout << "Total Value: " << globalMax << endl;
        for(int i = 0; i < numOfItems; i++){
            if(population[globalIdx][i]){
                cout << "	Item " << i+1 <<endl;
                cout << "		Weight: " <<vec[i].first <<endl;
                cout << "		Value: " <<vec[i].second <<endl;
            }
        }
        cout << endl;
    }
}