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

#include <math.h>
#include "cg_edgeex.hpp"

template <class T>
class CCanny : public CEdgeEx<T> {
private:
   float G3[3];
   float DG3[5];
public:
   CCanny() {
      G3[0] = 0.25;
      G3[1] = 0.5;
      G3[2] = 0.25;

      DG3[0] = 0.25;
      DG3[1] = 0.75;
      DG3[2] = 0;
      DG3[3] = -0.75;
      DG3[4] = -0.25;

   }
   ~CCanny() {}

   cg_buffer2D<unsigned char> ExtractEdge(cg_buffer2D<T> &X);
   cg_buffer2D<double>        ExtractAngle(cg_buffer2D<T> &X);

   cg_buffer2D<float>         GaussianX(cg_buffer2D<T> &X);
   cg_buffer2D<float>         GaussianY(cg_buffer2D<T> &X);
   cg_buffer2D<float>         DiffGausX(cg_buffer2D<T> &X);
   cg_buffer2D<float>         DiffGausY(cg_buffer2D<T> &X);
   cg_buffer2D<float>         GaussianX(cg_buffer2D<float> &X);
   cg_buffer2D<float>         GaussianY(cg_buffer2D<float> &X);
   cg_buffer2D<float>         DiffGausX(cg_buffer2D<float> &X);
   cg_buffer2D<float>         DiffGausY(cg_buffer2D<float> &X);

   cg_buffer2D<float>         Amplitude(cg_buffer2D<float> &X,
         cg_buffer2D<float> &Y);
   cg_buffer2D<float>         Angle(cg_buffer2D<float> &X,
         cg_buffer2D<float> &Y);

};

template <class T>
cg_buffer2D<unsigned char> CCanny<T>::ExtractEdge(cg_buffer2D<T> &X)
{
   cg_buffer2D<float> GRADX = DiffGausX(X);
   cg_buffer2D<float> GRADY = GaussianX(X);

   GRADX = GaussianY(GRADX);
   GRADY = DiffGausY(GRADY);

   cg_buffer2D<float> GAMP = Amplitude(GRADX, GRADY);

   cg_buffer2D<unsigned char> TEMP = GAMP.scale_to_unsignedchar();
   return TEMP;
}

template <class T>
cg_buffer2D<double> CCanny<T>::ExtractAngle(cg_buffer2D<T> &X)
{
   cg_buffer2D<float> GRADX = DiffGausX(X);
   cg_buffer2D<float> GRADY = GaussianX(X);

   GRADX = GaussianY(GRADX);
   GRADY = DiffGausY(GRADY);

   cg_buffer2D<float> GANG = Angle(GRADX, GRADY);

   cg_buffer2D<double> TEMP = GANG.to_double();
   return TEMP;
}

template <class T>
cg_buffer2D<float> CCanny<T>::GaussianX(cg_buffer2D<T> &X)
{
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   T *Tp1 = X.get_data();
   T *Tp2 = X.get_data()+1;
   T *Tp3 = X.get_data()+2;
   float *fp = TEMP.get_data()+1;

   for(int i=0;i<X.mHeight;i++) {
      for (int j=0;j<X.mStride-2;j++) {
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

template <class T>
cg_buffer2D<float> CCanny<T>::GaussianY(cg_buffer2D<T> &X)
{
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   T *fp1 = X.get_data();
   T *fp2 = X.get_data()+X.mStride;
   T *fp3 = X.get_data()+(X.mStride*2);
   float *fp = TEMP.get_data()+(X.mStride);

   for(int i=0;i<X.mStride;i++) {
      for (int j=0;j<X.mHeight-2;j++) {
         *fp = G3[0]*(float)*fp1 + G3[1]*(float)*fp2 + G3[2]*(float)*fp3;
         fp += X.mStride;
         fp1 += X.mStride;
         fp2 += X.mStride;
         fp3 += X.mStride;
      }
      fp1 = X.get_data()+i;;
      fp2 = X.get_data()+X.mStride+i;
      fp3 = X.get_data()+(X.mStride*2)+i;
      fp = TEMP.get_data()+(X.mStride)+i;
   }

   return TEMP;
}


template <class T>
cg_buffer2D<float> CCanny<T>::DiffGausX(cg_buffer2D<T> &X) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   T *Tp1 = X.get_data();
   T *Tp2 = X.get_data()+1;
   T *Tp3 = X.get_data()+2;
   T *Tp4 = X.get_data()+3;
   T *Tp5 = X.get_data()+4;

   float *fp = TEMP.get_data()+2;

   for(int i=0;i<X.mHeight;i++) {
      for (int j=0;j<X.mStride-4;j++) {
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

template <class T>
cg_buffer2D<float> CCanny<T>::DiffGausY(cg_buffer2D<T> &X) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   T *fp1 = X.get_data();
   T *fp2 = X.get_data()+X.mStride;
   T *fp3 = X.get_data()+(X.mStride*2);
   T *fp4 = X.get_data()+(X.mStride*3);
   T *fp5 = X.get_data()+(X.mStride*4);

   float *fp = TEMP.get_data()+(X.mStride*2);

   for(int i=0;i<X.mStride;i++) {
      for (int j=0;j<X.mHeight-4;j++) {
         *fp = DG3[0]*(float)*fp1 + DG3[1]*(float)*fp2 + DG3[2]*(float)*fp3 +
               DG3[3]*(float)*fp4 + DG3[4]*(float)*fp5;
         fp += X.mStride;
         fp1 += X.mStride;
         fp2 += X.mStride;
         fp3 += X.mStride;
         fp4 += X.mStride;
         fp5 += X.mStride;
      }
      fp1 = X.get_data()+i;
      fp2 = X.get_data()+X.mStride+i;
      fp3 = X.get_data()+(X.mStride*2)+i;
      fp4 = X.get_data()+(X.mStride*3)+i;
      fp5 = X.get_data()+(X.mStride*4)+i;
      fp = TEMP.get_data()+(X.mStride*2)+i;
   }
   return TEMP;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

template <class T>
cg_buffer2D<float> CCanny<T>::GaussianX(cg_buffer2D<float> &X) {

   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *Tp1 = X.get_data();
   float *Tp2 = X.get_data()+1;
   float *Tp3 = X.get_data()+2;

   float *fp = TEMP.get_data()+1;

   for(int i=0;i<X.mHeight;i++) {
      for (int j=0;j<X.mStride-2;j++) {
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

template <class T>
cg_buffer2D<float> CCanny<T>::GaussianY(cg_buffer2D<float> &X) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *fp1 = X.get_data();
   float *fp2 = X.get_data()+X.mStride;
   float *fp3 = X.get_data()+(X.mStride*2);

   float *fp = TEMP.get_data()+(X.mStride);

   for(int i=0;i<X.mStride;i++) {
      for (int j=0;j<X.mHeight-2;j++) {
         *fp = G3[0]*(float)*fp1 + G3[1]*(float)*fp2 + G3[2]*(float)*fp3;
         fp += X.mStride;
         fp1 += X.mStride;
         fp2 += X.mStride;
         fp3 += X.mStride;
      }
      fp1 = X.get_data()+i;
      fp2 = X.get_data()+X.mStride+i;
      fp3 = X.get_data()+(X.mStride*2)+i;
      fp = TEMP.get_data()+(X.mStride)+i;
   }

   return TEMP;
}

template <class T>
cg_buffer2D<float> CCanny<T>::DiffGausX(cg_buffer2D<float> &X) {

   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *Tp1 = X.get_data();
   float *Tp2 = X.get_data()+1;
   float *Tp3 = X.get_data()+2;
   float *Tp4 = X.get_data()+3;
   float *Tp5 = X.get_data()+4;

   float *fp = TEMP.get_data()+2;

   for(int i=0;i<X.mHeight;i++) {
      for (int j=0;j<X.mStride-4;j++) {
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

template <class T>
cg_buffer2D<float> CCanny<T>::DiffGausY(cg_buffer2D<float> &X) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *fp1 = X.get_data();
   float *fp2 = X.get_data()+X.mStride;
   float *fp3 = X.get_data()+(X.mStride*2);
   float *fp4 = X.get_data()+(X.mStride*3);
   float *fp5 = X.get_data()+(X.mStride*4);

   float *fp = TEMP.get_data()+(X.mStride*2);

   for(int i=0;i<X.mStride;i++) {
      for (int j=0;j<X.mHeight-4;j++) {
         *fp = DG3[0]*(float)*fp1 + DG3[1]*(float)*fp2 + DG3[2]*(float)*fp3 +
               DG3[3]*(float)*fp4 + DG3[4]*(float)*fp5;
         fp += X.mStride;
         fp1 += X.mStride;
         fp2 += X.mStride;
         fp3 += X.mStride;
         fp4 += X.mStride;
         fp5 += X.mStride;
      }
      fp1 = X.get_data()+i;
      fp2 = X.get_data()+X.mStride+i;
      fp3 = X.get_data()+(X.mStride*2)+i;
      fp4 = X.get_data()+(X.mStride*3)+i;
      fp5 = X.get_data()+(X.mStride*4)+i;
      fp = TEMP.get_data()+(X.mStride*2)+i;
   }
   return TEMP;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
template <class T>
cg_buffer2D<float> CCanny<T>::Amplitude(cg_buffer2D<float> &X, cg_buffer2D<float> &Y) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *dp = TEMP.get_data();
   float *sp1 = X.get_data();
   float *sp2 = Y.get_data();

   for (int i=0;i<(X.mStride*X.mHeight);i++) {
      *dp = sqrt((*sp1**sp1)+(*sp2**sp2));
      dp++;
      sp1++;
      sp2++;
   }

   return TEMP;
}

template <class T>
cg_buffer2D<float> CCanny<T>::Angle(cg_buffer2D<float> &X, cg_buffer2D<float> &Y) {
   cg_buffer2D<float> TEMP(X.mWidth, X.mHeight, X.mStride);

   float *dp = TEMP.get_data();
   float *sp1 = X.get_data();
   float *sp2 = Y.get_data();

   for (int i=0;i<(X.mStride*X.mHeight);i++) {
      *dp = atan(*sp2 / *sp1);
      dp++;
      sp1++;
      sp2++;
   }

   return TEMP;
}


#endif
