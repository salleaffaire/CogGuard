// ---------------------------------------------------------------------------
// Nom du Header : canny.hh
// Par           : Luc Martel
// Date          : 23 Avril 1999
// Modifie le    :
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Extrtacteur d'arretes de Canny

#ifndef CANNY_HPP__
#define CANNY_HPP__

// This needs a lot of optimization
// -----------------------------------------------------------
// -----------------------------------------------------------

#include <math.h>
#include "cg_edgeex.hpp"

template <class T>
class cg_canny : public cg_edge_ex<T> {
private:
   float G3[3];
   float DG3[5];
public:
   cg_canny() {
      G3[0] = 0.25;
      G3[1] = 0.5;
      G3[2] = 0.25;

      DG3[0] = 0.25;
      DG3[1] = 0.75;
      DG3[2] = 0;
      DG3[3] = -0.75;
      DG3[4] = -0.25;

   }
   ~cg_canny() {}


   void ExtractEdge(cg_buffer2D<T> &X_IN, cg_buffer2D<unsigned char> &X_OUT) {
      cg_buffer2D<float> GRADX = GaussianX(X_IN);
      cg_buffer2D<float> GRADY = GaussianY(X_IN);

      cg_buffer2D<float> DIFF_GRADX  = DiffGausX(GRADX);
      cg_buffer2D<float> DIFF_GRADY  = DiffGausY(GRADY);

      cg_buffer2D<float> GAMP = Amplitude(DIFF_GRADX, DIFF_GRADY);

      GAMP.scale_to_unsignedchar(X_OUT);
   }

   void ExtractAngle(cg_buffer2D<T> &X_IN, cg_buffer2D<double> &X_OUT) {
      cg_buffer2D<float> GRADX = GaussianX(X_IN);
      cg_buffer2D<float> GRADY = GaussianY(X_IN);

      cg_buffer2D<float> DIFF_GRADX = DiffGausX(GRADX);
      cg_buffer2D<float> DIFF_GRADY = DiffGausY(GRADY);

      cg_buffer2D<float> GANG = Angle(DIFF_GRADX, DIFF_GRADY);

      GANG.to_double(X_OUT);
   }

   cg_buffer2D<float> GaussianX(cg_buffer2D<T> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      T *Tp1 = X.get_data();
      T *Tp2 = X.get_data()+1;
      T *Tp3 = X.get_data()+2;
      float *fp = TEMP.get_data()+1;

      for(unsigned int i=0;i<X.get_height();i++) {
         for (unsigned int j=0;j<X.get_stride()-2;j++) {
            *fp = (G3[0]*(float)*Tp1++) + (G3[1]*(float)*Tp2++) +
                  (G3[2]*(float)*Tp3++);

            fp++;
         }
         fp += 2;
         Tp1 += 2;
         Tp2 += 2;
         Tp3 += 2;

      }

      return TEMP;
   }

   cg_buffer2D<float> GaussianY(cg_buffer2D<T> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      T *fp1 = X.get_data();
      T *fp2 = X.get_data()+X.get_stride();
      T *fp3 = X.get_data()+(X.get_stride()*2);
      float *fp = TEMP.get_data()+(X.get_stride());

      for(int i=0;i<X.get_stride();i++) {
         for (int j=0;j<X.get_height()-2;j++) {
            *fp = G3[0]*(float)*fp1 + G3[1]*(float)*fp2 + G3[2]*(float)*fp3;
            fp += X.get_stride();
            fp1 += X.get_stride();
            fp2 += X.get_stride();
            fp3 += X.get_stride();
         }
         fp1 = X.get_data()+i;;
         fp2 = X.get_data()+X.get_stride()+i;
         fp3 = X.get_data()+(X.get_stride()*2)+i;
         fp = TEMP.get_data()+(X.get_stride())+i;
      }

      return TEMP;
   }

   cg_buffer2D<float> DiffGausX(cg_buffer2D<T> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      T *Tp1 = X.get_data();
      T *Tp2 = X.get_data()+1;
      T *Tp3 = X.get_data()+2;
      T *Tp4 = X.get_data()+3;
      T *Tp5 = X.get_data()+4;

      float *fp = TEMP.get_data()+2;

      for(unsigned int i=0;i<X.get_height();i++) {
         for (unsigned int j=0;j<X.get_stride()-4;j++) {
            *fp = (DG3[0]*(float)*Tp1++) + (DG3[1]*(float)*Tp2++) +
                  (DG3[2]*(float)*Tp3++) + (DG3[3]*(float)*Tp4++) +
                  (DG3[4]*(float)*Tp5++);
            fp++;
         }
         fp += 4;
         Tp1 += 4;
         Tp2 += 4;
         Tp3 += 4;
         Tp4 += 4;
         Tp5 += 4;
      }

      return TEMP;
   }

   cg_buffer2D<float> DiffGausY(cg_buffer2D<T> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      T *fp1 = X.get_data();
      T *fp2 = X.get_data()+X.get_stride();
      T *fp3 = X.get_data()+(X.get_stride()*2);
      T *fp4 = X.get_data()+(X.get_stride()*3);
      T *fp5 = X.get_data()+(X.get_stride()*4);

      float *fp = TEMP.get_data()+(X.get_stride()*2);

      for(int i=0;i<X.get_stride();i++) {
         for (int j=0;j<X.get_height()-4;j++) {
            *fp = DG3[0]*(float)*fp1 + DG3[1]*(float)*fp2 + DG3[2]*(float)*fp3 +
                  DG3[3]*(float)*fp4 + DG3[4]*(float)*fp5;
            fp += X.get_stride();
            fp1 += X.get_stride();
            fp2 += X.get_stride();
            fp3 += X.get_stride();
            fp4 += X.get_stride();
            fp5 += X.get_stride();
         }
         fp1 = X.get_data()+i;
         fp2 = X.get_data()+X.get_stride()+i;
         fp3 = X.get_data()+(X.get_stride()*2)+i;
         fp4 = X.get_data()+(X.get_stride()*3)+i;
         fp5 = X.get_data()+(X.get_stride()*4)+i;
         fp = TEMP.get_data()+(X.get_stride()*2)+i;
      }
      return TEMP;
   }

   // -------------------------------------------------------------------------
   // -------------------------------------------------------------------------
   // -------------------------------------------------------------------------

   cg_buffer2D<float> GaussianX(cg_buffer2D<float> &X) {

      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *Tp1 = X.get_data();
      float *Tp2 = X.get_data()+1;
      float *Tp3 = X.get_data()+2;

      float *fp = TEMP.get_data()+1;

      for(int i=0;i<X.get_height();i++) {
         for (int j=0;j<X.get_stride()-2;j++) {
            *fp = (G3[0]*(float)*Tp1++) + (G3[1]*(float)*Tp2++) +
                  (G3[2]*(float)*Tp3++);
            fp++;
         }
         fp += 2;
         Tp1 += 2;
         Tp2 += 2;
         Tp3 += 2;
      }

      return TEMP;
   }

   cg_buffer2D<float> GaussianY(cg_buffer2D<float> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *fp1 = X.get_data();
      float *fp2 = X.get_data()+X.get_stride();
      float *fp3 = X.get_data()+(X.get_stride()*2);

      float *fp = TEMP.get_data()+(X.get_stride());

      for(unsigned int i=0;i<X.get_stride();i++) {
         for (unsigned int j=0;j<X.get_height()-2;j++) {
            *fp = G3[0]*(float)*fp1 + G3[1]*(float)*fp2 + G3[2]*(float)*fp3;
            fp += X.get_stride();
            fp1 += X.get_stride();
            fp2 += X.get_stride();
            fp3 += X.get_stride();
         }
         fp1 = X.get_data()+i;
         fp2 = X.get_data()+X.get_stride()+i;
         fp3 = X.get_data()+(X.get_stride()*2)+i;
         fp = TEMP.get_data()+(X.get_stride())+i;
      }

      return TEMP;
   }

   cg_buffer2D<float> DiffGausX(cg_buffer2D<float> &X) {

      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *Tp1 = X.get_data();
      float *Tp2 = X.get_data()+1;
      float *Tp3 = X.get_data()+2;
      float *Tp4 = X.get_data()+3;
      float *Tp5 = X.get_data()+4;

      float *fp = TEMP.get_data()+2;

      for(int i=0;i<X.get_height();i++) {
         for (int j=0;j<X.get_stride()-4;j++) {
            *fp = (DG3[0]*(float)*Tp1++) + (DG3[1]*(float)*Tp2++) +
                  (DG3[2]*(float)*Tp3++) + (DG3[3]*(float)*Tp4++) +
                  (DG3[4]*(float)*Tp5++);
            fp++;
         }
         fp += 4;
         Tp1 += 4;
         Tp2 += 4;
         Tp3 += 4;
         Tp4 += 4;
         Tp5 += 4;
      }

      return TEMP;
   }

   cg_buffer2D<float> DiffGausY(cg_buffer2D<float> &X) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *fp1 = X.get_data();
      float *fp2 = X.get_data()+X.get_stride();
      float *fp3 = X.get_data()+(X.get_stride()*2);
      float *fp4 = X.get_data()+(X.get_stride()*3);
      float *fp5 = X.get_data()+(X.get_stride()*4);

      float *fp = TEMP.get_data()+(X.get_stride()*2);

      for(unsigned int i=0;i<X.get_stride();i++) {
         for (unsigned int j=0;j<X.get_height()-4;j++) {
            *fp = DG3[0]*(float)*fp1 + DG3[1]*(float)*fp2 + DG3[2]*(float)*fp3 +
                  DG3[3]*(float)*fp4 + DG3[4]*(float)*fp5;
            fp += X.get_stride();
            fp1 += X.get_stride();
            fp2 += X.get_stride();
            fp3 += X.get_stride();
            fp4 += X.get_stride();
            fp5 += X.get_stride();
         }
         fp1 = X.get_data()+i;
         fp2 = X.get_data()+X.get_stride()+i;
         fp3 = X.get_data()+(X.get_stride()*2)+i;
         fp4 = X.get_data()+(X.get_stride()*3)+i;
         fp5 = X.get_data()+(X.get_stride()*4)+i;
         fp = TEMP.get_data()+(X.get_stride()*2)+i;
      }
      return TEMP;
   }

   // -------------------------------------------------------------------------
   // -------------------------------------------------------------------------

   cg_buffer2D<float> Amplitude(cg_buffer2D<float> &X, cg_buffer2D<float> &Y) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *dp = TEMP.get_data();
      float *sp1 = X.get_data();
      float *sp2 = Y.get_data();

      for (unsigned int i=0;i<(X.get_stride()*X.get_height());i++) {
         *dp = sqrt((*sp1**sp1)+(*sp2**sp2));
         dp++;
         sp1++;
         sp2++;
      }

      return TEMP;
   }

   cg_buffer2D<float> Angle(cg_buffer2D<float> &X, cg_buffer2D<float> &Y) {
      cg_buffer2D<float> TEMP(X.get_width(), X.get_height(), X.get_stride());

      float *dp = TEMP.get_data();
      float *sp1 = X.get_data();
      float *sp2 = Y.get_data();

      for (unsigned int i=0;i<(X.get_stride()*X.get_height());i++) {
         *dp = atan(*sp2 / *sp1);
         dp++;
         sp1++;
         sp2++;
      }

      return TEMP;
   }

};


#endif
