#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Hash
{
public:
	template<class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

class DFA
{
public:
	using State = std::string;
	using Symbol = char;
	using TransitionTable = std::unordered_map<std::pair<State, Symbol>, State, Hash>;

public:
	DFA() = default;

	bool Verify();
	size_t Accepts(const std::string&);
	friend std::istream& operator>>(std::istream&, DFA&);
	friend std::ostream& operator<<(std::ostream&, DFA&);
	void Print(std::ostream&);

	const std::unordered_set<State>& GetStates() const;
	const std::unordered_set<Symbol>& GetSymbols() const;
	const TransitionTable& GetTransitionTable() const;
	const State& GetInitialState() const;
	const std::unordered_set<State>& GetFinalStates() const;

	void InsertState(const State&);
	void InsertSymbol(const Symbol);
	void InsertTransition(const std::pair<State, Symbol>&, const State&);
	void InsertFinalState(const State&);

private:
	const State& GetTransition(const State& state, Symbol symbol);

private:
	std::unordered_set<State> states;
	std::unordered_set<Symbol> symbols;
	TransitionTable transitionTable;
	State initialState;
	std::unordered_set<State> finalStates;
};