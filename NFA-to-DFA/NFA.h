#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "DFA.h"
#define key first
#define value second

class NFA
{
	using State = std::string;
	using Symbol = char;
	using TransitionTable = std::unordered_map<std::pair<State, Symbol>, std::vector<State>, Hash>;
public:
	NFA() = default;

	bool Verify();
	friend std::istream& operator>>(std::istream&, NFA&);
	friend std::ostream& operator<<(std::ostream&, NFA&);

	const std::unordered_set<State>& GetStates() const;
	const std::unordered_set<Symbol>& GetSymbols() const;
	const TransitionTable& GetTransitionTable() const;
	const std::vector<State>& GetTransitions(const State&, Symbol);
	const State& GetInitialState() const;
	const std::unordered_set<State>& GetFinalStates() const;

	void InsertState(const State&);
	void InsertSymbol(const Symbol);
	void InsertTransition(const std::pair<State, Symbol>&, const State&);
	void InsertFinalState(const State&);

	void Print(std::ostream&);
	static DFA ConvertToDFA(NFA NFA);
	DFA Operations();

private:
	std::unordered_set<State> GetTransition(const std::unordered_set<State>&, Symbol);
	State GetNextState(const std::unordered_map<State, std::unordered_set<State>>&, const std::unordered_set<State>&);

private:
	std::unordered_set<State> states;
	std::unordered_set<Symbol> symbols;
	TransitionTable transitionTable;
	State initialState;
	std::unordered_set<State> finalStates;
};

