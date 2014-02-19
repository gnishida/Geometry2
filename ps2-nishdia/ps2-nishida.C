#include <vector>
#include <iostream>
#include "point.h"
#include "acp.h"
#include "arrange.h"
#include "circle.h"

using namespace std;

int main(int argc, char *argv[]) {
	Parameter::enable();

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

	// setup the arrangement
	Arrangement arr(false);
	for (int i = 0; i < centers.size(); ++i) {
		arr.addCircle(centers[i], radii[i]);
	}

	// compute the overlay of all the circles
	arr.computePS2();

	// show the results
	std::cout << "Vertices: " << arr.vertices.size() << std::endl;
	std::cout << "Edges: " << arr.edges.size() / 2 << std::endl;
	std::cout << "Faces: " << arr.faces.size() << std::endl;
	std::cout << "Components: " << arr.components.size() << std::endl;
	std::cout << "V - E + F = " << arr.vertices.size() - arr.edges.size() / 2  + arr.faces.size() << std::endl;
	if (arr.vertices.size() - arr.edges.size() / 2  + arr.faces.size() == 1 + arr.components.size()) {
		std::cout << "Correctness was verified." << std::endl;
	} else {
		std::cout << "Correctness was not verified." << std::endl;
	}

	return 0;
}
