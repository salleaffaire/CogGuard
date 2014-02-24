// ---------------------------------------------------------------------------
// Nom du Header : reggrow.hh
// Par           : Luc Martel
// Date          : 25 Mars 1999
// Modifie le    : Not Yet
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Implantation d'un region growing base a partir de 
//                 L'article de Adams et Bischof

#ifndef _REGGROW__HPP__
#define _REGGROW__HPP__

#define BOUNDARY_LABEL 255

#include <iostream>
#include <math.h>
#include "../cg_image.hpp"
#include "../cg_sorted_stack.hpp"

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//                          Seeded Region Growing
// ---------------------------------------------------------------------------
template <class T>
class cg_seedreggrowing {
private:
   CSortedStackInv<cg_wpix> WPStack;

   // Fonction de valeur absolu private
   inline double __ABSOLUTE(double x)
   {
      if (x >= 0) {
         return x;
      }
      else {
         return -x;
      }
   }

public:
   cg_seedreggrowing() {}
   ~cg_seedreggrowing() {}

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   //                  Region growing based on intensity
   // ---------------------------------------------------------------------------
   void MeanGrow(cg_buffer2D<T> &X, cg_buffer2D<unsigned int> &Y, int N, cg_pix pix[]) {

      // Only works if stride is same as width
      // TODO: Genaralize
      if (X.get_stride() != X.get_width()) {
         std::cout << "Stride must equal Width to run CSeedRegGrowing<T>::MeanGrow" << std::endl;
         return;
      }

      cg_buffer2D<unsigned char> TEMP(X.get_width(), X.get_height(), X.get_stride());
      cg_buffer2D<unsigned char> PIL(X.get_width(), X.get_height(), X.get_stride());
      cg_buffer2D<unsigned char> NEIGHBORS(X.get_width(), X.get_height(), X.get_stride());

      double *Means = new double [N];
      int    *NumberOfPoints = new int [N];
      int    TX,TY;

      int    VX[8];
      int    VY[8];

      // Create NEIGHBORS image
      unsigned char *ps1 = NEIGHBORS.get_data();
      for (int i=(X.get_width()*X.get_height());i>0;i--) {
         *ps1++ = 255;
      }
      ps1 = NEIGHBORS.get_data();
      unsigned char *ps2 = NEIGHBORS.get_data()+(X.get_height()-1)*X.get_width();
      for (int i=X.get_width();i>0;i--) {
         *ps1++ = 31;
         *ps2++ = 248;
      }
      ps1 = NEIGHBORS.get_data();
      ps2 = NEIGHBORS.get_data()+X.get_width()-1;
      for (int i=X.get_height();i>0;i--) {
         *ps1 = 107;
         *ps2 = 214;
         ps1 += X.get_width();
         ps2 += X.get_width();
      }
      *(NEIGHBORS.get_data()) = 11;
      *(NEIGHBORS.get_data()+X.get_width()-1) = 22;
      *(NEIGHBORS.get_data()+((X.get_height()-1)*X.get_width())) = 104;
      *(NEIGHBORS.get_data()+(X.get_height()*X.get_width())-1) = 208;

      // Initialization
      for (int i=0;i<N;i++) {
         TX = pix[i].X;
         TY = pix[i].Y;
         if (TX == 0) {TX++;}
         if (TY == 0) {TY++;}
         if (TX == (X.get_width()-1)) {TX--;}
         if (TY == (X.get_width()-1)) {TY--;}
         TEMP(TY,TX) = (i+1);
         Means[i] = X(TY,TX);
         NumberOfPoints[i] = 1;
      }

      // Place les voisins dans la liste
      for (int i=0;i<N;i++) {
         TX = pix[i].X;
         TY = pix[i].Y;
         std::cout << "X:" << TX << " Y:" << TY << std::endl;
         if (TX == 0) {TX++;}
         if (TY == 0) {TY++;}
         if (TX == (X.get_width()-1)) {TX--;}
         if (TY == (X.get_height()-1)) {TY--;}

         cg_wpix awpix;

         if (PIL(TY-1,TX-1) == 0) {
            PIL(TY-1,TX-1) = 1;
            awpix = CWPix(TX-1,TY-1,__ABSOLUTE(X(TY-1,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY-1,TX) == 0) {
            PIL(TY-1,TX) = 1;
            awpix = CWPix(TX,TY-1,__ABSOLUTE(X(TY-1,TX)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY-1,TX+1) == 0) {
            PIL(TY-1,TX+1) = 1;
            awpix = CWPix(TX+1,TY-1,__ABSOLUTE(X(TY-1,TX+1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY,TX-1) == 0) {
            PIL(TY,TX-1) = 1;
            awpix = CWPix(TX-1,TY,__ABSOLUTE(X(TY,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY,TX+1) == 0) {
            PIL(TY,TX+1) = 1;
            awpix = CWPix(TX+1,TY,__ABSOLUTE(X(TY,TX+1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX-1) == 0) {
            PIL(TY+1,TX-1) = 1;
            awpix = CWPix(TX-1,TY+1,__ABSOLUTE(X(TY+1,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX) == 0) {
            PIL(TY+1,TX) = 1;
            awpix = CWPix(TX,TY+1,__ABSOLUTE(X(TY+1,TX)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX+1) == 0) {
            PIL(TY+1,TX+1) = 1;
            awpix = CWPix(TX+1,TY+1,__ABSOLUTE(X(TY+1,TX+1)-Means[i]));
            WPStack.Push(awpix);
         }
      }

      unsigned char curNB = 0;
      unsigned char curLB[8] = {0,0,0,0,0,0,0,0};
      unsigned char curTL = 0;
      unsigned char curTS = 0;
      double        me    = 0;

      // While SortedStack is not Empty
      while (WPStack.NumberOfItem != 0) {
         cg_wpix awpix;
         awpix = WPStack.Pop();
         TX = awpix.X;
         TY = awpix.Y;
         PIL(TY,TX) = 0;
         curNB = NEIGHBORS(TY,TX);
         curLB[0] = 0;
         curLB[1] = 0;
         curLB[2] = 0;
         curLB[3] = 0;
         curLB[4] = 0;
         curLB[5] = 0;
         curLB[6] = 0;
         curLB[7] = 0;
         curTL = 0;
         curTS = 0;
         if (curNB & 128) {
            curLB[0] = TEMP(TY-1,TX-1);
         }
         if (curNB & 64) {
            curLB[1] = TEMP(TY-1,TX);
         }
         if (curNB & 32) {
            curLB[2] = TEMP(TY-1,TX+1);
         }
         if (curNB & 16) {
            curLB[3] = TEMP(TY,TX-1);
         }
         if (curNB & 8) {
            curLB[4] = TEMP(TY,TX+1);
         }
         if (curNB & 4) {
            curLB[5] = TEMP(TY+1,TX-1);
         }
         if (curNB & 2) {
            curLB[6] = TEMP(TY+1,TX);
         }
         if (curNB & 1) {
            curLB[7] = TEMP(TY+1,TX+1);
         }
         for (int i=0;i<8;i++) {
            if ((curLB[i] != 0) && (curLB[i] != BOUNDARY_LABEL)) {
               if (curTL != 0) {
                  if (curTL != curLB[i]) {
                     curTS = 1;
                  }
               }
               else {
                  curTL = curLB[i];
               }
            }
         }
         if (curTS == 1) {
            TEMP(TY,TX) = BOUNDARY_LABEL;
         }
         else {
            // Tag a New Point
            TEMP(TY,TX) = curTL;
            // Update the Mean of the region
            Means[curTL-1] += X(TY,TX);
            NumberOfPoints[curTL-1] += 1;
            me = (Means[curTL-1]/(double)NumberOfPoints[curTL-1]);
            // Place neighbors in SortedStack
            if (curNB & 128) {
               if ((PIL(TY-1,TX-1) == 0) && (TEMP(TY-1,TX-1) == 0)) {
                  awpix = CWPix(TX-1,TY-1,__ABSOLUTE(X(TY-1,TX-1)-me));
                  WPStack.Push(awpix);
                  PIL(TY-1,TX-1) = 1;
               }
            }
            if (curNB & 64) {
               if ((PIL(TY-1,TX) == 0) && (TEMP(TY-1,TX) == 0)) {
                  awpix = CWPix(TX,TY-1,__ABSOLUTE(X(TY-1,TX)-me));
                  WPStack.Push(awpix);
                  PIL(TY-1,TX) = 1;
               }
            }
            if (curNB & 32) {
               if ((PIL(TY-1,TX+1) == 0) && (TEMP(TY-1,TX+1) == 0)) {
                  awpix = CWPix(TX+1,TY-1,__ABSOLUTE(X(TY-1,TX+1)-me));
                  WPStack.Push(awpix);
                  PIL(TY-1,TX+1) = 1;
               }
            }
            if (curNB & 16) {
               if ((PIL(TY,TX-1) == 0) && (TEMP(TY,TX-1) == 0)) {
                  awpix = CWPix(TX-1,TY,__ABSOLUTE(X(TY,TX-1)-me));
                  WPStack.Push(awpix);
                  PIL(TY,TX-1) = 1;
               }
            }
            if (curNB & 8) {
               if ((PIL(TY,TX+1) == 0) && (TEMP(TY,TX+1) == 0)) {
                  awpix = CWPix(TX+1,TY,__ABSOLUTE(X(TY,TX+1)-me));
                  WPStack.Push(awpix);
                  PIL(TY,TX+1) = 1;
               }
            }
            if (curNB & 4) {
               if ((PIL(TY+1,TX-1) == 0) && (TEMP(TY+1,TX-1) == 0)) {
                  awpix = CWPix(TX-1,TY+1,__ABSOLUTE(X(TY+1,TX-1)-me));
                  WPStack.Push(awpix);
                  PIL(TY+1,TX-1) = 1;
               }
            }
            if (curNB & 2) {
               if ((PIL(TY+1,TX) == 0) && (TEMP(TY+1,TX) == 0)) {
                  awpix = CWPix(TX,TY+1,__ABSOLUTE(X(TY+1,TX)-me));
                  WPStack.Push(awpix);
                  PIL(TY+1,TX) = 1;
               }
            }
            if (curNB & 1) {
               if ((PIL(TY+1,TX+1) == 0) && (TEMP(TY+1,TX+1) == 0)) {
                  awpix = CWPix(TX+1,TY+1,__ABSOLUTE(X(TY+1,TX+1)-me));
                  WPStack.Push(awpix);
                  PIL(TY+1,TX+1) = 1;
               }
            }
         }
      }

      delete []NumberOfPoints;
      delete []Means;

      TEMP.to_uint(Y);
   }


   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   //                Weighted region growing based on weighted intensity
   // ---------------------------------------------------------------------------
   void WMeanGrow(cg_buffer2D<T> &X,
                  cg_buffer2D<unsigned char> &Y,
                  int N,
                  cg_pix pix[]) {

      // Only works if stride is same as width
      // TODO: Genaralize
      if (X.get_stride() != X.get_width()) {
         std::cout << "Stride must equal Width to run CSeedRegGrowing<T>::WMeanGrow" << std::endl;
         return;
      }

      Y.zero();

      cg_buffer2D<unsigned char> PIL(X.get_width(), X.get_height(), X.get_stride());
      cg_buffer2D<unsigned char> NEIGHBORS(X.get_width(), X.get_height(), X.get_stride());


      double *Means = new double [N];
      int    *NumberOfPoints = new int [N];
      int    TX,TY;

      int    VX[8];
      int    VY[8];

      // Create NEIGHBORS image
      unsigned char *ps1 = NEIGHBORS.get_data();
      for (int i=(X.get_width()*X.get_height());i>0;i--) {
         *ps1++ = 255;
      }
      ps1 = NEIGHBORS.get_data();
      unsigned char *ps2 = NEIGHBORS.get_data()+(X.get_height()-1)*X.X.get_width();
      for (int i=X.X.get_width();i>0;i--) {
         *ps1++ = 31;
         *ps2++ = 248;
      }
      ps1 = NEIGHBORS.get_data();
      ps2 = NEIGHBORS.get_data()+X.X.get_height()()-1;
      for (int i=X.get_height();i>0;i--) {
         *ps1 = 107;
         *ps2 = 214;
         ps1 += X.X.get_width();
         ps2 += X.X.get_width();
      }
      *(NEIGHBORS.get_data()) = 11;
      *(NEIGHBORS.get_data()+X.X.get_width()-1) = 22;
      *(NEIGHBORS.get_data()+((X.get_height()-1)*X.X.get_width())) = 104;
      *(NEIGHBORS.get_data()+(X.get_height()*X.X.get_width())-1) = 208;

      // Initialization
      for (int i=0;i<N;i++) {
         TX = pix[i].X;
         TY = pix[i].Y;
         if (TX == 0) {TX++;}
         if (TY == 0) {TY++;}
         if (TX == (X.get_width()-1)) {TX--;}
         if (TY == (X.get_height()-1)) {TY--;}
         Y(TY,TX) = (i+1);
         Means[i] = X(TY,TX);
         NumberOfPoints[i] = 1;
      }

      double Weight = 0;
      // Place les voisins dans la liste
      for (int i=0;i<N;i++) {
         TX = pix[i].X;
         TY = pix[i].Y;
         // std::cout << "X:" << TX << " Y:" << TY << std::endl;
         if (TX == 0) {TX++;}
         if (TY == 0) {TY++;}
         if (TX == (X.get_width()-1)) {TX--;}
         if (TY == (X.get_height()-1)) {TY--;}

         cg_wpix awpix;
         Weight = 0;

         if (PIL(TY-1,TX-1) == 0) {
            PIL(TY-1,TX-1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY-2,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            awpix = cg_wpix(TX-1,TY-1,__ABSOLUTE(X(TY-1,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY-1,TX) == 0) {
            PIL(TY-1,TX) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY-2,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX+1,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+1,1)-Means[i]));
            awpix = cg_wpix(TX,TY-1,__ABSOLUTE(X(TY-1,TX)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY-1,TX+1) == 0) {
            PIL(TY-1,TX+1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY-2,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX+2,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-2,TX+2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+2,1)-Means[i]));
            awpix = cg_wpix(TX+1,TY-1,__ABSOLUTE(X(TY-1,TX+1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY,TX-1) == 0) {
            PIL(TY,TX-1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY-1,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX,1)-Means[i]));
            awpix = cg_wpix(TX-1,TY,__ABSOLUTE(X(TY,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY,TX+1) == 0) {
            PIL(TY,TX+1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY+1,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+2,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY-1,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX+2,1)-Means[i]));
            awpix = cg_wpix(TX+1,TY,__ABSOLUTE(X(TY,TX+1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX-1) == 0) {
            PIL(TY+1,TX-1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX-2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX,1)-Means[i]));
            awpix = cg_wpix(TX-1,TY+1,__ABSOLUTE(X(TY+1,TX-1)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX) == 0) {
            PIL(TY+1,TX) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY+2,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX-1,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX-1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX+1,1)-Means[i]));
            awpix = cg_wpix(TX,TY+1,__ABSOLUTE(X(TY+1,TX)-Means[i]));
            WPStack.Push(awpix);
         }
         if (PIL(TY+1,TX+1) == 0) {
            PIL(TY+1,TX+1) = 1;
            Weight = 0.125*(__ABSOLUTE(X(TY+2,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+1,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX+2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+1,TX,1)-Means[i]));

            Weight += 0.125*(__ABSOLUTE(X(TY,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY,TX+2,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX,1)-Means[i]));
            Weight += 0.125*(__ABSOLUTE(X(TY+2,TX+2,1)-Means[i]));
            awpix = cg_wpix(TX+1,TY+1,__ABSOLUTE(X(TY+1,TX+1)-Means[i])+Weight);
            WPStack.Push(awpix);
         }
      }

      unsigned char curNB = 0;
      unsigned char curLB[8] = {0,0,0,0,0,0,0,0};
      unsigned char curTL = 0;
      unsigned char curTS = 0;
      double        me    = 0;

      // While SortedStack is not Empty
      while (WPStack.NumberOfItem != 0) {
         cg_wpix awpix;
         awpix = WPStack.Pop();
         TX = awpix.X;
         TY = awpix.Y;
         PIL(TY,TX) = 0;
         curNB = NEIGHBORS(TY,TX);
         curLB[0] = 0;
         curLB[1] = 0;
         curLB[2] = 0;
         curLB[3] = 0;
         curLB[4] = 0;
         curLB[5] = 0;
         curLB[6] = 0;
         curLB[7] = 0;
         curTL = 0;
         curTS = 0;
         if (curNB & 128) {
            curLB[0] = Y(TY-1,TX-1);
         }
         if (curNB & 64) {
            curLB[1] = Y(TY-1,TX);
         }
         if (curNB & 32) {
            curLB[2] = Y(TY-1,TX+1);
         }
         if (curNB & 16) {
            curLB[3] = Y(TY,TX-1);
         }
         if (curNB & 8) {
            curLB[4] = Y(TY,TX+1);
         }
         if (curNB & 4) {
            curLB[5] = Y(TY+1,TX-1);
         }
         if (curNB & 2) {
            curLB[6] = Y(TY+1,TX);
         }
         if (curNB & 1) {
            curLB[7] = Y(TY+1,TX+1);
         }
         for (int i=0;i<8;i++) {
            if ((curLB[i] != 0) && (curLB[i] != BOUNDARY_LABEL)) {
               if (curTL != 0) {
                  if (curTL != curLB[i]) {
                     curTS = 1;
                  }
               }
               else {
                  curTL = curLB[i];
               }
            }
         }
         if (curTS == 1) {
            Y(TY,TX) = BOUNDARY_LABEL;
         }
         else {
            // Tag a New Point
            Y(TY,TX) = curTL;
            // Update the Mean of the region
            Means[curTL-1] += X(TY,TX);
            NumberOfPoints[curTL-1] += 1;
            me = (Means[curTL-1]/(double)NumberOfPoints[curTL-1]);
            Weight = 0;
            // Place neighbors in SortedStack
            if (curNB & 128) {
               if ((PIL(TY-1,TX-1) == 0) && (Y(TY-1,TX-1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY-2,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX-2,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX-1,1)-me));
                  awpix = CWPix(TX-1,TY-1,__ABSOLUTE(X(TY-1,TX-1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY-1,TX-1) = 1;
               }
            }
            if (curNB & 64) {
               if ((PIL(TY-1,TX) == 0) && (Y(TY-1,TX) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY-2,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX+1,1)-me));
                  awpix = CWPix(TX,TY-1,__ABSOLUTE(X(TY-1,TX)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY-1,TX) = 1;
               }
            }
            if (curNB & 32) {
               if ((PIL(TY-1,TX+1) == 0) && (Y(TY-1,TX+1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY-2,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX+2,1)-me));
                  awpix = CWPix(TX+1,TY-1,__ABSOLUTE(X(TY-1,TX+1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY-1,TX+1) = 1;
               }
            }
            if (curNB & 16) {
               if ((PIL(TY,TX-1) == 0) && (Y(TY,TX-1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY-1,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX-2,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX,1)-me));
                  awpix = CWPix(TX-1,TY,__ABSOLUTE(X(TY,TX-1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY,TX-1) = 1;
               }
            }
            if (curNB & 8) {
               if ((PIL(TY,TX+1) == 0) && (Y(TY,TX+1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY+1,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY-1,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX+2,1)-me));
                  awpix = CWPix(TX+1,TY,__ABSOLUTE(X(TY,TX+1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY,TX+1) = 1;
               }
            }
            if (curNB & 4) {
               if ((PIL(TY+1,TX-1) == 0) && (Y(TY+1,TX-1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+2,TX-1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX-2,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX,1)-me));
                  awpix = CWPix(TX-1,TY+1,__ABSOLUTE(X(TY+1,TX-1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY+1,TX-1) = 1;
               }
            }
            if (curNB & 2) {
               if ((PIL(TY+1,TX) == 0) && (Y(TY+1,TX) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY+2,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX-1,1)-me));
                  awpix = CWPix(TX,TY+1,__ABSOLUTE(X(TY+1,TX)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY+1,TX) = 1;
               }
            }
            if (curNB & 1) {
               if ((PIL(TY+1,TX+1) == 0) && (Y(TY+1,TX+1) == 0)) {
                  Weight = 0.25*(__ABSOLUTE(X(TY+2,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY,TX+1,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX+2,1)-me));
                  Weight += 0.25*(__ABSOLUTE(X(TY+1,TX,1)-me));
                  awpix = CWPix(TX+1,TY+1,__ABSOLUTE(X(TY+1,TX+1)-me)+Weight);
                  WPStack.Push(awpix);
                  PIL(TY+1,TX+1) = 1;
               }
            }
         }
      }


      delete []NumberOfPoints;
      delete []Means;
   }

   void FreeStack()
   {
      cg_wpix awp;
      while (WPStack.NumberOfItem != 0) {
         awp = WPStack.Pop();
         std::cout << "Pixels " << awp.Y << ":" << awp.X << " Poids de : " << awp.W
               << std::endl;
      }
   }
};



#endif

