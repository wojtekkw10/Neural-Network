#include <vector>
#include <functional>
#pragma once
enum SelectionMethod
{
	Roulette,
	Rank
};
class Individual
{
private:

public:
	std::vector <double> Chromosome;
	double fitness;

	Individual();
	~Individual();
};

class Neuron
{
private:

public:
	double Output;
	std::vector <double> Weights;

	Neuron(int NumberOfNeuronsInPreviousLayer);
	~Neuron();
};

class NeuralLayer
{
private:

public:
	double output;
	std::vector <Neuron> Layer;

	NeuralLayer(int NumberOfNeuronsInPreviousLayer, int NumberOfNeurons);
	NeuralLayer();
	~NeuralLayer();
};

class NeuralNetwork
{
public:
	NeuralNetwork(std::vector <int> Structure);
	~NeuralNetwork();

	std::vector <NeuralLayer> NeuralNet;

	std::vector<std::vector<std::vector <double>>> LearningMaterial;
	void SetLearningMaterial(std::vector<std::vector<std::vector <double>>> LearningMaterial);

	//double Input;
	//void SetInput(double input);
	std::vector <double> Input;
	void SetInput(std::vector <double> input);

	//double Output;
	NeuralLayer OutputLayer;
	std::vector <double> Output;
	//std::vector <double> GetOutput();

	double Learn(); //the best fitness returned
	void Evaluate();







	//GA/////////////////////////////////////////////
	std::vector <Individual> Population;
	double MutationRate;
	double CrossoverRate;
	int epoch;

	//std::vector <double> Weights;
	void SetWeights(std::vector <double> Weights);

	void CalculateFitnessOfAll();
	double CalculateFitnessOfNet();

	void Evolve();
	void Sort(std::vector <Individual> &Array, int left, int right, bool ascending = true);
	void SlowSort();
	void Mutate();

	void RouletteSelection();
	void RankSelection();
	SelectionMethod SelectionMethod;

	void Crossover();

};


