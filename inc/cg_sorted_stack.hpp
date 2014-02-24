// ---------------------------------------------------------------------------
// Nom du Header : SortedStack.hh
// Par           : Luc Martel
// Date          : 25 Mars 1999
// Modifie le    : Not Yet
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Implantation d'un stack ordonne.
//                 Chaque element inserer est place a sa position en
//                 ordre decroissant/croissant.
//                 On retire toujours le premier element, soit le plus
//                 grand/petit.

#ifndef _SORTEDSTACK__HPP_
#define _SORTEDSTACK__HPP_

#include <stdlib.h>

// Fomard Declarations
template <class T> class CSortedStack;
template <class T> class CSortedStackInv;

template <class T>
class SSNode {
private:
  friend class CSortedStack<T>;
  friend class CSortedStackInv<T>;
  T item;
  SSNode<T> *next;

  // Prive pour eviter son usage
  SSNode() {}

public:
  SSNode(T &X) {
    item = X;
    next = NULL;
  }
  SSNode(T &X, SSNode<T> *n) {
    item = X;
    next = n;
  }
  ~SSNode() {}
};

template <class T>
class CSortedStack {
public:
  unsigned int NumberOfItem;
  SSNode<T> *first;

  CSortedStack() {
    NumberOfItem = 0;
    first = NULL;
  }
  ~CSortedStack() {
    while (NumberOfItem != 0) {
      Pop();
    }
  }
  
  // Place un element sur la pile
  void Push(T &X) {
    SSNode<T> *D = new SSNode<T>(X);
    SSNode<T> *C = first;
    SSNode<T> *Trailler;

    if (NumberOfItem == 0) {
      D->next = NULL;
      first = D;
    }
    else if (NumberOfItem == 1) {
      if (first->item > D->item) {
        first->next = D;
        D->next = NULL;
      }
      else {
        D->next = first;
        first = D;
      }
    }
    else {
      if (D->item > first->item) {
        D->next = first;
        first = D;
      }
      else {
        Trailler = first;
        C = first->next;
        while ((C != NULL) && (D->item < C->item)) {
          Trailler = C;
          C = C->next;
        }
        if (C == NULL) {
          D->next = NULL;
          Trailler->next = D;
        }
        else {
          D->next = C;
          Trailler->next = D;
        }
      }
    }
    NumberOfItem++;
  }

  // Retire le premier element
  T Pop()
  {
    if (first != NULL) {
      NumberOfItem--;
      T RV = first->item;

      SSNode<T> *D = first;
      if (first->next != NULL) {
        first = first->next;
      }
      else {
        first = NULL;
      }
      delete D;
      return RV;
    }
    else {
      return (T)0;
    }
  }

};

template <class T>
class CSortedStackInv : public CSortedStack<T> {
public:
  // Place un element sur la pile
  void Push(T &X) {
    SSNode<T> *D = new SSNode<T>(X);
    SSNode<T> *C = CSortedStack<T>::first;
    SSNode<T> *Trailler;

    if (CSortedStack<T>::NumberOfItem == 0) {
      D->next = NULL;
      CSortedStack<T>::first = D;
    }
    else if (CSortedStack<T>::NumberOfItem == 1) {
      if (CSortedStack<T>::first->item < D->item) {
         CSortedStack<T>::first->next = D;
        D->next = NULL;
      }
      else {
        D->next = CSortedStack<T>::first;
        CSortedStack<T>::first = D;
      }
    }
    else {
      if (D->item < CSortedStack<T>::first->item) {
        D->next = CSortedStack<T>::first;
        CSortedStack<T>::first = D;
      }
      else {
        Trailler = CSortedStack<T>::first;
        C = CSortedStack<T>::first->next;
        while ((C != NULL) && (D->item > C->item)) {
          Trailler = C;
          C = C->next;
        }
        if (C == NULL) {
          D->next = NULL;
          Trailler->next = D;
        }
        else {
          D->next = C;
          Trailler->next = D;
        }
      }
    }
    CSortedStack<T>::NumberOfItem++;
  }
};

#endif
