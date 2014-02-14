/*
 * cg_object.hpp
 *
 *  Created on: Dec 3, 2013
 *      Author: luc.martel
 */

#ifndef CG_OBJECT_HPP_
#define CG_OBJECT_HPP_

#include <list>
#include <tuple>

template <class T>
class cg_vertex {
public:
   cg_vertex() : mX(0), mY(0), mZ(0), mW(0) {}
   cg_vertex(T x, T y, T z, T w) :  mX(x), mY(y), mZ(z), mW(w) {}
private:
   T mX;
   T mY;
   T mZ;
   T mW;
};

template <class T>
class cg_2d_vertex : public cg_vertex<T> {
public:
   cg_2d_vertex(T x, T y) : cg_vertex(x, y, 0, 0) {}
};

template <class T>
class cg_3d_vertex : public cg_vertex<T> {
public:
   cg_3d_vertex(T x, T y, T z) : cg_vertex(x, y, z, 0) {}
};


template <class T>
class cg_edge {
public:
   cg_edge(cg_vertex<T> *v1, cg_vertex<T> *v2) : mV1(v1), mV2(v2) {}

private:
   cg_vertex<T> *mV1;
   cg_vertex<T> *mV2;
};

template <class T>
class cg_object {
public:
   cg_object(std::list<cg_edge<T> *> *edges) : mEdges(edges) {}
   ~cg_object() {}

   void add(cg_edge<T> *edge);

private:
   std::list<cg_edge<T> > *mEdges;
};


#endif /* CG_OBJECT_HPP_ */
