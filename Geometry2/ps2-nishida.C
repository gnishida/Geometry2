#include <vector>
#include <iostream>
#include "point.h"
#include "acp.h"
#include "arrange.h"
#include "circle.h"

using namespace std;

int main(int argc, char *argv[]) {
	int numCircles;
	std::cin >> numCircles;

	vector<Circle1pt1rad*> circles;

	for (int i = 0; i < numCircles; ++i) {
		double x, y, r;
		std::cin >> x >> y >> r;

		circles.push_back(new Circle1pt1rad(PV2(x, y), Parameter(r)));
	}

	assert(circles.size() >= 2);

	Arrangement a(true);
	Arrangement b(true);
	Arrangement* o = new Arrangement(true);

	o->addCircle(circles[0]);
	for (int i = 1; i < circles.size(); ++i) {
		Arrangement b(true);
		b.addCircle(circles[i]);

		o = overlay(o, &b);
	}

	/*
	Point* p1 = new InputPoint(0, 0);
	Point* p2 = new InputPoint(10, 0);

	a.addCircle(new Circle1pt1rad(PV2(0, 0), Parameter((double)10)));
	b.addCircle(new Circle1pt1rad(PV2(10, 0), Parameter((double)10)));

	Arrangement* o = overlay(&a, &b);
	*/

	std::cout << "Vertices: " << o->vertices.size() << std::endl;
	std::cout << "Edges: " << o->edges.size() << std::endl;
	std::cout << "Faces: " << o->faces.size() << std::endl;

	/*
	for (int i = 0; i < o->vertices.size(); ++i) {
		std::cout << "Point [" << i << "]: " << o->vertices[i]->p->getP().getX().mid() << "," << o->vertices[i]->p->getP().getY().mid() << std::endl;
	}
	*/

	/*
	for (int i = 0; i < o->edges.size(); ++i) {
		std::cout << "(" << o->edges[i]->tail->p->getP().getX().mid() << "," << o->edges[i]->tail->p->getP().getY().mid() << ") -> (" << o->edges[i]->head()->p->getP().getX().mid() << "," << o->edges[i]->head()->p->getP().getY().mid() << ")" << std::endl;
	}
	*/

	return 0;
}
