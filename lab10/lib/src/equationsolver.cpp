#include <cmath>
#include "../include/equationsolver.h"

bool GetQuadraticEquationRoots(double a, double b, double c, double& root1, double& root2)
{
	double discriminant = b * b - 4 * a * c;

	if (discriminant > 0)
	{
		root1 = (-b + sqrt(discriminant)) / (2 * a);
		root2 = (-b - sqrt(discriminant)) / (2 * a);
		return true;
	}

	if (discriminant == 0)
	{
		root1 = root2 = -b / (2 * a);
		return true;
	}

	return false;
}