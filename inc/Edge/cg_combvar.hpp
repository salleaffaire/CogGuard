// ---------------------------------------------------------------------------
// Nom du Header : combvar.hh
// Par           : Luc Martel
// Date          :
// Modifie le    :
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Etude des combinaison de variations autour d'un pixel
//                 Une carte de variation retourne pour chaque pixel A
//                
//                 Horizontale : B A C 
//                               CVHILL si (A > B et A > C) ou (A < B et A < C)
//                               CVPOS  si (B < A < C)
//                               CVNEG  si (B > A > C)

#ifndef COMBVAR_HPP__
#define COMBVAR_HPP__

#include "../cg_image.hpp"

#define CVPOS  100
#define CVNEG  200
#define CVHILL 0

template <class T>
class cg_combvar {
private:
   unsigned char IsBetween(T a, T b, T c) {
      if (((b > a) && (b < c)) || ((b < a)&&(b > c))) {
         return 1;
      }
      else {
         return 0;
      }
   }

public:
   cg_combvar() {}
   ~cg_combvar() {}

   // Retourne la carte de variation Horizontale
   cg_buffer2D<unsigned char> HVariations(cg_buffer2D<T> &X) {

      cg_buffer2D<unsigned char> TEMP(X.get_width(),X.get_height(), X.get_stride());
      TEMP.zero();

      T *sp1 = X.get_data()+X.get_stride();
      T *sp2 = X.get_data()+X.get_stride()+1;
      T *sp3 = X.get_data()+X.get_stride()+2;
      unsigned char *dp = TEMP.get_data()+TEMP.get_stride()+1;

      for (int i=0;i<(X.get_height()-2);i++) {
         for (int j=0;j<(X.get_stride()-2);j++) {
            if (((*sp2-*sp3)*(*sp2-*sp1))>0) {
               *dp = CVHILL;
            }
            else {
               if ((*sp1 - *sp3) > 0) {
                  *dp = CVNEG;
               }
               else {
                  *dp = CVPOS;
               }
            }
            dp++;
            sp1++;
            sp2++;
            sp3++;
         }
         dp += 2;
         sp1 += 2;
         sp2 += 2;
         sp3 += 2;
      }

      return TEMP;
   }


   // Retourne la carte de variation Verticale
   cg_buffer2D<unsigned char> VVariations(cg_buffer2D<T> &X) {
      cg_buffer2D<unsigned char> TEMP(X.get_width(),X.get_height(), X.get_stride());
      TEMP.zero();

      T *sp1 = X.get_data()+1;
      T *sp2 = X.get_data()+X.get_stride()+1;
      T *sp3 = X.get_data()+(2*X.get_stride())+1;
      unsigned char *dp = TEMP.get_data()+TEMP.get_stride()+1;

      for (int i=0;i<(X.get_stride()-2);i++) {
         for (int j=0;j<(X.get_height()-2);j++) {
            if (((*sp2-*sp3)*(*sp2-*sp1))>0) {
               *dp = CVHILL;
            }
            else {
               if ((*sp1 - *sp3) > 0) {
                  *dp = CVNEG;
               }
               else {
                  *dp = CVPOS;
               }
            }
            dp+=TEMP.get_stride();
            sp1+=TEMP.get_stride();
            sp2+=TEMP.get_stride();
            sp3+=TEMP.get_stride();
         }
         dp = TEMP.get_data()+TEMP.get_stride()+(i+2);
         sp1 = X.get_data()+(i+2);
         sp2 = X.get_data()+X.get_stride()+(i+2);
         sp3 = X.get_data()+(2*X.get_stride())+(i+2);
      }

      return TEMP;
   }


   // Accepte la carte de variation Horizontale et la carte de variation
   // Verticale et place a 1 les pixels pour lesquels les deux cartes
   // possedes un HILL.
   void HandH(cg_buffer2D<unsigned char> X,
              cg_buffer2D<unsigned char> Y,
              cg_buffer2D<unsigned char> &Z) {

      unsigned char *spX = X.get_data();
      unsigned char *spY = Y.get_data();
      unsigned char *dp  = Z.get_data();

      for (int i = 0;i<(X.get_stride()*X.get_height());i++) {
         if ((*spX==CVHILL)&&(*spY==CVHILL)) {
            *dp = 255;
         }
         else {
            *dp = 0;
         }
         dp++;
         spX++;
         spY++;
      }

   }

   // Accepte la carte de variation Horizontale et la carte de variation
   // Verticale et place a 1 les pixels pour lesquels une des deux cartes
   // possedes un HILL.
   void HorH(cg_buffer2D<unsigned char> X,
             cg_buffer2D<unsigned char> Y,
             cg_buffer2D<unsigned char> &Z) {

      unsigned char *spX = X.get_data();
      unsigned char *spY = Y.get_data();
      unsigned char *dp  = Z.get_data();

      for (int i = 0;i<(X.get_stride()*X.get_height());i++) {
         if ((*spX==CVHILL)||(*spY==CVHILL)) {
            *dp = 255;
         }
         else {
            *dp = 0;
         }
         dp++;
         spX++;
         spY++;
      }

   }

};


#endif
