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
	Arrangement a;
	Arrangement b;

	a.addCircle(centers[0], radii[0]);
	for (int i = 1; i < centers.size(); ++i) {
		b.addCircle(centers[i], radii[i]);
	}

	// compute the overlay
	Arrangement *o = overlay(&a, &b);

	// show the results
	std::cout << "Vertices: " << o->vertices.size() << std::endl;
	std::cout << "Edges: " << o->edges.size() / 2 << std::endl;
	std::cout << "Faces: " << o->faces.size() << std::endl;
	std::cout << "Components: " << o->numComponents << std::endl;
	std::cout << "V - E + F = " << o->vertices.size() - o->edges.size() / 2  + o->faces.size() << std::endl;
	if (o->vertices.size() - o->edges.size() / 2  + o->faces.size() == 1 + o->numComponents) {
		std::cout << "Correctness was verified." << std::endl;
	} else {
		std::cout << "Correctness was not verified." << std::endl;
	}

	return 0;
}
