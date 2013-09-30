/*
 * cg_test.hpp
 *
 *  Created on: Sep 9, 2013
 *      Author: luc.martel
 */

#ifndef CG_TEST_HPP_
#define CG_TEST_HPP_

#include <deque>
#include <iostream>

template <class T>
class cg_test_a {
public:
   int public_a;
protected:
   int protected_a;
private:
   int private_a;
};

template <class T>
class cg_test_b : public cg_test_a<T> {
public:
   void a_function() {
      this->public_a = 0;
      this->protected_a = 0;
      //private_a = 0;
   }
};

void TestDeque() {
   std::deque<unsigned int> a_deque;

   a_deque.push_back(10);
   a_deque.push_back(20);
   a_deque.push_back(30);

   std::deque<unsigned int>::iterator it = a_deque.begin();

   for(; it != a_deque.end();++it) {
      unsigned int element = a_deque.front();
      std::cout << element << std::endl;
      a_deque.pop_front();
   }

}


#endif /* CG_TEST_HPP_ */
