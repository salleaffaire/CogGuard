// ---------------------------------------------------------------------------
// Nom du Header : seeds.hh
// Par           : Luc Martel
// Date          : 22 Avril 1999
// Modifie le    : Not Yet
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Methodes de manipulations des racines du regions growing.


#ifndef SEEDS_HPP__
#define SEEDS_HPP__

#define SDROITE 1
#define SGAUCHE 2
#define SHAUT 3
#define SBAS  4

#include <iostream>
#include "../cg_buffer.hpp"


class cg_seeds {
private:
   int oint;
   int onum1,onum2;

   int NDir() {
      if (oint == SDROITE) {
         oint = SBAS;
         return SBAS;
      }
      else if (oint == SBAS) {
         oint = SGAUCHE;
         return SGAUCHE;
      }
      else if (oint == SGAUCHE) {
         oint = SHAUT;
         return SHAUT;
      }
      else if (oint == SHAUT) {
         oint = SDROITE;
         return SDROITE;
      }
   }


   int NNum() {
      if(onum1 == onum2) {
         onum2 = onum1+1;
         return onum1+1;
      }
      else {
         onum1 = onum2;
         return onum2;;
      }
   }

public:

   cg_seeds() {
      oint = SDROITE;
      onum1 = 1;
      onum2 = 1;
   }

   void UniformDiffusion(cg_pix P, int step, int number, int Width,
                         int Height, cg_pix *S) {

      cg_pix *SS = S;

      int CX,CY;
      int count = number;
      oint = SDROITE;
      onum1 = 1;
      onum2 = 1;

      S->X = P.X;
      S->Y = P.Y - step;
      CX = P.X;
      CY = P.Y - step;
      S++;
      count--;

      if (number > 1) {
         CX += step;
         S->X = CX;
         S->Y = CY;
         S++;
      }
      count--;

      int i;
      int ND;
      while (count > 0) {
         ND = NDir();
         for (i=NNum();i>0;i--) {
            if (count > 0) {
               count--;
               if (ND == SDROITE) {
                  CX += step;
                  S->X = CX;
                  S->Y = CY;
                  S++;
               }
               if (ND == SGAUCHE) {
                  CX -= step;
                  S->X = CX;
                  S->Y = CY;
                  S++;
               }
               if (ND == SBAS) {
                  CY += step;
                  S->X = CX;
                  S->Y = CY;
                  S++;
               }
               if (ND == SHAUT) {
                  CY -= step;
                  S->X = CX;
                  S->Y = CY;
                  S++;
               }
            }
         }
      }

      S = SS;
      for (i=0;i<number;i++) {
         if (S->X < 0) {
            S->X += (step*(onum2+1));
         }
         if (S->Y < 0) {
            S->Y += (step*(onum2+1));
         }
         if (S->X > Width) {
            S->X -= (step*(onum2+1));
         }
         if (S->Y > Height) {
            S->Y -= (step*(onum2+1));
         }
         S++;
      }
   }

   void Print(cg_pix S[], int number)
   {
      for (int i = 0; i<number; i++) {
         std::cout << "X : " << S->X << std::endl;
         std::cout << "Y : " << S->Y << std::endl;
         std::cout << "___________" << std::endl << std::endl;
         S++;
      }

   }
};
#endif
