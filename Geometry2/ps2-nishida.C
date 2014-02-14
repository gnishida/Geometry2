#include <vector>
#include <iostream>
#include "point.h"
#include "acp.h"
#include "arrange.h"
#include "circle.h"

using namespace std;

int main(int argc, char *argv[]) {
  /*
	Arrangement a(true);
	Arrangement b(true);

	Point* o1 = new InputPoint(0, 0);
	Point* o2 = new InputPoint(10, 10);

  if (Parameter((double)10) > Parameter((double)10)) {
	  std::cout << "OKOKOOK" << endl;
  } 

  PV2 dirX(Parameter((double)10), Parameter((double)0));
  PV2 dirY(Parameter((double)0), Parameter((double)10));
  Point *p1 = new InputPoint(o1->getP() + dirY);
  Point *p2 = new InputPoint(o1->getP() - dirX);
  Point *p3 = new InputPoint(o1->getP() - dirY);
  Point *p4 = new InputPoint(o1->getP() + dirX);

  Vertex *v1 = a.addVertex(p1);
  Vertex *v2 = a.addVertex(p2);
  Vertex *v3 = a.addVertex(p3);
  Vertex *v4 = a.addVertex(p4);

  Circle* circle1 = new Circle1pt1rad(o1, Parameter((double)10));

  Edge* e1 = a.addEdge(circle1, true, false, v1, v2);
  Edge* e2 = a.addEdge(circle1, true, true, v2, v3);
  Edge* e3 = a.addEdge(circle1, false, true, v3, v4);
  Edge* e4 = a.addEdge(circle1, false, false, v4, v1);


  Point *p5 = new InputPoint(o2->getP() + dirY);
  Point *p6 = new InputPoint(o2->getP() - dirX);
  Point *p7 = new InputPoint(o2->getP() - dirY);
  Point *p8 = new InputPoint(o2->getP() + dirX);

  Vertex *v5 = a.addVertex(p5);
  Vertex *v6 = a.addVertex(p6);
  Vertex *v7 = a.addVertex(p7);
  Vertex *v8 = a.addVertex(p8);

  Circle* circle2 = new Circle1pt1rad(o2, Parameter((double)10));

  Edge* e5 = b.addEdge(circle2, true, false, v5, v6);
  Edge* e6 = b.addEdge(circle2, true, true, v6, v7);
  Edge* e7 = b.addEdge(circle2, false, true, v7, v8);
  Edge* e8 = b.addEdge(circle2, false, false, v8, v5);

  if (e4->twin->leftOf(e6)) {
	  cout << "e4 is left of e6." << endl;
  } else {
	  cout << "e6 is left of e4." << endl;
  }
  if (e4->twin->leftOf(e1)) {
	  cout << "e4 is left of e1." << endl;
  } else {
	  cout << "e1 is left of e4." << endl;
  }
  if (e1->leftOf(e6)) {
	  cout << "e1 is left of e6." << endl;
  } else {
	  cout << "e6 is left of e1." << endl;
  }
  if (e4->twin->leftOf(e7->twin)) {
	  cout << "e4 is left of e7." << endl;
  } else {
	  cout << "e7 is left of e4." << endl;
  }
  if (e6->leftOf(e7->twin)) {
	  cout << "e6 is left of e7." << endl;
  } else {
	  cout << "e7 is left of e6." << endl;
  }
  if (e2->leftOf(e6)) {
	  cout << "e2 is left of e6." << endl;
  } else {
	  cout << "e6 is left of e2." << endl;
  }

  Points points;
  e2->intersects(e5, points);

	for (int i = 0; i < points.size(); ++i) {
	  std::cout << " Intersection: " << std::endl;
	  pp(points[i]);

	  if (e2->withinArc(points[i])) {
		std::cout << " e within" << endl;
	  } else {
		cout << " e outside" << endl;
	  }

	  if (e5->withinArc(points[i])) {
		std::cout << " f within" << endl;
	  } else {
		cout << " f outside" << endl;
	  }
	}
  */


	/*
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

	a.addCircle(circles[0]);
	for (int i = 1; i < circles.size(); ++i) {
		b.addCircle(circles[i]);
	}

	
	Arrangement *o = overlay(&a, &b);
	*/

	Arrangement a(true);
	Arrangement b(true);

	Point* p1 = new InputPoint(0, 0);
	Point* p2 = new InputPoint(10, 10);

	Circle *c1 = new Circle1pt1rad(p1, Parameter((double)10)); 
	Circle *c2 = new Circle1pt1rad(p2, Parameter((double)10)); 

	PV2 dir = c2->getO() - c1->getO();
	Parameter d2 = dir.dot(dir);
	Parameter d = d2.sqrt();

	a.addCircle(p1, Parameter((double)10));
	b.addCircle(p2, Parameter((double)10));

	Arrangement* o = overlay(&a, &b);

	std::cout << "Vertices: " << o->vertices.size() << std::endl;
	std::cout << "Edges: " << o->edges.size() / 2 << std::endl;
	std::cout << "Faces: " << o->faces.size() << std::endl;
	std::cout << "V - E + F = " << o->vertices.size() - o->edges.size() / 2  + o->faces.size() << std::endl;

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
