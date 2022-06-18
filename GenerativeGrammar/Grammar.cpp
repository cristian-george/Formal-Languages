#include "Grammar.h"
#include <random>
#include <chrono>
#include <unordered_set>

bool Grammar::Verify() const
{
	//(1) {VN} si {VT} multimi disjuncte
	for (const auto& nonterminalSymbol : GetNonterminalSymbols())
		if (GetTerminalSymbols().find(nonterminalSymbol) != GetTerminalSymbols().end())
			return false;


	//(2) S apartine {VN}
	if (GetNonterminalSymbols().find(startSymbol) == GetNonterminalSymbols().end())
		return false;


	//(3) Pentru fiecare productie, membrul stang contine cel putin un neterminal
	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftHandSide = productionRule.second.GetLeftHandSide();
		bool ok3 = false;

		for (const auto& nonterminalSymbol : GetNonterminalSymbols())
		{
			if (leftHandSide.find(nonterminalSymbol) != std::string::npos)
				ok3 = true;
		}

		if (ok3 == false) return false;
	}


	//(4) Exista cel putin o productie care are in stanga doar S
	bool ok4 = false;

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftHandSide = productionRule.second.GetLeftHandSide();

		if (leftHandSide.find(startSymbol) != std::string::npos && leftHandSide.size() == 1)
		{
			ok4 = true;
			break;
		}
	}
	if (ok4 == false) return false;


	//(5) Fiecare productie contine doar elemente din {VN} si {VT}
	std::unordered_set<Symbol> allSymbols(GetNonterminalSymbols());
	for (const auto& terminalSymbol : GetTerminalSymbols())
		allSymbols.insert(terminalSymbol);

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftHandSide = productionRule.second.GetLeftHandSide();
		const auto& righHandSide = productionRule.second.GetRightHandSide();

		for (const auto& symbol : leftHandSide)
			if (allSymbols.find(symbol) == allSymbols.end())
				return false;

		for (const auto& symbol : righHandSide)
			if (allSymbols.find(symbol) == allSymbols.end() && symbol != lambda[0])
				return false;
	}

	return true;
}

int Grammar::Randomizer(unsigned int min, unsigned int max)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::mt19937_64 gen(seed);
	std::uniform_int_distribution<int> distrib(min, max);

	return distrib(gen);
}

std::vector<size_t> Grammar::ProductionRulesToApply(const std::string& currWord)
{
	std::vector<size_t> productionRulesToApply;

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftHandSide = productionRule.second.GetLeftHandSide();
		if (currWord.find(leftHandSide) != std::string::npos)
			productionRulesToApply.push_back(productionRule.first);
	}

	return productionRulesToApply;
}

bool Grammar::ContainsOnlyTerminalSymbols(const std::string& currWord) const
{
	for (const auto& symbol : currWord)
	{
		if (GetTerminalSymbols().find(symbol) == GetTerminalSymbols().end())
			return false;
	}

	return true;
}

void Grammar::GenerateWord(bool option)
{
	static std::unordered_set<std::string> usedWords;

	std::string steps;
	std::string currWord;
	currWord.push_back(GetStartSymbol());

	while (true)
	{
		if (option == 1)
			steps += currWord;

		std::vector<size_t> productionRulesToApply = ProductionRulesToApply(currWord);

		if (productionRulesToApply.empty())
		{
			if (ContainsOnlyTerminalSymbols(currWord) and usedWords.find(currWord) == usedWords.end())
			{
				usedWords.insert(currWord);
				switch (option)
				{
				case 0:
					std::cout << currWord;
					break;

				case 1:
					std::cout << steps;
				};

				break;
			}
			else
			{
				steps.clear();
				currWord.clear();
				currWord.push_back(GetStartSymbol());
			}
		}
		else
		{
			unsigned int random = Randomizer(0, productionRulesToApply.size() - 1);
			size_t pos = productionRulesToApply[random];

			const auto& leftHandSide = GetProductionRules().at(pos).GetLeftHandSide();
			auto rightHandSide = GetProductionRules().at(pos).GetRightHandSide();
			if (rightHandSide == lambda)
				rightHandSide = "";

			currWord.replace(currWord.find(leftHandSide), leftHandSide.length(), rightHandSide);

			if (option == 1)
				steps += " =(" + std::to_string(pos + 1) + ")=> ";
		}

		productionRulesToApply.clear();
	}
}

const std::unordered_set<Grammar::Symbol>& Grammar::GetNonterminalSymbols() const
{
	return nonterminalSymbols;
}

const std::unordered_set<Grammar::Symbol>& Grammar::GetTerminalSymbols() const
{
	return terminalSymbols;
}

Grammar::Symbol Grammar::GetStartSymbol() const
{
	return startSymbol;
}

const Grammar::ProductionRules& Grammar::GetProductionRules() const
{
	return productionRules;
}

void Grammar::InsertNonterminalSymbol(Symbol symbol)
{
	nonterminalSymbols.insert(symbol);
}

void Grammar::InsertTerminalSymbol(Symbol symbol)
{
	terminalSymbols.insert(symbol);
}

void Grammar::SetStartSymbol(Symbol symbol)
{
	startSymbol = symbol;
}

void Grammar::InsertProductionRule(size_t index, const ProductionRule& productionRule)
{
	productionRules.insert(std::make_pair(index, productionRule));
}

std::istream& operator>>(std::istream& in, Grammar& obj)
{
	size_t numberOfNonterminalSymbols;
	in >> numberOfNonterminalSymbols;
	for (size_t index = 0; index < numberOfNonterminalSymbols; ++index)
	{
		Grammar::Symbol symbol;
		in >> symbol;
		obj.InsertNonterminalSymbol(symbol);
	}

	size_t numberOfTerminalSymbols;
	in >> numberOfTerminalSymbols;
	for (size_t index = 0; index < numberOfTerminalSymbols; ++index)
	{
		Grammar::Symbol symbol;
		in >> symbol;
		obj.InsertTerminalSymbol(symbol);
	}

	in >> obj.startSymbol;

	size_t numberOfProductionRules;
	in >> numberOfProductionRules;

	for (size_t index = 0; index < numberOfProductionRules; ++index)
	{
		ProductionRule productionRule;
		in >> productionRule;
		obj.InsertProductionRule(index, productionRule);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& obj)
{
	out << "G = ({";
	for (const auto& nonterminal : obj.GetNonterminalSymbols())
	{
		out << nonterminal << ", ";
	}
	out << "\b\b}, ";

	out << "{";
	for (const auto& terminal : obj.GetTerminalSymbols())
	{
		out << terminal << ", ";
	}
	out << "\b\b}, ";

	out << obj.GetStartSymbol() << ", {";

	for (const auto& productionRule : obj.GetProductionRules())
	{
		out << productionRule.second << ", ";
	}
	out << "\b\b})";

	return out;
}
