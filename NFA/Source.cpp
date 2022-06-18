#include "NFA.h"
#include <fstream>

int main()
{
	std::ifstream fin("elements.txt");
	NFA NFA;
	fin >> NFA;
	if (NFA.Verify())
	{
		std::cout << "OK ! \n";
		std::cout << std::endl;
		NFA.Print(std::cout);

		size_t option;
		do
		{
			std::cout << "Testing word....... 1 \n";
			std::cout << "Exit............... 0 \n";
			std::cout << "Choose your option: ";
			std::cin >> option;
			switch (option)
			{
			case 0:
				break;
			case 1:
				try
				{
					std::string word;
					std::cout << "Write a word: ";
					std::cin >> word;
					NFA.Accepts(NFA.GetInitialState(), word, 0);
				}
				catch (const char* message)
				{
					std::cout << message;
				}
				break;
			}
			std::cout << "\n\n";

		} while (option != 0);
	}
	else
		std::cout << "NOT OK ! \n";
	return 0;
}