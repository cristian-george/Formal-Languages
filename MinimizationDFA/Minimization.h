#pragma once
#include "DFA.h"
#include <unordered_set>

class Minimization
{
public:
	using PairTable = std::unordered_map<std::pair<DFA::State, DFA::State>, bool, Hash>;
	using EquivalenceClasses = std::unordered_map<DFA::State, std::unordered_set<DFA::State>>;

public:
	Minimization() = default;

	void TableFillingMethod(DFA&);
	static void Minimize(DFA&);

private:
	void FindUnreachableStates();
	void RemoveUnreachableStates();

	void AddTrapState();

	void ConstructPairTable();
	void PrintPairTable();
	void MarkPairs();

	void ConstructEquivalenceClasses();
	void PrintEquivalenceClasses();

	DFA GetNewDFA();

private:
	DFA dfa;
	std::unordered_set<DFA::State> unreachableStates;
	PairTable pairTable;
	std::unordered_map<DFA::State, DFA::State> newStates;
	EquivalenceClasses equivalenceClasses;
};

