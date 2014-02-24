// ---------------------------------------------------------------------------
// Nom du Header : BusisetPath.hh
// Par           : Luc Martel
// Date          : 31 Aout 1999
// Modifie le    : Not Yet
// martel@gel.ulaval.ca
//
// ---------------------------------------------------------------------------
// Description :   Implantation d'une bulle d'air au cerveau. 
// 

#ifndef BUSIESTPATH_HPP___
#define BUSIESTPATH_HPP___

#include <queue>
#include <vector>
#include "../cg_image.hpp"

template <class T>
class cg_busiest_path {
private:
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
   cg_busiest_path() {}
   ~cg_busiest_path() {}

   void
   operator()(cg_buffer2D<T> &X, cg_buffer2D<T> &Y, double Trsh) {

      // Width = to Stride
      cg_buffer2D<double> TEMP(X.get_width(), X.get_height(), X.get_width());
      TEMP.zero();

      // Carte des voisins - Width = to Stride
      cg_buffer2D<unsigned char> NEIGHBORS(X.get_width(), X.get_height(), X.get_width());

      // Carte des pixels etiquettes - Width = to Stride
      cg_buffer2D<unsigned char> TAGED(X.get_width(), X.get_height(), X.get_width());
      TAGED.zero();

      // Create NEIGHBORS image
      // ----------------------------------------------------------------------------------
      unsigned char *ps1 = NEIGHBORS.get_data();

      for (int i=(NEIGHBORS.get_width()*NEIGHBORS.get_height());i>0;i--) {
         *ps1++ = 255;
      }

      ps1 = NEIGHBORS.get_data();
      unsigned char *ps2 = NEIGHBORS.get_data()+(NEIGHBORS.get_height()-1)*NEIGHBORS.get_width();
      for (int i=X.get_width();i>0;i--) {
         *ps1++ = 31;
         *ps2++ = 248;
      }
      ps1 = NEIGHBORS.get_data();
      ps2 = NEIGHBORS.get_data()+NEIGHBORS.get_width()-1;
      for (int i=NEIGHBORS.get_height();i>0;i--) {
         *ps1 = 107;
         *ps2 = 214;
         ps1 += NEIGHBORS.get_width();
         ps2 += NEIGHBORS.get_width();
      }
      *(NEIGHBORS.get_data()) = 11;
      *(NEIGHBORS.get_data()+NEIGHBORS.get_width()-1) = 22;
      *(NEIGHBORS.get_data()+((NEIGHBORS.get_height()-1)*NEIGHBORS.get_width())) = 104;
      *(NEIGHBORS.get_data()+(NEIGHBORS.get_height()*NEIGHBORS.get_width())-1) = 208;
      // ----------------------------------------------------------------------------------

      // Creation des conteneurs pour le Line Field
      cg_buffer2D<double> LineFV(X.get_width()-1,X.get_height(),X.get_width()-1);
      cg_buffer2D<double> LineFH(X.get_width(),X.get_height()-1,X.get_width());

      std::priority_queue<cg_wpix> GrowingQueue;
      std::vector<cg_pix>          GrowingResult;

      // Weighted pixel temporaire
      cg_pix  apix;
      cg_wpix awpix;

      // Effectu un region growing sur chaque pixels
      for (int i=0;i<X.get_height();i++) {
         for (int j=0;j<X.get_width();j++) {
            //cout << i << ":" << j << endl;
            // Place le pixel dans le vecteur
            apix.X = j;
            apix.Y = i;
            GrowingResult.push_back(apix);
            TAGED(i,j) = 255;

            // Push les pixels voisins
            if (NEIGHBORS(i,j) == 255) {
               //awpix.Y = i-1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i-1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j+1));
               //GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j-1)-X(i,j+1));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j+1));
               //GrowingQueue.push(awpix);
            }
            // Haut
            else if (NEIGHBORS(i,j) == 31) {
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j+1));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j+1));
               //GrowingQueue.push(awpix);
            }
            // Bas
            else if (NEIGHBORS(i,j) == 248) {
               //awpix.Y = i-1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i-1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j+1));
               //GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j+1));
               GrowingQueue.push(awpix);
            }
            // Droite
            else if (NEIGHBORS(i,j) == 214) {
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i-1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j+1));
               //GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j+1));
               //GrowingQueue.push(awpix);
            }
            // Gauche
            else if (NEIGHBORS(i,j) == 107) {
               //awpix.Y = i-1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j+1));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
            }
            // Haut-Gauche
            else if (NEIGHBORS(i,j) == 11) {
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j+1));
               GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j+1));
               //GrowingQueue.push(awpix);
            }
            // Haut-Droit
            else if (NEIGHBORS(i,j) == 22) {
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
               //awpix.Y = i+1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i+1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i+1,j));
               GrowingQueue.push(awpix);
            }
            // Bas-Gauche
            else if (NEIGHBORS(i,j) == 104) {
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i-1;
               //awpix.X = j+1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j+1));
               //GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j+1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j+1));
               GrowingQueue.push(awpix);
            }
            // Bas-Droit
            else if (NEIGHBORS(i,j) == 208) {
               awpix.Y = i-1;
               awpix.X = j;
               awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j));
               GrowingQueue.push(awpix);
               //awpix.Y = i-1;
               //awpix.X = j-1;
               //awpix.W = __ABSOLUTE(X(i,j)-X(i-1,j-1));
               //GrowingQueue.push(awpix);
               awpix.Y = i;
               awpix.X = j-1;
               awpix.W = __ABSOLUTE(X(i,j)-X(i,j-1));
               GrowingQueue.push(awpix);
            }

            // -----------------------------------------------
            // -----------------------------------------------
            awpix = GrowingQueue.top();
            int I,J;
            //double sum    = X(i,j);
            //double npixel = 1;
            //double mean   = 0;

            while (awpix.W < Trsh) {
               //cout << GrowingQueue.size() << endl;
               //cout << awpix.X << "::" << awpix.Y << " Weight : "
               //     << awpix.W << endl;
               I = awpix.Y;
               J = awpix.X;
               GrowingQueue.pop();

               // Le pixel fait partie du groupe et et obtient l'etiquette TAGED.
               apix.X = J;
               apix.Y = I;
               GrowingResult.push_back(apix);
               TAGED(I,J) = 255;
               //sum += X(I,J);
               //npixel++;
               //mean = (sum/npixel);


               // Push les pixels voisins
               if (NEIGHBORS(I,J) == 255) {
                  //if (TAGED(I-1,J-1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I-1,J+1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J-1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J+1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
               }
               // Haut
               else if (NEIGHBORS(I,J) == 31) {
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J-1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J+1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
               }
               // Bas
               else if (NEIGHBORS(I,J) == 248) {
                  //if (TAGED(I-1,J-1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I-1,J+1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
               }
               // Droite
               else if (NEIGHBORS(I,J) == 214) {
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(J-1,I));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I-1,J+1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J+1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
               }
               // Gauche
               else if (NEIGHBORS(I,J) == 107) {
                  //if (TAGED(I-1,J-1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J-1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
               }
               // Haut-Gauche
               else if (NEIGHBORS(I,J) == 11) {
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J+1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
               }
               // Haut-Droit
               else if (NEIGHBORS(I,J) == 22) {
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I+1,J-1) == 0) {
                  //awpix.Y = I+1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I+1,J) == 0) {
                     awpix.Y = I+1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I+1,J));
                     GrowingQueue.push(awpix);
                  }
               }
               // Bas-Gauche
               else if (NEIGHBORS(I,J) == 104) {
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I-1,J+1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J+1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J+1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I,J+1) == 0) {
                     awpix.Y = I;
                     awpix.X = J+1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J+1));
                     GrowingQueue.push(awpix);
                  }
               }
               // Bas-Droit
               else if (NEIGHBORS(I,J) == 208) {
                  if (TAGED(I-1,J) == 0) {
                     awpix.Y = I-1;
                     awpix.X = J;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J));
                     GrowingQueue.push(awpix);
                  }
                  //if (TAGED(I-1,J-1) == 0) {
                  //awpix.Y = I-1;
                  //awpix.X = J-1;
                  //awpix.W = __ABSOLUTE(X(I,J)-X(I-1,J-1));
                  //GrowingQueue.push(awpix);
                  //}
                  if (TAGED(I,J-1) == 0) {
                     awpix.Y = I;
                     awpix.X = J-1;
                     awpix.W = __ABSOLUTE(X(I,J)-X(I,J-1));
                     GrowingQueue.push(awpix);
                  }
               }
               awpix = GrowingQueue.top();
            }

            // A ce stage, la GrowingQueue contient les pixels frontieres de
            // l'objet.
            // Le GrowingResult contient les pixels interieurs de l'objet

            // Tag TEMP pour chaque pixels de l'objet
            std::cout << i << " " << j << " Size = " << GrowingResult.size() << "Pixels ." << std::endl;
            for (int i=0;i<GrowingResult.size();i++) {
               // Untag pixel
               TAGED(GrowingResult[i].Y,GrowingResult[i].X) = 0;
            }

            // Clear both vector and queue
            for (int i=0;i<GrowingQueue.size();i++) {
               awpix = GrowingQueue.top();
               TEMP(awpix.Y,awpix.X) += (double)1/(double)GrowingResult.size();
               GrowingQueue.pop();
            }
            GrowingResult.clear();
         }
      }

     TEMP.scale_to_unsignedchar(Y);
   }
};


#endif


