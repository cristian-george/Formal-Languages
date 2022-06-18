#include "NFA.h"
#include <queue>
#include <set>

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

const std::vector<NFA::State>& NFA::GetTransitions(const State& state, Symbol symbol)
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

	while (numberOfTransitions)
	{
		std::string buffer;
		getline(in >> std::ws, buffer);

		DFA::State currState = buffer.substr(0, 2), nextState;
		DFA::Symbol symbol = buffer[3];

		buffer = buffer.substr(5);

		size_t first = 0;
		while (first < buffer.size())
		{
			size_t second = buffer.find_first_of(' ', first);

			if (second == std::string::npos)
				second = buffer.size();

			nextState = buffer.substr(first, second - first);
			first = second + 1;

			obj.InsertTransition(make_pair(currState, symbol), nextState);
			numberOfTransitions--;
		}
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
			const auto& transition = GetTransitions(state, symbol);
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
			const auto& it = GetTransitions(currState, symbol);
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

DFA NFA::ConvertToDFA(NFA NFA)
{
	DFA DFA = NFA.Operations();
	return DFA;
}

DFA NFA::Operations()
{
	std::queue<std::unordered_set<State>> queue;
	queue.push({ GetInitialState() });

	size_t count = 0;
	std::unordered_map<State, std::unordered_set<State>> visited;
	visited.insert(make_pair("q" + std::to_string(count++), queue.front()));

	while (!queue.empty())
	{
		std::unordered_set<State> currState = queue.front();
		queue.pop();

		for (const auto& symbol : GetSymbols())
		{
			std::unordered_set<State> nextState = GetTransition(currState, symbol);
			if (!nextState.empty() && GetNextState(visited, nextState) == "")
			{
				queue.push(nextState);
				visited.insert(make_pair("q" + std::to_string(count++), nextState));
			}
		}
	}

	for (const auto& newState : visited)
	{
		std::cout << newState.key << ": {";
		for (const auto& it : newState.value)
		{
			std::cout << it << ", ";
		}
		std::cout << "\b\b} \n";
	}
	std::cout << std::endl;

	DFA DFA;
	DFA.SetInitialState(initialState);

	for (const auto& symbol : GetSymbols())
	{
		DFA.InsertSymbol(symbol);
	}

	for (const auto& newState : visited)
	{
		DFA.InsertState(newState.key);

		for (const auto& finalState : GetFinalStates())
		{
			if (newState.value.find(finalState) != newState.value.end())
			{
				DFA.InsertFinalState(newState.key);
				break;
			}
		}

		for (const auto& symbol : GetSymbols())
		{
			State nextState = GetNextState(visited, GetTransition(newState.value, symbol));
			if (nextState != "")
				DFA.InsertTransition(make_pair(newState.key, symbol), nextState);
		}
	}

	return DFA;
}

std::unordered_set<NFA::State> NFA::GetTransition(const std::unordered_set<State>& currState, Symbol symbol)
{
	std::unordered_set<State> nextState;

	for (const auto& state : currState)
	{
		const auto& transitions = GetTransitions(state, symbol);
		for (const auto& transition : transitions)
			nextState.insert(transition);
	}

	return nextState;
}

NFA::State NFA::GetNextState(const std::unordered_map<State, std::unordered_set<State>>& visited, const std::unordered_set<State>& nextState)
{
	for (const auto& state : visited)
	{
		if (state.value == nextState)
			return state.key;
	}
	return "";
}
