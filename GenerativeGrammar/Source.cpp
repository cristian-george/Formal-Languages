#include "Grammar.h"
#include <fstream>

int main()
{
	Grammar grammar;
	std::ifstream fin("elements.txt");
	fin >> grammar;
	std::cout << grammar << std::endl;

	if (grammar.Verify())
	{
		std::cout << "It's a generative grammar !\n";

		size_t numberOfWords;
		std::cout << "Enter number of words : ";
		std::cin >> numberOfWords;

		if (numberOfWords <= 0)
			return 0;

		bool option;
		std::cout << "Print only words................ 0 \n";
		std::cout << "Print the series of choices..... 1 \n";
		std::cout << "Choose your option: ";
		std::cin >> option;

		for (size_t index = 0; index < numberOfWords; ++index)
		{
			std::cout << index + 1 << ". ";
			grammar.GenerateWord(option);
			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "It's not a generative grammar !\n";
	}

	return 0;
}