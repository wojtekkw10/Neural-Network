#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <fstream>
#include "NeuralNetwork.h"
#include <thread>
#include <future>

void Learn(NeuralNetwork *NN, std::vector<std::vector<std::vector <double>>> HandWrittenDigits);
std::vector<double> PixelsToinputForNN(std::vector<sf::Uint8> Pixels);


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

	tgui::Gui gui{ window };

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
	//////////////////////////////////////////////

	/*
	std::vector <double> WeightsFromFile;
	std::ifstream w("weights.txt");
	while (w)
	{
		double temp;
		w >> temp;
		WeightsFromFile.push_back(temp);
	}
	*/

	std::vector <int> Structure;
	Structure.push_back(256);
	Structure.push_back(30);
	Structure.push_back(10);

	sf::Texture texture;
	texture.create(16, 16);
	std::vector<sf::Uint8> pixels;
	for (int i = 0; i < 16 * 16 * 4; i++)
	{
		pixels.push_back(255);
	}
	texture.update(&pixels[0]);


	sf::Font font;
	font.loadFromFile("thundertrooper.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(124);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(400,0));





	NeuralNetwork NN(Structure);
	NN.SelectionMethod = Rank;
	NN.SetLearningMaterial(HandWrittenDigits);

	/*
	std::cout << "Size of All Loaded Weights: " << WeightsFromFile.size() << std::endl;
	std::cout << "Size of single Chromosome: " << NN.Population[0].Chromosome.size() << std::endl;

	if (WeightsFromFile.size() >= NN.Population.size() * NN.Population[0].Chromosome.size())
	{
		for (int j = 0; j < NN.Population.size(); j++)
		{
			int size = NN.Population[j].Chromosome.size();
			NN.Population[j].Chromosome.clear();
			for (int i = 0; i < size; i++)
			{
				NN.Population[j].Chromosome.push_back(WeightsFromFile[i]);
			}
		}
	}
	*/

	auto f = std::async(Learn, &NN, HandWrittenDigits);

	std::vector <double> TheBestWeightsSoFar;
	NeuralNetwork NewNN(Structure);
	NewNN.SetWeights(NN.Population[0].Chromosome);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			gui.handleEvent(event);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// getting coordinates and updating pixels
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			if (localPosition.x / 20 < 16 && localPosition.x / 20 >= 0 && localPosition.y / 20 < 16 && localPosition.y / 20 >= 0)
			{
				pixels[localPosition.x / 20 * 4 + localPosition.y / 20 * 16 * 4] = 0;
				texture.update(&pixels[0]);
			}

		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			// getting coordinates and updating pixels
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			if (localPosition.x / 20 < 16 && localPosition.x / 20 >= 0 && localPosition.y / 20 < 16 && localPosition.y / 20 >= 0)
			{
				pixels[localPosition.x / 20 * 4 + localPosition.y / 20 * 16 * 4] = 255;
				texture.update(&pixels[0]);
			}

		}

		//Checking if thread is still running
		auto status = f.wait_for(std::chrono::seconds(0));
		if (status == std::future_status::ready) {
			std::cout << "Changed Weights" << std::endl;
			TheBestWeightsSoFar = NN.Population[0].Chromosome;
			NewNN.SetWeights(TheBestWeightsSoFar);
			/*
			std::ofstream WeightsFile;
			WeightsFile.open("weights.txt", std::ios::trunc);

			for (int i = 0; i < NN.Population.size(); i++)
			{
				for (int j = 0; j < NN.Population[i].Chromosome.size(); j++)
				{
					WeightsFile << NN.Population[i].Chromosome[j] << " ";
				}
			}
			WeightsFile.close();

			*/


			f = std::async(Learn, &NN, HandWrittenDigits);
		}

		NewNN.SetInput(PixelsToinputForNN(pixels));
		NewNN.Evaluate();
		std::vector <double> Output;
		Output = NewNN.Output;
		int answer = 0;
		double thegreatest = 0;
		for (int j = 0; j < 10; j++)
		{
			if (Output[j] > thegreatest)
			{
				thegreatest = Output[j];
				answer = j;
			}
		} //Checking the answer

		std::string digit;
		if (answer == 0) digit = "0";
		if (answer == 1) digit = "1";
		if (answer == 2) digit = "2";
		if (answer == 3) digit = "3";
		if (answer == 4) digit = "4";
		if (answer == 5) digit = "5";
		if (answer == 6) digit = "6";
		if (answer == 7) digit = "7";
		if (answer == 8) digit = "8";
		if (answer == 9) digit = "9";
		text.setString(digit);



		window.clear();
		//Drawing pixels
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(20, 20));
		rect.setFillColor(sf::Color::Red);
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (pixels[i * 4 + j * 16 * 4] == 0)
				{
					rect.setPosition(20 * i, 20 * j);
					window.draw(rect);
				}

			}
		}
		///////////////////////////////////////
		gui.draw();
		window.draw(text);
		window.display();
	}

	return 0;
}

void Learn(NeuralNetwork *NN, std::vector<std::vector<std::vector <double>>> HandWrittenDigits)
{
	double fitness = NN->Learn();
	//if (fitness < 0.00000001) break;

	int howmanyclassifiedcorrectly = 0;
	for (int i = 0; i < HandWrittenDigits.size(); i++)
	{
		NN->SetInput(HandWrittenDigits[i][0]);
		NN->Evaluate();


		std::vector <double> Output;
		Output = NN->Output;
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

std::vector<double> PixelsToinputForNN(std::vector<sf::Uint8> Pixels)
{
	std::vector <double> Input;

	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (Pixels[x * 4 + y * 16 * 4] == 255) Input.push_back(0);
			else Input.push_back(1);
		}

	}
	return Input;
}