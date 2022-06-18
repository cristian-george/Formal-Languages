#include "DFA.h"
#include "Minimization.h"

DFA::DFA(const States& states,
	const std::set<Symbol>& symbols,
	const TransitionTable& transitionTable,
	const State& initialState,
	const States& finalStates) :
	states(states), symbols(symbols), transitionTable(transitionTable), initialState(initialState), finalStates(finalStates) {}

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
		if (GetStates().find(transition.second) == GetStates().end() && transition.second != "-")
			return false;
	}

	return true;
}

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

	size_t numberOfSymbols;
	in >> numberOfSymbols;
	for (size_t index = 0; index < numberOfSymbols; ++index)
	{
		DFA::Symbol symbol;
		in >> symbol;
		obj.InsertSymbol(symbol);
	}

	size_t numberOfTransitions;
	in >> numberOfTransitions;

	for (size_t index = 0; index < numberOfTransitions; ++index)
	{
		DFA::State state, state2;
		DFA::Symbol symbol;
		in >> state >> symbol >> state2;
		obj.InsertTransition(make_pair(state, symbol), state2);
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
	out << obj.GetStates().size() << std::endl;
	for (const auto& state : obj.GetStates())
		out << state << " ";

	out << std::endl << obj.GetSymbols().size() << std::endl;
	for (const auto& symbol : obj.GetSymbols())
		out << symbol << " ";

	out << std::endl << obj.GetTransitionTable().size() << std::endl;
	for (const auto& state : obj.GetStates())
	{
		for (const auto& symbol : obj.GetSymbols())
			out << state << " " << symbol << " " << obj.GetTransition(state, symbol) << std::endl;
	}

	out << obj.GetInitialState() << std::endl;

	out << obj.GetFinalStates().size() << std::endl;
	for (const auto& finalState : obj.GetFinalStates())
		out << finalState << " ";
	return out;
}

const DFA::States& DFA::GetStates() const
{
	return states;
}

const std::set<DFA::Symbol>& DFA::GetSymbols() const
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

const DFA::States& DFA::GetFinalStates() const
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

void DFA::RemoveState(const State& state)
{
	states.erase(state);
}

void DFA::RemoveTransition(const State& state, Symbol symbol)
{
	transitionTable.erase(make_pair(state, symbol));
}

void DFA::Minimize(DFA& DFA)
{
	Minimization minimization;
	minimization.TableFillingMethod(DFA);
}
