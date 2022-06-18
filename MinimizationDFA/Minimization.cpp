#include "Minimization.h"
#include <queue>

void Minimization::Minimize(DFA& DFA)
{
	Minimization minimization;
	minimization.TableFillingMethod(DFA);
}

void Minimization::TableFillingMethod(DFA& otherDFA)
{
	dfa = otherDFA;

	FindUnreachableStates();

	if (!unreachableStates.empty())
	{
		std::cout << "Unreachable states: ";
		for (const auto& state : unreachableStates)
			std::cout << state << ", ";
		std::cout << "\b\b \n";

		RemoveUnreachableStates();
		std::cout << "Removed unreachable states: \n";
		dfa.Print(std::cout);
	}

	if (dfa.GetStates().size() * dfa.GetSymbols().size() != dfa.GetTransitionTable().size())
	{
		AddTrapState();
		std::cout << "The DFA after adding the trap state: \n";
		dfa.Print(std::cout);
	}

	ConstructPairTable();
	std::cout << "The table of unmarked pairs: \n";
	PrintPairTable();

	std::cout << "The table of marked pairs: \n";
	MarkPairs();

	ConstructEquivalenceClasses();
	std::cout << "Equivalence classes: \n";
	PrintEquivalenceClasses();

	otherDFA = std::move(GetNewDFA());
}

void Minimization::FindUnreachableStates()
{
	for (const auto& state : dfa.GetStates())
		unreachableStates.insert(state);
	std::unordered_set<DFA::State> reachableStates;
	std::queue<DFA::State> queue;

	queue.push(dfa.GetInitialState());
	reachableStates.insert(dfa.GetInitialState());

	while (!queue.empty())
	{
		DFA::State currState = queue.front();
		queue.pop();
		unreachableStates.erase(currState);

		for (const auto& symbol : dfa.GetSymbols())
		{
			DFA::State nextState = dfa.GetTransition(currState, symbol);
			if (reachableStates.find(nextState) == reachableStates.end())
			{
				queue.push(nextState);
				reachableStates.insert(nextState);
			}
		}
	}
}

void Minimization::RemoveUnreachableStates()
{
	for (const auto& state : unreachableStates)
	{
		dfa.RemoveState(state);
		for (const auto& symbol : dfa.GetSymbols())
			dfa.RemoveTransition(state, symbol);
	}
}

void Minimization::AddTrapState()
{
	DFA::State newState = "q" + std::to_string(dfa.GetStates().size());
	dfa.InsertState(newState);

	for (const auto& state : dfa.GetStates())
	{
		for (const auto& symbol : dfa.GetSymbols())
		{
			if (dfa.GetTransition(state, symbol) == "")
			{
				dfa.RemoveTransition(state, symbol);
				dfa.InsertTransition(std::make_pair(state, symbol), newState);
			}
		}
	}
}

void Minimization::ConstructPairTable()
{
	for (const auto& state1 : dfa.GetStates())
		for (const auto& state2 : dfa.GetStates())
			if (!StateComparator()(state1, state2) && state1 != state2)
			{
				std::pair<DFA::State, DFA::State> pairOfStates = { state1, state2 };
				pairTable.insert(make_pair(move(pairOfStates), 0));
			}
}

void Minimization::PrintPairTable()
{
	for (const auto& state1 : dfa.GetStates())
	{
		for (const auto& state2 : dfa.GetStates())
			if (!StateComparator()(state1, state2) && state1 != state2)
				std::cout << "(" << state1 << state2 << ", " << pairTable.at(make_pair(state1, state2)) << ") ";

		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Minimization::MarkPairs()
{
	for (const auto& state1 : dfa.GetStates())
	{
		for (const auto& state2 : dfa.GetStates())
		{
			if (!StateComparator()(state1, state2) && state1 != state2)
			{
				if (dfa.GetFinalStates().find(state1) != dfa.GetFinalStates().end() &&
					dfa.GetFinalStates().find(state2) == dfa.GetFinalStates().end() or
					dfa.GetFinalStates().find(state1) == dfa.GetFinalStates().end() &&
					dfa.GetFinalStates().find(state2) != dfa.GetFinalStates().end())
					pairTable[{state1, state2}] = 1;
			}
		}
	}

	std::cout << "The table after marking all of (P,F) pairs: \n";
	PrintPairTable();

	bool isModified;
	size_t numberOfIterations = 0;
	do
	{
		isModified = false;

		for (const auto& state1 : dfa.GetStates())
			for (const auto& state2 : dfa.GetStates())
				if (!StateComparator()(state1, state2) && state1 != state2 && !pairTable.at({ state1, state2 }))
				{
					for (const auto& symbol : dfa.GetSymbols())
					{
						DFA::State nextState1 = dfa.GetTransition(state1, symbol);
						DFA::State nextState2 = dfa.GetTransition(state2, symbol);

						if (StateComparator()(nextState1, nextState2))
							swap(nextState1, nextState2);

						if (nextState1 != nextState2 && pairTable.at({ nextState1, nextState2 }))
						{
							pairTable[{state1, state2}] = 1;
							isModified = true;
							break;
						}
					}
				}

		std::cout << "Iteration " << ++numberOfIterations << ": \n";
		PrintPairTable();

	} while (isModified == true);
}

void Minimization::ConstructEquivalenceClasses()
{
	size_t count = 0;

	for (const auto& state2 : dfa.GetStates())
	{
		std::unordered_set<DFA::State> temp;
		for (const auto& state1 : dfa.GetStates())
		{
			if (!StateComparator()(state1, state2) && state1 != state2 && !pairTable.at({ state1, state2 }))
			{
				temp.insert(state1);
				temp.insert(state2);
			}
		}

		if (temp.empty())
		{
			temp.insert(state2);
		}

		DFA::State newState = "q" + std::to_string(count);
		bool ok = true;
		for (const auto& state : temp)
		{
			if (newStates.find(state) != newStates.end())
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			++count;
			for (const auto& state : temp)
				newStates.insert(make_pair(state, newState));
		}

	}
}

void Minimization::PrintEquivalenceClasses()
{
	for (const auto& newState : newStates)
	{
		std::cout << newState.key << " " << newState.value << std::endl;
		equivalenceClasses[newState.value].insert(newState.key);
	}

	for (const auto& equivalenceClass : equivalenceClasses)
	{
		std::cout << "{";
		for (const auto& state : equivalenceClass.value)
		{
			std::cout << state << ", ";
		}
		std::cout << "\b\b} ";
	}
	std::cout << std::endl << std::endl;
}

DFA Minimization::GetNewDFA()
{
	DFA::States newStates;
	DFA::TransitionTable newTransitionTable;
	DFA::State newInitialState;
	DFA::States newFinalStates;

	for (const auto& equivalenceClass : Minimization::newStates)
	{
		DFA::State oldState = equivalenceClass.key;
		DFA::State newState = equivalenceClass.value;
		newStates.insert(newState);
		if (oldState == dfa.GetInitialState())
			newInitialState = newState;
		if (dfa.GetFinalStates().find(oldState) != dfa.GetFinalStates().end())
			newFinalStates.insert(newState);

		for (const auto& symbol : dfa.GetSymbols())
		{
			DFA::State nextState = dfa.GetTransition(oldState, symbol);
			newTransitionTable[{newState, symbol}] = Minimization::newStates.at(nextState);
		}
	}

	DFA minimizedDFA(newStates, dfa.GetSymbols(), newTransitionTable, newInitialState, newFinalStates);
	return minimizedDFA;
}
