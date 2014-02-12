#include <vector>
#include <iostream>
#include "point.h"
#include "acp.h"
#include "arrange.h"

using namespace std;

int main(int argc, char *argv[]) {
	Arrangement a(true);
	Arrangement b(true);

	Point* p1 = new InputPoint(0, 0);
	Point* p2 = new InputPoint(10, 0);

	a.addCircle(p1, Parameter((double)10));
	b.addCircle(p2, Parameter((double)10));

	Arrangement* o = overlay(&a, &b);

	std::cout << "Vertices: " << a.vertices.size() << std::endl;
	std::cout << "Edges: " << a.edges.size() << std::endl;
	std::cout << "Faces: " << a.faces.size() << std::endl;

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
