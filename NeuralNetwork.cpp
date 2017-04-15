#include "NeuralNetwork.h"
#include <iostream>
#include <functional>



NeuralNetwork::NeuralNetwork(std::vector <int> Structure)
{
	int NumberOfIndividuals = 200; //pick as you want

	Input.resize(Structure[0]);

	//Creating NN--------------------
	NeuralLayer temp(Input.size() + 1, Structure[1]); //+1 for bias
	NeuralNet.push_back(temp);

	for (int i = 2; i < Structure.size() - 1; i++)
	{
		NeuralLayer temp(Structure[i - 1] + 1, Structure[i]); //+1 for bias
		NeuralNet.push_back(temp);
	}

	NeuralLayer temp2(Structure[Structure.size() - 2] + 1, Structure[Structure.size() - 1]); //+1 for bias
	OutputLayer = temp2;
	Output.resize(Structure[Structure.size() - 1]);



	//Counting the size of Chromosome
	int SizeOfChromosome = 0;
	for (int i = 0; i < NeuralNet.size(); i++)
	{
		for (int j = 0; j < NeuralNet[i].Layer.size();j++)
		{
			SizeOfChromosome += NeuralNet[i].Layer[j].Weights.size();
		}
	}
	for (int i = 0; i < OutputLayer.Layer.size(); i++)
	{
		SizeOfChromosome += OutputLayer.Layer[i].Weights.size();
	}

	Population.resize(NumberOfIndividuals);
	for (unsigned int i = 0; i < NumberOfIndividuals; i++) //number of individuals
	{
		for (int j = 0; j < SizeOfChromosome; j++)
		{
			bool negative = rand() % 2;
			if (negative) Population[i].Chromosome.push_back(((double)rand() / (RAND_MAX)) - 1);//-1 < r < 0
			else Population[i].Chromosome.push_back(((double)rand() / (RAND_MAX))); //0 < r < 1
		}
	} //Creating Individuals with Chromosomes
	CrossoverRate = 0.5;
	MutationRate = 0.1;
	//displayStats = true;
	SelectionMethod = Rank;
	epoch = 0;
}

NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::SetInput(std::vector <double> input)
{
	this->Input = input;
}

void NeuralNetwork::SetWeights(std::vector<double> Weights)
{
	//this->Weights = Weights;
	int l = 0;
	for (int i = 0; i < NeuralNet.size(); i++)
	{
		for (int j = 0; j < NeuralNet[0].Layer.size(); j++)
		{
			for (int k = 0; k < NeuralNet[0].Layer[0].Weights.size(); k++)
			{
				NeuralNet[i].Layer[j].Weights[k] = Weights[l];
				l++;
			}
		}
	}
	for (int j = 0; j < Output.size();j++)
	{
		for (int k = 0; k < NeuralNet[NeuralNet.size() - 1].Layer.size(); k++) //the last layer
		{
			OutputLayer.Layer[j].Weights[k] = Weights[l];
			l++;
		}

	}
}

void NeuralNetwork::Evaluate()
{
	double bias = 1;


	//i - number of weight in neuron
	//j = number of neuron in neuron layer
	//k - number of layer

	//Calculate activation values in first layer
	for (int j = 0; j < NeuralNet[0].Layer.size(); j++)
	{
		double Activation = 0; //sum of weights * inputs
		for (int i = 0; i < Input.size(); i++)
		{
			Activation += Input[i] * NeuralNet[0].Layer[j].Weights[i]; // j - number of neuron in a layer
		}
		Activation += bias * NeuralNet[0].Layer[j].Weights[NeuralNet[0].Layer[j].Weights.size() - 1]; //bias*the last weight
		NeuralNet[0].Layer[j].Output = 1 / (1 + exp(-Activation)); //sigmoid function
	}
	//First Layer Done

	for (int k = 1; k < NeuralNet.size(); k++)
	{
		for (int j = 0; j < NeuralNet[k].Layer.size(); j++)
		{
			double Activation = 0; //sum of weights * inputs
			for (int i = 0; i < NeuralNet[k - 1].Layer.size(); i++)
			{
				Activation += NeuralNet[k - 1].Layer[i].Output * NeuralNet[k].Layer[j].Weights[i];
			}
			Activation += bias * NeuralNet[k].Layer[j].Weights[NeuralNet[k].Layer[j].Weights.size() - 1];
			NeuralNet[k].Layer[j].Output = 1 / (1 + exp(-Activation)); //sigmoid function
		}
	}

	//Evaluating the output layer

	int thelastLayer = NeuralNet.size() - 1; //bo pierwsza warstwa ma numer 0
	for (int j = 0; j < OutputLayer.Layer.size(); j++)
	{
		double Activation = 0; //sum of weights * inputs
		for (int i = 0; i < NeuralNet[thelastLayer].Layer.size(); i++) //for (int i = 0; i < NN[thelastLayer].Layer[j].numberOfInputs; i++) 
		{
			Activation += NeuralNet[thelastLayer].Layer[i].Output * OutputLayer.Layer[j].Weights[i];
		}
		Activation += bias * OutputLayer.Layer[j].Weights[OutputLayer.Layer[j].Weights.size() - 1];
		OutputLayer.Layer[j].Output = 1 / (1 + exp(-Activation)); //sigmoid function
	}


	Output.clear();
	for (int j = 0; j < OutputLayer.Layer.size(); j++)
	{
		Output.push_back(OutputLayer.Layer[j].Output);
	}



}

void NeuralNetwork::CalculateFitnessOfAll()
{
	for (int i = 0; i < Population.size(); i++)
	{
		double error = 0;
		SetWeights(Population[i].Chromosome);

		for (int j = 0; j < LearningMaterial.size(); j++)
		{
			SetInput(LearningMaterial[j][0]);
			Evaluate();
			std::vector <double> Output = this->Output;
			for (int i = 0; i < Output.size();i++)
			{
				error += pow(Output[i] - LearningMaterial[j][1][i], 2);
			}
		}
		Population[i].fitness = error;
		//if (0 == error) Population[i].fitness =  10;
		//else Population[i].fitness =  -5*abs(error) + 10;
	}
}

double NeuralNetwork::CalculateFitnessOfNet()
{
	double error = 0;

	for (int j = 0; j < LearningMaterial.size(); j++)
	{
		SetInput(LearningMaterial[j][0]);
		Evaluate();
		std::vector <double> Output = this->Output;
		for (int i = 0; i < Output.size();i++)
		{
			error += pow(Output[i] - LearningMaterial[j][1][i], 2);
		}
	}
	return error;
}

void NeuralNetwork::SetLearningMaterial(std::vector<std::vector<std::vector <double>>> LearningMaterial)
{
	this->LearningMaterial = LearningMaterial;
}

double NeuralNetwork::Learn()
{
	double speed = 0.05;
	int numberOfWeight = 0;
	std::vector <double> weights = Population[0].Chromosome;
	//for (int i = 0; i < 1; i++)
	//{
	//	Evolve();
	//}
	//Sort(Population, 0, Population.size() - 1);
	//return Population[0].fitness;
	double error3;

	for (int k = 0; k < 1000; k++)
	{
		for (int j = 0; j < weights.size(); j++)
		{
			for (int i = 0; i < 2; i++)
			{
				bool neededMore = false;
				SetWeights(weights);

				double error = CalculateFitnessOfNet();
				weights[j] += speed;
				SetWeights(weights);
				double error2 = CalculateFitnessOfNet();
				if (error2 < error) neededMore = true;

				if (neededMore) weights[0] += 0;
				else weights[j] -= 2 * speed;

				error3 = CalculateFitnessOfNet();

				std::cout << error3 << std::endl;
			}
		}
	}




	return error3;
}

void NeuralNetwork::Evolve()
{
	//epoch++;
	//Sort(Population, 0, Population.size()-1);
	Mutate();
	CalculateFitnessOfAll();

	//CalculateAverageFitness();

	Sort(Population, 0, Population.size() - 1);
	if (SelectionMethod == Roulette) RouletteSelection();
	if (SelectionMethod == Rank) RankSelection();
	Crossover();


	//if (displayStats)
	//{
	CalculateFitnessOfAll();
	Sort(Population, 0, Population.size() - 1);
	std::cout << "Epoch: " << epoch << " ";
	std::cout << "Error: " << Population[0].fitness << std::endl;
	//std::cout << "Average Fitness: " << avgFitness << std::endl << std::endl;
	//}
	epoch++;
}

void NeuralNetwork::Sort(std::vector <Individual> &Array, int left, int right, bool ascending)
{

	double v = Array[right].fitness; //Fitness of the last individual
	double i, j;
	Individual x;
	i = left;
	j = right;
	do {
		if (ascending)
		{
			while (Array[i].fitness<v) i++;
			while (Array[j].fitness>v) j--;
		}
		else
		{
			while (Array[i].fitness>v) i++;
			while (Array[j].fitness<v) j--;
		}

		if (i <= j) {
			x = Array[i];
			Array[i] = Array[j];
			Array[j] = x;
			i++; j--;
		}
	} while (i <= j);
	if (j>left) Sort(Array, left, j);
	if (i<right) Sort(Array, i, right);
}

void NeuralNetwork::SlowSort()
{
	//sort the population by fitness
	std::vector <Individual> Sorted;
	std::vector <int> Checked; //Individuals ktore zostaly juz posortowane

	for (int k = 0; k < Population.size(); k++)
	{
		double theBestFitness = 99999999;
		int numberOfTheBest = 0;

		for (int i = 0; i < Population.size(); i++)
		{
			if (Population[i].fitness < theBestFitness)
			{
				bool isChecked = false;
				for (int j = 0; j < Checked.size(); j++)
				{
					if (Checked[j] == i) isChecked = true;
				}
				if (isChecked == false)
				{
					theBestFitness = Population[i].fitness;
					numberOfTheBest = i;
				}
			}

		}
		Checked.push_back(numberOfTheBest);
		Sorted.push_back(Population[numberOfTheBest]);
		std::cout << k << std::endl;
	}
	//Invert
	int j = 0;
	std::vector <Individual> QSorted;
	QSorted.resize(Sorted.size());
	for (int i = Sorted.size() - 1; i >= 0; i--)
	{
		QSorted[j] = Sorted[i];
		j++;
	}
	for (int i = 0; i < Sorted.size(); i++)
	{
		Sorted[i] = QSorted[i];
	}
	Population = Sorted;
	//Sorted
}

void NeuralNetwork::Mutate()
{
	double result = rand() / RAND_MAX;

	if (result < MutationRate)
	{
		for (int i = 0; i < Population.size(); i++)
		{
			int which = rand() % 2;
			if (which == 0)
			{
				int random = rand() % Population[0].Chromosome.size();
				int random1 = rand() % Population.size();
				int random2 = rand() % Population[0].Chromosome.size();
				Population[i].Chromosome[random] += Population[random1].Chromosome[random2]; //adding value from random chromosome in population
			}
			if (which == 1)
			{
				bool negative = rand() % 2;
				int random = rand() % Population[0].Chromosome.size();

				if (negative) Population[i].Chromosome[random] += (((double)rand() / (RAND_MAX)) - 1);//-1 < r < 0
				else Population[i].Chromosome[random] += (((double)rand() / (RAND_MAX))); //0 < r < 1
			}

		}
	}
}

void NeuralNetwork::RouletteSelection()
{
	double FitnessSum = 0;
	for (int i = 0; i < Population.size(); i++)
	{
		FitnessSum += Population[i].fitness;
	}
	Sort(Population, 0, Population.size() - 1, false);

	std::vector <Individual> Roulette;
	for (int i = 0; i < Population.size(); i++)
	{
		double r = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (FitnessSum)));
		int j = 0;
		double sum = 0;
		for (j = 0; j<Population.size() - 1; j++)
		{
			sum += Population[j].fitness;
			if (sum > r) break;
		}
		Roulette.push_back(Population[j]);
	}
	Population = Roulette;
}

void NeuralNetwork::RankSelection()
{
	for (int i = 0, j = Population.size(); i < Population.size(); i++, j--)
	{
		Population[i].fitness = j;
	}
	RouletteSelection();
}

void NeuralNetwork::Crossover()
{
	std::vector <Individual> Crossover;
	//Dla kazdych dwoch
	for (int i = 0; i < Population.size() / 2; i++)
	{
		bool isCrossed = 0;
		double result = rand() / RAND_MAX;
		if (result < CrossoverRate) isCrossed = true;
		else isCrossed = false;

		if (isCrossed)
		{
			for (int j = 0; j < 2; j++)
			{
				int crossing = rand() % Population[i].Chromosome.size(); //wylosuj punkt przeciecia chromosomu
				Individual temp;
				for (int k = 0; k < crossing; k++)
				{
					temp.Chromosome.push_back(Population[2 * i].Chromosome[k]);
				} //kopiuj geny do punktu crossing

				for (int k = crossing; k < Population[i].Chromosome.size(); k++)
				{
					temp.Chromosome.push_back(Population[2 * i + 1].Chromosome[k]);
				} //kopiuj geny od punktu crossing z kolejnego osobnika

				Crossover.push_back(temp);

			}
		}
		else
		{
			Crossover.push_back(Population[2 * i]);
			Crossover.push_back(Population[2 * i + 1]);
		}
		//Wygeneruj dwa nowe


	}
	Population = Crossover;
}

Individual::Individual()
{

}

Individual::~Individual()
{
}

Neuron::Neuron(int NumberOfNeuronsInPreviousLayer)
{
	for (int i = 0; i < NumberOfNeuronsInPreviousLayer; i++)
	{
		Weights.push_back(static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / 2)) - 1); //(double number from -1 to 1)
	}
}

Neuron::~Neuron()
{
}

NeuralLayer::NeuralLayer(int NumberOfNeuronsInPreviousLayer, int NumberOfNeurons)
{
	for (int i = 0; i < NumberOfNeurons; i++)
	{
		Neuron temp(NumberOfNeuronsInPreviousLayer);
		Layer.push_back(temp);
	}

}

NeuralLayer::NeuralLayer()
{
}

NeuralLayer::~NeuralLayer()
{
}
