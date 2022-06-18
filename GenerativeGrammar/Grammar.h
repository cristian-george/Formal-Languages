#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "ProductionRule.h"

constexpr auto lambda = "*";

class Grammar
{
public:
	using Symbol = char;
	using ProductionRules = std::unordered_map<size_t, ProductionRule>;
public:
	Grammar() = default;

	const std::unordered_set<Symbol>& GetNonterminalSymbols() const;
	const std::unordered_set<Symbol>& GetTerminalSymbols() const;
	Symbol GetStartSymbol() const;
	const ProductionRules& GetProductionRules() const;

	void InsertNonterminalSymbol(Symbol);
	void InsertTerminalSymbol(Symbol);
	void SetStartSymbol(Symbol);
	void InsertProductionRule(size_t, const ProductionRule&);

public:
	bool Verify() const;
	void GenerateWord(bool);

public:
	friend std::istream& operator>>(std::istream&, Grammar&);
	friend std::ostream& operator<<(std::ostream&, const Grammar&);

private:
	std::unordered_set<Symbol> nonterminalSymbols;
	std::unordered_set<Symbol> terminalSymbols;
	Symbol startSymbol;
	ProductionRules productionRules;

private:
	int Randomizer(unsigned int, unsigned int);
	std::vector<size_t> ProductionRulesToApply(const std::string&);
	bool ContainsOnlyTerminalSymbols(const std::string&) const;

};