#include <iostream>
#include <time.h>
#include <vector>
#include "NeuralNetwork.h"
#include <fstream>

//This will generate a number from some arbitrary LO to some arbitrary HI :
//
//float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

//Roulette selection doesnt work

int main()
{
	srand((unsigned int)time(NULL));

	//Loading learning data

	std::vector<std::vector<std::vector <double>>> HandWrittenDigits;

	std::ifstream in("semeion.data");
	int counter = 0;
	while (in)
	{
		counter++;
		std::vector <double> input;
		std::vector <double> output;

		for (int j = 0; j < 16; j++)
		{
			for (int i = 0; i < 16; i++)
			{
				float value;
				in >> value;
				//std::cout << value;
				input.push_back(value);
			}
			//std::cout << std::endl;
		}
		for (int i = 0; i < 10; i++)
		{
			float value;
			in >> value;
			//std::cout << value;
			output.push_back(value);
		}
		//std::cout << std::endl << std::endl;

		std::vector <std::vector <double>> pattern;
		pattern.push_back(input);
		pattern.push_back(output);

		HandWrittenDigits.push_back(pattern);
		std::cout << counter << std::endl;

	}











	std::vector <int> Structure;
	Structure.push_back(256);
	Structure.push_back(30);
	Structure.push_back(10);

	std::vector<std::vector<std::vector <double>>> LearningMaterial;

	std::vector <std::vector <double>> pattern;
	std::vector <double> input;
	std::vector <double> output;
	input.push_back(0); //input
	output.push_back(0); //desired output
	pattern.push_back(input);
	pattern.push_back(output);

	std::vector <std::vector <double>> pattern2;
	std::vector <double> input2;
	std::vector <double> output2;
	input2.push_back(1); //input
	output2.push_back(1); //desired output
	pattern2.push_back(input2);
	pattern2.push_back(output2);

	std::vector <std::vector <double>> pattern3;
	std::vector <double> input3;
	std::vector <double> output3;
	input3.push_back(0.5); //input
	output3.push_back(0.5); //desired output
	pattern3.push_back(input3);
	pattern3.push_back(output3);


	std::vector <double> pattern4;
	pattern4.push_back(0.1); //input
	pattern4.push_back(0.1); //desired output

	LearningMaterial.push_back(pattern);
	LearningMaterial.push_back(pattern2);
	LearningMaterial.push_back(pattern3);
	//LearningMaterial.push_back(pattern4);

	for (int k = 0; k < 1; k++)
	{
		NeuralNetwork NN(Structure); //very simple
		NN.SetLearningMaterial(HandWrittenDigits);

		for (int i = 0; i < 2000; i++)
		{
			double fitness = NN.Learn();
			//if (fitness < 0.00000001) break;

			int howmanyclassifiedcorrectly = 0;
			for (int i = 0; i < HandWrittenDigits.size(); i++)
			{
				NN.SetInput(HandWrittenDigits[i][0]);
				NN.Evaluate();


				std::vector <double> Output;
				Output = NN.Output;
				int trueanswer = 0;
				double thegreatest = 0;
				for (int j = 0; j < 10; j++)
				{
					if (HandWrittenDigits[i][1][j] > thegreatest)
					{
						thegreatest = HandWrittenDigits[i][1][j];
						trueanswer = j;
					}
				}
				int answer = 0;
				double thegreatest2 = 0;
				for (int j = 0; j < 10; j++)
				{
					if (Output[j] > thegreatest2)
					{
						thegreatest2 = Output[j];
						answer = j;
					}
				}

				
				if (answer == trueanswer) howmanyclassifiedcorrectly++;

			}
			std::cout << howmanyclassifiedcorrectly << std::endl;
		}

		//for (int i = 0; i < HandWrittenDigits.size(); i++)
		//{
		//	NN.SetInput(HandWrittenDigits[i][0]);
		//	NN.Evaluate();


		//	std::vector <double> Output;
		//	Output = NN.Output;
		//	int trueanswer = 0;
		//	double thegreatest = 0;
		//	for (int j = 0; j < 10; j++)
		//	{
		//		if (HandWrittenDigits[i][1][j] > thegreatest)
		//		{
		//			thegreatest = HandWrittenDigits[i][1][j];
		//			trueanswer = j;
		//		}
		//	}
		//	int answer = 0;
		//	double thegreatest2 = 0;
		//	for (int j = 0; j < 10; j++)
		//	{
		//		if (Output[j] > thegreatest2)
		//		{
		//			thegreatest2 = HandWrittenDigits[i][1][j];
		//			answer = j;
		//		}
		//	}

		//	int howmanyclassifiedcorrectly = 0;
		//	if (answer == trueanswer) howmanyclassifiedcorrectly++;
		//	std::cout << howmanyclassifiedcorrectly << std::endl;

		//}

		//for (int i = 0; i < LearningMaterial.size(); i++)
		//{
		//	NN.SetInput(LearningMaterial[i][0]);
		//	NN.Evaluate();
		//	for (int j = 0; j<LearningMaterial[i][1].size(); j++)
		//	{
		//		std::cout << "DO: " << LearningMaterial[i][1][j] << " AO: " << NN.Output[j] << " | ";
		//	}

		//}
		std::cout << std::endl << std::endl;
	}


	
	system("pause");

}