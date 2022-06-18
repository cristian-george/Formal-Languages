#include <fstream>
#include "DFA.h"
#include "NFA.h"

int main()
{
	std::ifstream fin("nfa_elements.txt");
	NFA NFA;
	fin >> NFA;
	fin.close();

	if (NFA.Verify())
	{
		std::cout << "OK ! \n";

		NFA.Print(std::cout);
		DFA DFA = NFA::ConvertToDFA(NFA);

		std::ofstream fout("dfa_elements.txt");
		fout << DFA;
		fout.close();

		std::cout << "Converted NFA to DFA: \n\n";
		DFA.Print(std::cout);

		bool option;
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