#ifndef	ARRANGE
#define ARRANGE

#include "point.h"
#include "circle.h"
#include <set>
#include <map>

class Edge;

class Vertex {
 public:
  Vertex () : p(0), edge(0), left(0) {}
  Vertex (Point *p) : p(p), edge(0), left(0) {}
  ~Vertex () { delete p; }
  void addEdge (Edge *e);
  void removeEdge (Edge *e);

  Point *p;
  Edge *edge, *left;
  bool flag;
};

typedef vector<Vertex *> Vertices;

class Face;

class Sweepnode;

class Component;

class Edge {
 public:
  Edge (Vertex *tail, Edge *twin, Edge *next, bool in, bool aflag, bool flag, Circle *circle, bool leftOfCircle)
    : tail(tail), helper(0), twin(twin), next(next), face(0), u(0), node(0), 
    in(in), aflag(aflag), flag(flag), circle(circle), leftOfCircle(leftOfCircle) {}
  ~Edge () { delete u; }
  Vertex * head () const { return twin->tail; }
  bool incident (Edge *e) const;
  bool increasingX ();
  bool increasingY ();
  bool clockwise (Edge *e);
  bool leftOf (Edge *e);
  bool intersects (Edge *e, Points &points);
  Edge * pred () const;
  Edge * succ () const;
  Edge * formLoop ();
  bool outer ();
  bool withinArc (Point* point) const;

  Vertex *tail, *helper;
  Edge *twin, *next;
  Face *face;
  Point *u;
  Sweepnode *node;
  bool in, aflag, flag;

  Circle* circle;
  bool leftOfCircle;
};

typedef vector<Edge *> Edges;

class Epair {
 public:
  Epair (Edge *e, Edge *f) : e(e), f(f) {}
  bool operator< (const Epair &p) const {
    return e < p.e || e == p.e && f < p.f;
  }

  Edge *e, *f;
};

typedef set<Epair> EpairSet;

class CirclePair {
 public:
  CirclePair (Circle *c1, Circle *c2) : c1(c1), c2(c2) {}
  bool operator< (const CirclePair &p) const {
    if (c1 < p.c1) {
      return true;
	}
	if (c1 == p.c1 && c2 < p.c2) {
      return true;
    }

	return false;
  }

  Circle *c1, *c2;
};

typedef set<CirclePair> CirclePairSet;

class Face {
 public:
  Face () : wna(-1), wnb(-1) {}

  Edges boundary;
  int wna, wnb;
};

typedef vector<Face *> Faces;

class Component {
 public:
  set<Circle*> members;
};

typedef vector<Component*> Components;

class Sweepnode {
 public:
  Sweepnode (Edge *edge, Sweepnode *parent) 
    : edge(edge), parent(parent), left(0), right(0) {}
  void insert (Edge *e);
  Sweepnode * pred ();
  Sweepnode * succ ();
  Sweepnode * min ();
  Sweepnode * max ();

  Edge *edge;
  Sweepnode *parent, *left, *right;
};

class Sweep {
 public:
  Sweep () : root(0) {}
  void insert (Edge *e);
  void remove (Edge *e);
  void swap (Edge *e, Edge *f);

  Sweepnode *root;
};

enum EventType {Insert, Remove, Swap};

class Event {
 public:
  Event (EventType type, Point *p, Edge *a, Edge *b = 0) 
    : type(type), p(p), a(a), b(b) {}
  bool operator< (Event &x);
  bool YOrder (Event &e);
  
  EventType type;
  Point *p;
  Edge *a, *b;
  Circle *c;
};

typedef vector<Event> Events;

void makeHeap (const Edges &edges, Events &heap);

Event nextEvent (Events &heap);

void pushHeap (const Event &e, Events &heap);

Event popHeap (Events &heap);

class Arrangement {
 public:
  Arrangement (bool rbflag = false) :rbflag(rbflag) {}
  ~Arrangement ();
  Vertex * addVertex (Point *p);
  Edge * addEdge (Circle *circle, bool leftOfCircle, Vertex *tail = 0, Vertex *head = 0, bool aflag = true,
		  bool flag = false);
  Edge * addHalfEdge (Vertex *tail, Edge *twin, Edge *next, bool in,
		      bool aflag, bool flag, Circle *circle, bool leftOfCircle);
  void removeEdge (Edge *e);
  //void addLoop (const Points &pts);
  Circle* addCircle (Point* center, Parameter radius);
  void intersectEdges ();
  void insert (Edge *e, Sweep &sweep, Events &heap, map<CirclePair, Points> &eset) const;
  void remove (Edge *e, Sweep &sweep, Events &heap, map<CirclePair, Points> &eset) const;
  void swap (Edge *e, Edge *f, Point *p, Sweep &sweep, 
	     Events &heap, map<CirclePair, Points> &eset);
  void check (Edge *e, Edge *f, Events &heap, map<CirclePair, Points> &eset) const;
  void split (Edge *e, Edge *f, Point *p);
  void formFaces ();
  void addBoundary (Edge *e, Face *f) const;
  void computeWindingNumbers () const;
  void computeComponents ();
  void computePS2 ();

  Vertices vertices;
  Edges edges;
  Faces faces;
  int numComponents;
  Components components;
  bool rbflag;
};

class HeadXOrder {
 public:
  bool operator() (Edge *e, Edge *f) const {
    return e != f && XOrder(e->head()->p, f->head()->p) == 1;
  }
};

typedef pair<Vertex *, Vertex *> Vpair;

typedef map<Vertex *, Vertex *> Vmap;

enum SetOp {Union, Intersection, AminusB, Minkowski};

Arrangement * setOperation (Arrangement *a, Arrangement *b, SetOp op);

Arrangement * overlay (Arrangement *a, Arrangement *b);

void copyEdges (Arrangement *a, bool aflag, Arrangement *b);

void copyEdge (Edge *e, bool aflag, Vmap &vmap, Arrangement *a);

Vertex * getVertex (Vertex *v, Vmap &vmap, Arrangement *a);

Arrangement * boundary (Arrangement *a, SetOp op);

bool boundary (Edge *e, SetOp op);

bool setop (SetOp op, int wna, int wnb);

void pe (Edge *e);

#endif
