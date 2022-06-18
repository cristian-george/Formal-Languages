#include "ProductionRule.h"

const std::string& ProductionRule::GetLeftHandSide() const
{
	return leftHandSide;
}

const std::string& ProductionRule::GetRightHandSide() const
{
	return rightHandSide;
}

std::istream& operator>>(std::istream& in, ProductionRule& obj)
{
	in >> obj.leftHandSide >> obj.rightHandSide;
	return in;
}

std::ostream& operator<<(std::ostream& out, const ProductionRule& obj)
{
	out << obj.leftHandSide << " -> " << obj.rightHandSide;
	return out;
}
