#include "arrange.h"

#define M_PI	3.141592653589793238

void Vertex::addEdge (Edge *e)
{
  if (!edge) {
    edge = e;
    e->next = e;
  }
  else if (e->clockwise(edge)) {
    e->next = edge;
    Edge *f = edge;
    while (f->next != edge)
      f = f->next;
    f->next = e;
    edge = e;
  }
  else {
    Edge *f = edge;
    while (f->next != edge && f->next->clockwise(e))
      f = f->next;
    e->next = f->next;
    f->next = e;
  }
}

void Vertex::removeEdge (Edge *e)
{
  if (edge == e && e->next == e)
    edge = 0;
  else {
    Edge *f = edge;
    while (f->next != e)
      f = f->next;
    f->next = e->next;
    if (edge == e)
      edge = f;
  }
}

bool Edge::incident (Edge *e) const
{
  return tail == e->tail || tail == e->head() ||
    head() == e->tail || head() == e->head();
}

bool Edge::increasingX ()
{
  return XSign(u) == 1;
}

bool Edge::increasingY ()
{
  return YSign(u) == 1;
}

bool Edge::clockwise (Edge *e)
{
  bool inc = increasingY(), einc = e->increasingY();
  return inc != einc ? inc 
    : LeftTurn(head()->p, tail->p, e->head()->p) == 1;
}

bool Edge::leftOf (Edge *e)
{
  /*
  return tail == e->tail ? 
    LeftTurn(head()->p, e->tail->p, e->head()->p) == -1
    : LeftTurn(tail->p, e->tail->p, e->head()->p) == -1;
  */

  if (circle == e->circle) {
  return tail == e->tail ? 
    LeftTurn(head()->p, e->tail->p, e->head()->p) == -1
    : LeftTurn(tail->p, e->tail->p, e->head()->p) == -1;
  }

  if (e->leftOfCircle) {
    if (tail->p->getP().getX() > e->circle->getO().getX()) return false;
    PV2 dir = tail->p->getP() - e->circle->getO();
    if (dir.dot(dir) > e->circle->getRR()) return true;
    else return false;
  } else {
    if (tail->p->getP().getX() < e->circle->getO().getX()) return true;
    PV2 dir = tail->p->getP() - e->circle->getO();
    if (dir.dot(dir) > e->circle->getRR()) return false;
    else return true;
  } 
}

void Edge::intersects (Edge *e, Points &points)
{
  std::cout << "=== Edge::intersects ==============================" << std::endl;
  std::cout << "=== 1st edge =====" << std::endl;
  pp(tail->p);
  pp(head()->p);
  std::cout << " center: " << circle->getO().getX().mid() << "," << circle->getO().getY().mid() << std::endl;
  std::cout << "=== 2nd edge =====" << std::endl;
  pp(e->tail->p);
  pp(e->head()->p);
  std::cout << " center: " << e->circle->getO().getX().mid() << "," << e->circle->getO().getY().mid() << std::endl;

  PV2 dir = e->circle->getO() - circle->getO();
  Parameter d2 = dir.dot(dir);
  std::cout << "d2: " << d2.mid() << std::endl;
  Parameter d = d2.sqrt();
  std::cout << "d: " << d.mid() << std::endl;

  if (d > circle->getRR().sqrt() + e->circle->getRR().sqrt()) {
	std::cout << "  too far away!" << std::endl;
    return;
  }

  std::cout << "OK" << std::endl;

  if (d < (circle->getRR().sqrt() - e->circle->getRR().sqrt()).abs()) {
    std::cout << "  One circle is inside the other." << std::endl;
	return;
  }

  std::cout << "OK" << std::endl;

  Parameter a = (d2 + circle->getRR() - e->circle->getRR()) / 2 / d;
  PV2 midPt = circle->getO() + dir * a / d;

  Point* normal = new Normal(new InputPoint(circle->getO()), new InputPoint(e->circle->getO()));
  Parameter h = (circle->getRR() - a * a).sqrt();

  PV2 intersection1 = circle->getO() + dir / d * a + normal->getP() / d * h;
  PV2 intersection2 = circle->getO() + dir / d * a - normal->getP() / d * h;

  std::cout << "   intersection1: " << intersection1.getX().mid() << "," << intersection1.getY().mid() << std::endl;
  std::cout << "   intersection2: " << intersection2.getX().mid() << "," << intersection2.getY().mid() << std::endl;

  delete normal;

  if (intersection1.getY() > intersection2.getY()) {
    points.push_back(new InputPoint(intersection1));
    points.push_back(new InputPoint(intersection2));
  } else {
    points.push_back(new InputPoint(intersection2));
    points.push_back(new InputPoint(intersection1));
  }
}

Edge * Edge::pred () const
{
  Sweepnode *n = node->pred();
  return n ? n->edge : 0;
}

Edge * Edge::succ () const
{
  Sweepnode *n = node->succ();
  return n ? n->edge : 0;
}

Edge * Edge::formLoop ()
{
  int count = 0;
  std::cout << "!*!*!*!*!* [ Edge::formLoop ] !*!*!*!*!" << std::endl;
  Edge *e = this, *l = this;
  while (true) {
	std::cout << "No. " << (count+1) << std::endl;
	count++;
	pp(e->tail->p);
	pp(e->head()->p);
    e->flag = true;
    e = e->twin->next;
    if (e->flag)
      break;
    if (e->head()->p != l->head()->p &&
	XOrder(e->head()->p, l->head()->p) == 1)
      l = e;
  }
  // eps: to do: use vis version
  return l;
}

bool Edge::outer ()
{
  Edge *f = twin->next;
  return f != twin && LeftTurn(tail->p, head()->p, f->head()->p) == 1;
}

bool Edge::withinArc (Point* point) const
{
  if (leftOfCircle && point->getP().getX() > circle->getO().getX()) return false;
  if (!leftOfCircle && point->getP().getX() < circle->getO().getX()) return false;

  if (bottomOfCircle && point->getP().getY() > circle->getO().getY()) return false;
  if (!bottomOfCircle && point->getP().getY() < circle->getO().getY()) return false;

  return true;
}

void Sweepnode::insert (Edge *e)
{
  if (e->twin->leftOf(edge->twin))
    if (left)
      left->insert(e);
    else {
      left = new Sweepnode(e, this);
      e->node = left;
    }
  else if (right)
    right->insert(e);
  else {
    right = new Sweepnode(e, this);
    e->node = right;
  }
}

Sweepnode * Sweepnode::pred ()
{
  if (left)
    return left->max();
  Sweepnode *e = this, *f = parent;
  while (f && e == f->left) {
    e = f;
    f = f->parent;
  }
  return f;
}

Sweepnode * Sweepnode::succ ()
{
  if (right)
    return right->min();
  Sweepnode *e = this, *f = parent;
  while (f && e == f->right) {
    e = f;
    f = f->parent;
  }
  return f;
}

Sweepnode * Sweepnode::min ()
{
  Sweepnode *x = this;
  while (x->left)
    x = x->left;
  return x;
}

Sweepnode * Sweepnode::max ()
{
  Sweepnode *x = this;
  while (x->right)
    x = x->right;
  return x;
}

void Sweep::insert (Edge *e)
{
  if (root == 0) {
	std::cout << "Sweep::insert ... no root" << std::endl;
    root = new Sweepnode(e, 0);
    e->node = root;
  }
  else {
	std::cout << "Sweep::insert ... root->insert" << std::endl;
	std::cout << "    root: " << root->edge->tail->p->getP().getX().mid() << "," <<  root->edge->tail->p->getP().getY().mid() << std::endl;
    root->insert(e);
  }
}

void Sweep::remove (Edge *e)
{
  Sweepnode *z = e->node;
  Sweepnode *y = !z->left || !z->right ? z : z->succ();
  Sweepnode *x = y->left ? y->left : y->right;
  if (x)
    x->parent = y->parent;
  if (!y->parent)
    root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;
  if (y != z) {
    z->edge = y->edge;
    y->edge->node = z;
    delete y;
  }
  else
    delete z;
  e->node = 0;
}

void Sweep::swap (Edge *e, Edge *f)
{
  Sweepnode *temp = e->node;
  e->node = f->node;
  f->node = temp;
  e->node->edge = e;
  f->node->edge = f;
}

bool Event::operator< (Event &e)
{ 
  if (type == Insert && e.type == Remove && a->head() == e.a->tail)
    return true;
  if (type == Remove && e.type == Insert && a->tail == e.a->head())
    return false;
  if (type == Insert && e.type == Insert && a->head() == e.a->head() ||
      type == Remove && e.type == Remove && a->tail == e.a->tail) {
	/*std::cout << "=== Event::operator<" << std::endl;
    pp(a->tail->p);
    pp(a->head()->p);
    pp(e.a->tail->p);
    pp(e.a->head()->p);
	std::cout << "Ans: " << (a < e.a) << std::endl;*/
    return a < e.a;
  }
  return YOrder(e);
}

bool Event::YOrder (Event &e)
{
  return ::YOrder(p, e.p) == 1;
}

void makeHeap (const Edges &edges, Events &heap)
{
  for (Edges::const_iterator e = edges.begin(); e != edges.end(); ++e)
    if ((*e)->increasingY())
      heap.push_back(Event(Remove, (*e)->tail->p, *e));
    else
      heap.push_back(Event(Insert, (*e)->tail->p, (*e)->twin));
  make_heap(heap.begin(), heap.end());
}

Event nextEvent (Events &heap)
{
  Event e = heap[0];
  do {
    pop_heap(heap.begin(), heap.end());
    heap.pop_back();
  }
  while (!heap.empty() && e.type == Swap && heap[0].type == Swap &&
	 e.a == heap[0].a && e.b == heap[0].b);
  return e;
}

void pushHeap (const Event &e, Events &heap)
{
  heap.push_back(e);
  push_heap(heap.begin(), heap.end());
}

Event popHeap (Events &heap)
{
  Event e = heap[0];
  pop_heap(heap.begin(), heap.end());
  heap.pop_back();
  return e;
}

Arrangement::~Arrangement ()
{
  for (Vertices::iterator v = vertices.begin(); v != vertices.end(); ++v)
    delete *v;
  for (Edges::iterator e = edges.begin(); e != edges.end(); ++e)
    delete *e;
  for (Faces::iterator f = faces.begin(); f != faces.end(); ++f)
    delete *f;
}

Vertex * Arrangement::addVertex (Point *p)
{
  Vertex *v = new Vertex(p);
  vertices.push_back(v);
  return v;
}

Edge * Arrangement::addEdge (Circle *circle, bool leftOfCircle, bool bottomOfCircle, Vertex *tail, Vertex *head, bool aflag, bool flag)
{
  Edge *e = addHalfEdge(tail, 0, 0, true, aflag, flag, circle, leftOfCircle, bottomOfCircle),
    *et = addHalfEdge(head, e, 0, false, aflag, flag, circle, leftOfCircle, bottomOfCircle);
  e->twin = et;
  e->u = new Vector(tail->p, head->p);
  e->twin->u = new Vector(head->p, tail->p);
  tail->addEdge(e);
  head->addEdge(et);
  return e;
}

Edge * Arrangement::addHalfEdge (Vertex *tail, Edge *twin, Edge *next, bool in,
				 bool aflag, bool flag, Circle *circle, bool leftOfCircle, bool bottomOfCircle)
{
  Edge *e = new Edge(tail, twin, next, in, aflag, flag, circle, leftOfCircle, bottomOfCircle);
  edges.push_back(e);
  return e;
}

void Arrangement::removeEdge (Edge *e)
{
  int c = 0, i = 0;
  while (c < 2 && i < edges.size())
    if (edges[i] == e || edges[i] == e->twin) {
      edges[i] = *(edges.end()-1);
      edges.pop_back();
      ++c;
    }
    else
      ++i;
  e->tail->removeEdge(e);
  e->twin->tail->removeEdge(e->twin);
  delete e->twin;
  delete e;
}

/*void Arrangement::addLoop (const Points &pts)
{
  int n = pts.size();
  Vertex *t = addVertex(pts[0]->copy()), *t0 = t;
  for (int i = 1; i <= n; ++i) {
    Vertex *h = i < n ? addVertex(pts[i]->copy()) : t0;
    addEdge(t, h);
    t = h;
  }
}*/

void Arrangement::addCircle (Point* center, Parameter radius)
{
  PV2 dirX(radius, Parameter((double)0));
  PV2 dirY(Parameter((double)0), radius);
  Point *p1 = new InputPoint(center->getP() + dirY);
  Point *p2 = new InputPoint(center->getP() - dirX);
  Point *p3 = new InputPoint(center->getP() - dirY);
  Point *p4 = new InputPoint(center->getP() + dirX);

  Vertex *v1 = addVertex(p1);
  Vertex *v2 = addVertex(p2);
  Vertex *v3 = addVertex(p3);
  Vertex *v4 = addVertex(p4);

  Circle* circle = new Circle1pt1rad(center, radius);

  addEdge(circle, true, false, v1, v2);
  addEdge(circle, true, true, v2, v3);
  addEdge(circle, false, true, v3, v4);
  addEdge(circle, false, false, v4, v1);
}

void Arrangement::intersectEdges ()
{
  Events heap;
  makeHeap(edges, heap);
  Sweep sweep;
  CirclePairSet eset;

  map<CirclePair, Points> intersectionsMap;

  while (!heap.empty()) {
    Event e = popHeap(heap);
    std::cout << "heap " << e.type << std::endl;
	pp(e.a->tail->p);
	pp(e.a->head()->p);
	std::cout << " circle: " << e.a->circle->getO().getX().mid() << "," << e.a->circle->getO().getY().mid() << std::endl;
    std::cout << "===" << std::endl;

    switch (e.type) {
    case Insert:
      insert(e.a, sweep, heap, intersectionsMap);
      break;
    case Remove:
      remove(e.a, sweep, heap, intersectionsMap);
      break;
    case Swap:
      swap(e.a, e.b, e.p, sweep, heap, intersectionsMap);
    }
  }
}

void Arrangement::insert (Edge *e, Sweep &sweep, Events &heap,
			  map<CirclePair, Points> &eset) const
{
  sweep.insert(e);
  Edge *pred = e->pred(), *succ = e->succ();
  if (pred && !e->head()->left && e->head() != pred->head())
    e->head()->left = pred->twin;

  std::cout << "insert" << std::endl;
  std::cout << "pred: " << std::endl;
  if (pred) {
    pp(pred->tail->p);
	pp(pred->head()->p);
  }
  std::cout << "succ: " << std::endl;
  if (succ) {
    pp(succ->tail->p);
	pp(succ->head()->p);
  }


  check(pred, e, heap, eset);
  check(e, succ, heap, eset);
}

void Arrangement::remove (Edge *e, Sweep &sweep, Events &heap,
			  map<CirclePair, Points> &eset) const
{
  Edge *pred = e->pred(), *succ = e->succ();
  sweep.remove(e);
  if (pred && pred->tail != e->tail)
    e->tail->left = pred->twin;
  check(pred, succ, heap, eset);
}

void Arrangement::swap (Edge *e, Edge *f, Point *p, Sweep &sweep,
			Events &heap, map<CirclePair, Points> &eset)
{
  split(e, f, p);
  Edge *pred = e->pred(), *succ = f->succ();
  sweep.swap(e, f);
  if (pred)
    e->head()->left = pred->twin;
  check(pred, f, heap, eset);
  check(e, succ, heap, eset);
}

void Arrangement::check (Edge *e, Edge *f, Events &heap, map<CirclePair, Points> &intersectionsMap) const
{
  if (rbflag) {
    std::cout << "WARNING!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  }

  if (e && f && !(rbflag && e->aflag == f->aflag)) {
    if (e->circle == f->circle) return;

	std::cout << "check" << std::endl;

	Points intersections;

    CirclePair ef(e->circle < f->circle ? e->circle : f->circle, e->circle < f->circle ? f->circle : e->circle);
	if (intersectionsMap.find(ef) != intersectionsMap.end()) {
	  std::cout << "These two circles are already checked before." << std::endl;
	  intersections = intersectionsMap[ef];
	  if (intersections.size() == 0) return;
	} else {
	  std::cout << "These two circles are not checked before." << std::endl;
	  e->intersects(f, intersections);
	  intersectionsMap[ef] = intersections;
	}
	std::cout << " ... OK" << std::endl;

	Points remainedIntersections;

	bool swapped = false;
	std::cout << "#### start checking if intersection is within arc." << std::endl;
	for (int i = 0; i < intersections.size(); ++i) {
	  std::cout << " Intersection: " << std::endl;
	  pp(intersections[i]);

	  if (e->withinArc(intersections[i]) && f->withinArc(intersections[i])) {
	    std::cout << " ... within arc!!!" << std::endl;
		if (swapped) {
		  std::cout << "pushHeap (Swap, f, e)" << std::endl;
	      pushHeap(Event(Swap, intersections[i], f, e), heap);
		} else {
		  std::cout << "pushHeap (Swap, e, f)" << std::endl;
	      pushHeap(Event(Swap, intersections[i], e, f), heap);
		}
		swapped = true;
	  } else {
   	    remainedIntersections.push_back(intersections[i]);
	  }
	}
	std::cout << "#### end checking if intersection is within arc." << std::endl;
	std::cout << intersections.size() - remainedIntersections.size() << std::endl;

	intersectionsMap[ef] = remainedIntersections;
  }
}

void Arrangement::split (Edge *e, Edge *f, Point *p)
{
  std::cout << "split" << std::endl;
  pp(e->tail->p);
  pp(e->head()->p);
  std::cout << "center: " << e->circle->getO().getX().mid() << "," << e->circle->getO().getY().mid() << std::endl;
  pp(f->tail->p);
  pp(f->head()->p);
  std::cout << "center: " << f->circle->getO().getX().mid() << "," << f->circle->getO().getY().mid() << std::endl;

  Vertex *v = addVertex(p);
  Edge *et = e->twin, *ft = f->twin, 
	  *e4 = addHalfEdge(v, et, 0, e->in, e->aflag, e->flag, e->circle, e->leftOfCircle, e->bottomOfCircle),
    *e3 = addHalfEdge(v, f, e4, ft->in, ft->aflag, ft->flag, f->circle, e->leftOfCircle, e->bottomOfCircle),
    *e2 = addHalfEdge(v, e, e3, et->in, et->aflag, et->flag, e->circle, e->leftOfCircle, e->bottomOfCircle),
    *e1 = addHalfEdge(v, ft, e2, f->in, f->aflag, f->flag, f->circle, e->leftOfCircle, e->bottomOfCircle);




  e4->next = e1;
  e->twin = e2;
  et->twin = e4;
  f->twin = e3;
  ft->twin = e1;

  std::cout << "middle split" << std::endl;
  pp(e->tail->p);
  pp(e->head()->p);
  pp(f->tail->p);
  pp(f->head()->p);


  e1->u = f->u->copy();
  e2->u = et->u->copy();
  e3->u = ft->u->copy();
  e4->u = e->u->copy();
  v->edge = e1;

  std::cout << "after split" << std::endl;
  pp(e->tail->p);
  pp(e->head()->p);
  pp(f->tail->p);
  pp(f->head()->p);
}

void Arrangement::formFaces ()
{
  for (Faces::iterator f = faces.begin(); f != faces.end(); ++f)
    delete *f;
  faces.clear();
  Edges inner;
  faces.push_back(new Face);
  for (Edges::iterator e = edges.begin(); e != edges.end(); ++e)
    if (!(*e)->flag) {
      Edge *l = (*e)->formLoop();
      if (l->outer()) {
	std::cout << " this is an outer loop." << std::endl;
	Face *f = new Face;
	faces.push_back(f);
	addBoundary(l, f);
      }
      else {
	std::cout << " this is an inner loop." << std::endl;
	inner.push_back(l);
	  }
    }
  sort(inner.begin(), inner.end(), HeadXOrder());
  for (Edges::iterator e = inner.begin(); e != inner.end(); ++e) {
    Vertex *v = (*e)->head();
    Face *f = v->left ? v->left->face : faces[0];
    addBoundary(*e, f);
  }
}

void Arrangement::addBoundary (Edge *e, Face *f) const
{
  f->boundary.push_back(e);
  Edge *g = e;
  do {
    g->tail->flag = true;
    g->flag = true;
    g->face = f;
    g = g->twin->next;
  }
  while (g != e);
}

void Arrangement::computeWindingNumbers () const
{
  Faces st;
  st.push_back(faces[0]);
  faces[0]->wna = faces[0]->wnb = 0;
  while (!st.empty()) {
    Face *f = *(st.end()-1);
    st.pop_back();
    for (Edges::iterator b = f->boundary.begin(); 
	 b != f->boundary.end(); ++b) {
      Edge *e = *b;
      do {
	Face *g = e->twin->face;
	if (g->wna == -1) {
	  if (e->aflag) {
	    g->wna = e->in ? f->wna - 1 : f->wna + 1;
	    g->wnb = f->wnb;
	  }
	  else {
	    g->wna = f->wna;
	    g->wnb = e->in ? f->wnb - 1 : f->wnb + 1;
	  }
	  st.push_back(g);
	}
	e = e->twin->next;
      }
      while (e != *b);
    }
  }
}

Arrangement * setOperation (Arrangement *a, Arrangement *b, SetOp op)
{
  Arrangement *o = overlay(a, b);
  Arrangement *s = boundary(o, op);
  delete o;
  return s;
}

Arrangement * overlay (Arrangement *a, Arrangement *b)
{
  Arrangement *arr = new Arrangement(false);
  copyEdges(a, true, arr);
  copyEdges(b, false, arr);
  std::cout << "overlay: copyEdges" << std::endl;
  arr->intersectEdges();
  std::cout << "overlay: intersectEdges" << std::endl;
  arr->formFaces();
  arr->computeWindingNumbers();
  return arr;
}

void copyEdges (Arrangement *a, bool aflag, Arrangement *b)
{
  Vmap vmap;
  for (Edges::iterator e = a->edges.begin(); e != a->edges.end(); ++e)
    if ((*e)->in)
      copyEdge(*e, aflag, vmap, b);
}

void copyEdge (Edge *e, bool aflag, Vmap &vmap, Arrangement *a)
{
  Vertex *t = getVertex(e->tail, vmap, a),
    *h = getVertex(e->head(), vmap, a);
  a->addEdge(e->circle, e->leftOfCircle, e->bottomOfCircle, t, h, aflag);
}

Vertex * getVertex (Vertex *v, Vmap &vmap, Arrangement *a)
{
  Vmap::iterator i = vmap.find(v);
  if (i != vmap.end())
    return i->second;
  Vertex *w = a->addVertex(v->p->copy());
  vmap.insert(Vpair(v, w));
  return w;
}

Arrangement * boundary (Arrangement *a, SetOp op)
{
  Vmap vmap;
  Arrangement *b = new Arrangement;
  for (Edges::iterator e = a->edges.begin(); e != a->edges.end(); ++e) 
    if (boundary(*e, op))
      copyEdge(*e, true, vmap, b);
  b->formFaces();
  return b;
}

bool boundary (Edge *e, SetOp op)
{
  bool ein = setop(op, e->face->wna, e->face->wnb),
    etin = setop(op, e->twin->face->wna, e->twin->face->wnb);
  return ein && !etin;
}

bool setop (SetOp op, int wna, int wnb)
{
  bool ina = wna%2 == 1, inb = wnb%2 == 1;
  switch (op) {
  case Union: 
    return ina || inb;
  case Intersection: 
    return ina && inb;
  case AminusB:
    return ina && !inb;
  case Minkowski:
    return wna > 0;
  }
  return false;
}

void pe (Edge *e)
{
  cerr << "(";
  pp(e->tail->p);
  pp(e->head()->p);
  cerr << ")";
}

void pes (Edges &edges)
{
  cerr << "(";
  for (int i = 0; i < edges.size(); ++i)
    pe(edges[i]);
  cerr << ")" << endl;
}

