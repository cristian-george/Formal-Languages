#pragma once
#include <iostream>
#include <string>

class ProductionRule
{
public:
	ProductionRule() = default;

public:
	const std::string& GetLeftHandSide() const;
	const std::string& GetRightHandSide() const;

public:
	friend std::istream& operator>>(std::istream&, ProductionRule&);
	friend std::ostream& operator<<(std::ostream&, const ProductionRule&);

private:
	std::string leftHandSide;
	std::string rightHandSide;
};

