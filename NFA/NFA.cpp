#include "NFA.h"
#include <iomanip>

bool NFA::Verify()
{
	if (states.find(initialState) == states.end())
		return false;

	for (const auto& finalState : finalStates)
		if (states.find(finalState) == states.end())
			return false;

	for (const auto& transition : transitionTable)
	{
		if (states.find(transition.key.first) == states.end())
			return false;
		if (symbols.find(transition.key.second) == symbols.end())
			return false;

		for (const auto& state : transition.value)
			if (states.find(state) == states.end())
				return false;
	}

	return true;
}

void NFA::Accepts(State currState, const std::string& word, size_t index)
{
	if (word.size() == index)
	{
		std::cout << "(" << currState << ", *) -> ";
		if (finalStates.find(currState) != finalStates.end())
		{
			throw "The word is accepted ! \n";
		}

		std::cout << "The word is not accepted ! \n";
		return;
	}
	else
	{
		std::cout << "(" << currState << ", " << word.substr(index) << ") -> ";

		const auto& transition = GetTransition(currState, word[index]);
		if (transition.empty())
		{
			std::cout << "The NFA has frozen...\n";
			return;
		}

		for (auto state : transition)
		{
			Accepts(state, word, index + 1);
		}
	}
}

const std::unordered_set<NFA::State>& NFA::GetStates() const
{
	return states;
}

const std::unordered_set<NFA::Symbol>& NFA::GetSymbols() const
{
	return symbols;
}

const NFA::TransitionTable& NFA::GetTransitionTable() const
{
	return transitionTable;
}

const std::vector<NFA::State>& NFA::GetTransition(const State& state, Symbol symbol)
{
	return transitionTable[make_pair(state, symbol)];
}

const NFA::State& NFA::GetInitialState() const
{
	return initialState;
}

const std::unordered_set<NFA::State>& NFA::GetFinalStates() const
{
	return finalStates;
}

void NFA::InsertState(const State& state)
{
	states.insert(state);
}

void NFA::InsertSymbol(const Symbol symbol)
{
	symbols.insert(symbol);
}

void NFA::InsertTransition(const std::pair<State, Symbol>& pair, const State& state)
{
	transitionTable[pair].push_back(state);
}

void NFA::InsertFinalState(const State& finalState)
{
	finalStates.insert(finalState);
}

std::istream& operator>>(std::istream& in, NFA& obj)
{
	size_t numberOfStates;
	in >> numberOfStates;
	for (size_t index = 0; index < numberOfStates; ++index)
	{
		NFA::State state;
		in >> state;
		obj.InsertState(state);
	}

	size_t numberOfTransitions;
	in >> numberOfTransitions;
	for (size_t index = 0; index < numberOfTransitions; ++index)
	{
		NFA::State state, state2;
		NFA::Symbol symbol;
		in >> state >> symbol >> state2;
		obj.InsertTransition(make_pair(state, symbol), state2);
		obj.InsertSymbol(symbol);
	}

	in >> obj.initialState;

	size_t numberOfFinalStates;
	in >> numberOfFinalStates;
	for (size_t index = 0; index < numberOfFinalStates; ++index)
	{
		NFA::State state;
		in >> state;
		obj.InsertFinalState(state);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, NFA& obj)
{
	out << "M = ({";
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
		std::pair<NFA::State, NFA::Symbol> pair(transition.key.first, transition.key.second);
		out << "	(" << pair.first << ", " << pair.second << ") -> {";
		for (const auto& state : transition.value)
			out << state << ", ";
		out << "\b\b}\n";
	}
	out << "    }\n\n";

	return out;
}

void NFA::Print(std::ostream& out)
{
	std::unordered_map<Symbol, size_t> width;
	for (const auto& symbol : GetSymbols())
	{
		width[symbol] = 0;
		for (const auto& state : GetStates())
		{
			const auto& transition = GetTransition(state, symbol);
			if (width[symbol] < transition.size())
				width[symbol] = transition.size();
		}
	}

	out << "      || ";
	for (const auto& symbol : GetSymbols())
	{
		out << symbol << "  ";
		for (size_t index = 1; index < width[symbol]; ++index)
			out << "   ";
		out << "  ";
	}
	out << std::endl;

	out << "------++-";
	for (const auto& symbol : GetSymbols())
	{
		for (size_t index = 0; index < width[symbol]; ++index)
			out << "---";
		out << "--";
	}
	out << "\b \b\b \n";

	for (const auto& currState : GetStates())
	{
		if (currState == GetInitialState())
			out << "-> ";
		else
			if (GetFinalStates().find(currState) != GetFinalStates().end())
				out << " * ";
			else
				out << "   ";

		out << currState << " || ";

		for (const auto& symbol : GetSymbols())
		{
			const auto& it = GetTransition(currState, symbol);
			for (size_t index = 0; index < width[symbol]; ++index)
				if (index < it.size())
					out << it[index] << " ";
				else
					out << "   ";
			out << "  ";
		}
		out << std::endl;
	}
	out << std::endl;
}
