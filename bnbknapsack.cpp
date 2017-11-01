//============================================================================
// Name        : bnbknapsack.cpp
// Author      : Adam Lamoglia
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

long int estimateMaximumValue = 0;

struct item{
	long int value;
	double weight;
	double density;
	int index;
};

struct node{
	long int profit;
	long int level;
	long int bound;
	double weight;
	vector<bool>decision;
};



//sort items by decreasing order of value/weight
bool cmp(item a, item b){
	if(a.density == b.density)
		return a.value > b.value;
	return a.density > b.density;
}



//defining the new highest possible value of the knapsack
int relax(vector<item>&knapsack ,node u, int items, int capacity){

	if(u.weight >= capacity)
		return 0;

	//that is the bound of item
	int estimateValue = u.profit;

	//next level of the tree
	int nextLevel = u.level + 1;

	int totalWeight = u.weight;

	//walking on the tree
	while((nextLevel < items) && (totalWeight + knapsack[nextLevel].weight <= capacity)){

		totalWeight += knapsack[nextLevel].weight;
		estimateValue += knapsack[nextLevel].value;
		nextLevel++;
	}

	//put a part of the next item in the knapsack
	//it works like a fractional knapsack
	if(nextLevel < items){
		estimateValue += (capacity - totalWeight)*(knapsack[nextLevel].value/knapsack[nextLevel].weight);

	}

	return estimateValue;

}


void bnbKnapsack(vector<item> &knapsack, vector<item> &decisionArray, int capacity, int items){

	int maxValue = 0,sum,maxWeight,finalWeight;
	bool result = false;

	//the queue go through on tree
	queue <node> branch;

	vector<bool>decisionVariables;

	//auxiliar nodes
	node u,v;

	//initializing nodes
	u.level = -1;
	u.profit = u.weight = 0;
	u.decision.resize(items);

	branch.push(u);

	while(!branch.empty()){
		sum = 0;
		maxWeight = 0;

		u = branch.front();
		branch.pop();


		//assign the first level of tree
		if(u.level == -1)
			v.level = 0;


		//last level of tree
		else if(u.level == items - 1)
			continue;


		//goes to next level (except if u.level = 1)
		v.level = u.level + 1;

		//catching the weight and value in the current level of our tree
		v.weight = u.weight + knapsack[v.level].weight;
		v.profit = u.profit + knapsack[v.level].value;
		v.decision = u.decision;

		//if the current value is greater than the maximum value
		//then we take the item
		if(v.weight <= capacity && v.profit > maxValue){
			maxValue = v.profit;
			v.decision[knapsack[v.level].index] = true;
			decisionVariables = v.decision;
		}


		//catch the current maximum estimate value
		v.bound = relax(knapsack,v,items,capacity);


		//go through the tree adding the current item on knapsack
		if(v.bound > maxValue){
			//cout << "take the item: " << v.level << " goes on the queue" << endl;
			v.decision[knapsack[v.level].index] = true;
			branch.push(v);

		}

		//not adding the item on knapsack
		v.weight = u.weight;
		v.profit = u.profit;


		v.bound = relax(knapsack,v,items,capacity);

		//go through the tree, not adding the current item on knapsack
		if(v.bound > maxValue){

			v.decision[knapsack[v.level].index] = false;
			branch.push(v);
		}

		//cout << knapsack[v.level].index << endl;

	}



	cout << maxValue << " " << 1 << endl;


	for(int i = 0;i < items; i++){
		if(decisionVariables[i] == false)
			cout << 0 << " ";
		else
			cout << 1 << " ";
	}


}


int main() {
	long int items, capacity;

	cin >> items >> capacity;

	//our knapsack
	vector<item> knapsack(items);

	vector<item> aux(items);

	for(int i = 0; i < items; i++){

		cin >> knapsack[i].value >> knapsack[i].weight;

		knapsack[i].density = knapsack[i].value/knapsack[i].weight;
		knapsack[i].index = i;

		aux[i].value = knapsack[i].value;
		aux[i].weight = knapsack[i].weight;
		aux[i].density = knapsack[i].density;
		aux[i].index = i;
	}


	sort(aux.begin(),aux.end(),cmp);


	bnbKnapsack(aux,knapsack,capacity,items);


}
