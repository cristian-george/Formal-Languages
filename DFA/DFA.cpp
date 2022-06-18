#include "DFA.h"

bool DFA::Verify()
{
	if (GetStates().find(GetInitialState()) == GetStates().end())
		return false;

	for (const auto& finalState : GetFinalStates())
		if (GetStates().find(finalState) == GetStates().end())
			return false;

	for (const auto& transition : GetTransitionTable())
	{
		if (GetStates().find(transition.first.first) == GetStates().end())
			return false;
		if (GetSymbols().find(transition.first.second) == GetSymbols().end())
			return false;
		if (GetStates().find(transition.second) == GetStates().end())
			return false;
	}

	return true;
}

// O(l), l = word.length()
size_t DFA::Accepts(const std::string& word)
{
	State currState = GetInitialState();
	for (const auto& character : word)
	{
		currState = GetTransition(currState, character);
		if (currState == "")
			return -1;
	}

	if (GetFinalStates().find(currState) == GetFinalStates().end())
		return 0;
	return 1;
}

void DFA::Print(std::ostream& out)
{
	out << "      || ";
	for (const auto& symbol : GetSymbols())
		out << symbol << "   ";

	out << std::endl;

	out << "------++";
	for (size_t index = 0; index < GetSymbols().size(); ++index)
		out << "----";
	out << std::endl;

	for (const auto& currState : GetStates())
	{
		if (currState == GetInitialState())
			out << "-> ";
		else
			if (GetFinalStates().find(currState) != GetFinalStates().end())
				out << " * ";
			else
				out << "   ";

		out << currState << " ||";

		for (const auto& symbol : GetSymbols())
		{
			const auto& it = GetTransitionTable().find(std::make_pair(currState, symbol));
			if (it != GetTransitionTable().end())
				out << " " << it->second << " ";
			else
				out << "    ";
		}
		out << std::endl;
	}
	out << std::endl;
}

const DFA::State& DFA::GetTransition(const State& state, Symbol symbol)
{
	return transitionTable[make_pair(state, symbol)];
}

std::istream& operator>>(std::istream& in, DFA& obj)
{
	size_t numberOfStates;
	in >> numberOfStates;
	for (size_t index = 0; index < numberOfStates; ++index)
	{
		DFA::State state;
		in >> state;
		obj.InsertState(state);
	}

	size_t numberOfTransitions;
	in >> numberOfTransitions;

	for (size_t index = 0; index < numberOfTransitions; ++index)
	{
		DFA::State state, state2;
		DFA::Symbol symbol;
		in >> state >> symbol >> state2;
		obj.InsertTransition(make_pair(state, symbol), state2);
		if (obj.GetSymbols().find(symbol) == obj.GetSymbols().end())
			obj.InsertSymbol(symbol);
	}

	in >> obj.initialState;

	size_t numberOfFinalStates;
	in >> numberOfFinalStates;
	for (size_t index = 0; index < numberOfFinalStates; ++index)
	{
		DFA::State state;
		in >> state;
		obj.InsertFinalState(state);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, DFA& obj)
{
	out << "({";
	for (const auto& state : obj.GetStates())
		out << state << ", ";
	out << "\b\b}, {";
	for (const auto& symbol : obj.GetSymbols())
		out << symbol << ", ";
	out << "\b\b}, d, " << obj.GetInitialState() << ", {";
	for (const auto& finalState : obj.GetFinalStates())
		out << finalState << ", ";
	out << "\b\b})\nd = {\n";
	for (const auto& transition : obj.GetTransitionTable())
	{
		std::pair<DFA::State, DFA::Symbol> key(transition.first.first, transition.first.second);
		out << "	(" << key.first << ", " << key.second << ") -> " << transition.second << "\n";
	}
	out << "    }\n\n";

	return out;
}

const std::unordered_set<DFA::State>& DFA::GetStates() const
{
	return states;
}

const std::unordered_set<DFA::Symbol>& DFA::GetSymbols() const
{
	return symbols;
}

const DFA::TransitionTable& DFA::GetTransitionTable() const
{
	return transitionTable;
}

const DFA::State& DFA::GetInitialState() const
{
	return initialState;
}

const std::unordered_set<DFA::State>& DFA::GetFinalStates() const
{
	return finalStates;
}

void DFA::InsertState(const State& state)
{
	states.insert(state);
}

void DFA::InsertSymbol(const Symbol symbol)
{
	symbols.insert(symbol);
}

void DFA::InsertTransition(const std::pair<State, Symbol>& key, const State& value)
{
	transitionTable.insert(make_pair(key, value));
}

void DFA::InsertFinalState(const State& state)
{
	finalStates.insert(state);
}