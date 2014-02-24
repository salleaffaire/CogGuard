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

#include "../cg_image.hpp"

template <class T>

class cg_edge_ex {
public:
   cg_edge_ex() {}
   virtual ~cg_edge_ex() {}

   virtual void ExtractEdge(cg_buffer2D<T> &X_IN, cg_buffer2D<unsigned char> &X_OUT)  = 0;
   virtual void ExtractAngle(cg_buffer2D<T> &X_IN, cg_buffer2D<double> &X_OUT) = 0;

   cg_image<unsigned char> ExtractEdge(cg_buffer2D<T> &X) {
      cg_image<unsigned char> rval(_ExtractEdge(X));
      return rval;
   }

   cg_image<double> ExtractAngle(cg_buffer2D<T> &X) {
      cg_image<double> rval(_ExtractAngle(X));
      return rval;
   }

private:

   cg_buffer2D<unsigned char> _ExtractEdge(cg_buffer2D<T> &X_IN) {
      cg_buffer2D<unsigned char> TEMP(X_IN.get_width(), X_IN.get_height(), X_IN.get_stride());

      ExtractEdge(X_IN, TEMP);

      return TEMP;
   }

   cg_buffer2D<double> _ExtractAngle(cg_buffer2D<T> &X_IN) {
      cg_buffer2D<double> TEMP(X_IN.get_width(), X_IN.get_height(), X_IN.get_stride());

      ExtractAngle(X_IN, TEMP);

      return TEMP;
   }

};

#endif
