#pragma once
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#define key first
#define value second

class Hash
{
public:
	template<class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) + std::hash<T2>()(pair.second);
	}
};

class NFA
{
	using State = std::string;
	using Symbol = char;
	using TransitionTable = std::unordered_map<std::pair<State, Symbol>, std::vector<State>, Hash>;
public:
	NFA() = default;

	bool Verify();
	void Accepts(State, const std::string&, size_t);
	friend std::istream& operator>>(std::istream&, NFA&);
	friend std::ostream& operator<<(std::ostream&, NFA&);

	const std::unordered_set<State>& GetStates() const;
	const std::unordered_set<Symbol>& GetSymbols() const;
	const TransitionTable& GetTransitionTable() const;
	const std::vector<State>& GetTransition(const State&, Symbol);
	const State& GetInitialState() const;
	const std::unordered_set<State>& GetFinalStates() const;

	void InsertState(const State&);
	void InsertSymbol(const Symbol);
	void InsertTransition(const std::pair<State, Symbol>&, const State&);
	void InsertFinalState(const State&);

	void Print(std::ostream&);

private:
	std::unordered_set<State> states;
	std::unordered_set<Symbol> symbols;
	TransitionTable transitionTable;
	State initialState;
	std::unordered_set<State> finalStates;
};

