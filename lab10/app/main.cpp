#include <iostream>
#include "equationsolver.h"

int main()
{
	std::cout << "Enter quadratic equation coefficients" << std::endl;
	std::cout << "< ";
	double a, b, c;
	std::cin >> a >> b >> c;

	double root1;
	double root2;
	const bool hasRoots = GetQuadraticEquationRoots(a, b, c, root1, root2);
	if (hasRoots)
	{
		std::cout << root1 << std::endl;
		std::cout << root2 << std::endl;
	}
	else
	{
		std::cout << "No solution" << std::endl;
	}

	return EXIT_SUCCESS;
}
