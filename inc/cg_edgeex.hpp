// ---------------------------------------------------------------------------
// Nom du Header : edgeex.hh
// Par           : Luc Martel
// Date          : 23 Avril 1999
// Modifie le    : 
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Classe de base pour un extracteurs d'arretes
// 

#ifndef CG_EDGEEX_HPP__
#define CG_EDGEEX_HPP__

#include "cg_buffer.hpp"


template <class T>

class CEdgeEx {
public:
  CEdgeEx() {}
  virtual ~CEdgeEx() {}

  // Methode virtuelle pure d'extraction d'arretes
  virtual cg_buffer2D<unsigned char> ExtractEdge(cg_buffer2D<T> &X)  = 0;
  virtual cg_buffer2D<double>        ExtractAngle(cg_buffer2D<T> &X) = 0;
};

#endif
