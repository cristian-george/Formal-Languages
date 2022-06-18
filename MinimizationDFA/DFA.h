#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#define key first
#define value second

class Hash
{
public:
	template<class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

class StateComparator
{
public:
	template<class T>
	bool operator()(const T& state1, const T& state2) const
	{
		size_t number1 = std::stoi(state1.substr(1));
		size_t number2 = std::stoi(state2.substr(1));

		return number1 < number2;
	}
};

class DFA
{
public:
	using State = std::string;
	using States = std::set<State, StateComparator>;
	using Symbol = char;
	using TransitionTable = std::unordered_map<std::pair<State, Symbol>, State, Hash>;

public:
	DFA() = default;
	DFA(const States&,
		const std::set<Symbol>&,
		const TransitionTable&,
		const State&,
		const States&);

	bool Verify();
	size_t Accepts(const std::string&);
	friend std::istream& operator>>(std::istream&, DFA&);
	friend std::ostream& operator<<(std::ostream&, DFA&);
	void Print(std::ostream&);

	const States& GetStates() const;
	const std::set<Symbol>& GetSymbols() const;
	const TransitionTable& GetTransitionTable() const;
	const State& GetTransition(const State&, Symbol);
	const State& GetInitialState() const;
	const States& GetFinalStates() const;

	void InsertState(const State&);
	void InsertSymbol(const Symbol);
	void InsertTransition(const std::pair<State, Symbol>&, const State&);
	void InsertFinalState(const State&);

	void RemoveState(const State&);
	void RemoveTransition(const State&, Symbol);

	static void Minimize(DFA&);

private:
	States states;
	std::set<Symbol> symbols;
	TransitionTable transitionTable;
	State initialState;
	States finalStates;
};