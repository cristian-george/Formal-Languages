#include "DFA.h"
#include <fstream>

int main()
{
	std::ifstream fin("elements.txt");
	DFA DFA;
	fin >> DFA;
	if (DFA.Verify())
	{
		std::cout << "OK ! \n";
		DFA.Print(std::cout);
		std::cout << "M = " << DFA;

		bool option;
		do
		{
			std::cout << "Testing word...... 1 \n";
			std::cout << "Exit.............. 0 \n";
			std::cout << "Choose your option: ";
			std::cin >> option;
			switch (option)
			{
			case 0:
				break;
			case 1:
				std::string word;
				std::cout << "Write a word: ";
				std::cin >> word;
				switch (DFA.Accepts(word))
				{
				case -1:
					std::cout << "The DFA has frozen...\n";
					break;
				case 0:
					std::cout << "Word is not accepted ! \n";
					break;
				case 1:
					std::cout << "Word accepted ! \n";
				}
				break;
			}
			std::cout << "\n\n";

		} while (option != 0);
	}
	else
		std::cout << "NOT OK !\n";

	return 0;
}