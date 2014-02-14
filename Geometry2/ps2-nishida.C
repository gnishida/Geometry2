#include <vector>
#include <iostream>
#include "point.h"
#include "acp.h"
#include "arrange.h"
#include "circle.h"

using namespace std;

int main(int argc, char *argv[]) {
	// read input data from the standard input
	int numCircles;
	std::cin >> numCircles;

	Points centers;
	std::vector<Parameter> radii;

	for (int i = 0; i < numCircles; ++i) {
		double x, y, r;
		std::cin >> x >> y >> r;

		centers.push_back(new InputPoint(x, y));
		radii.push_back(Parameter(r));
	}

	assert(centers.size() >= 2);

	// setup the arrangement
	Arrangement a(false);
	for (int i = 0; i < centers.size(); ++i) {
		a.addCircle(centers[i], radii[i]);
	}

	// compute the overlay
	a.computePS2();

	// show the results
	std::cout << "Vertices: " << a.vertices.size() << std::endl;
	std::cout << "Edges: " << a.edges.size() / 2 << std::endl;
	std::cout << "Faces: " << a.faces.size() << std::endl;
	std::cout << "Components: " << a.components.size() << std::endl;
	std::cout << "V - E + F = " << a.vertices.size() - a.edges.size() / 2  + a.faces.size() << std::endl;
	if (a.vertices.size() - a.edges.size() / 2  + a.faces.size() == 1 + a.components.size()) {
		std::cout << "Correctness was verified." << std::endl;
	} else {
		std::cout << "Correctness was not verified." << std::endl;
	}

	return 0;
}
