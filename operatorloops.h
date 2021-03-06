/*                                                                           
Developed by Sandeep Sharma and Garnet K.-L. Chan, 2012                      
Copyright (c) 2012, Garnet K.-L. Chan                                        
                                                                             
This program is integrated in Molpro with the permission of 
Sandeep Sharma and Garnet K.-L. Chan
*/


#ifndef SPIN_OPERATOR_LOOPS_HEADER_H
#define SPIN_OPERATOR_LOOPS_HEADER_H
#include <vector>
#include <iostream>
#include <communicate.h>
#include <omp.h>
#include <boost/shared_ptr.hpp>


/**
 * Distributed loops to be used functors on OperatorArrays
 * 
 */

/**
 * Loop over all local (i.e. stored on current processor) in array
 * 
 */


namespace SpinAdapted{
class SpinBlock;
//********************************************************************************************
//loop over all local operators and build them
// different version include multithread build, single thread build and single thread build from csf

template<class A> void singlethread_build(A& array, SpinBlock& b, std::vector< Csf >& s, vector< vector<Csf> >& ladders)
{
#pragma omp parallel default(shared)
#pragma omp for schedule(guided) nowait
  for (int i = 0; i < array.get_size(); ++i) {
    //typedef typename A::OpType Op;
    std::vector<boost::shared_ptr<SparseMatrix> > vec = array.get_local_element(i);
    for (int j=0; j<vec.size(); j++)
      vec[j]->buildUsingCsf(b, ladders, s);
  }
}

template<class A> void singlethread_build(A& array, SpinBlock& b)
{
#pragma omp parallel default(shared)
#pragma omp for schedule(guided) nowait
  for (int i = 0; i < array.get_size(); ++i) {
    //typedef typename A::OpType Op;
    //std::vector<boost::shared_ptr<Op> >& vec = array.get_local_element(i);
    std::vector<boost::shared_ptr<SparseMatrix> > vec = array.get_local_element(i);
    for (int j=0; j<vec.size(); j++)
      vec[j]->build(b);
  }
}
//*****************************************************************************


//****************************************************************************
//execute a function on all elements of an array
//single thread and multithread versions of the code
template<typename T2, class A> void for_all_singlethread(A& array, const T2& func)
{
  int i;
  {
    for (i = 0; i < array.get_size(); ++i) {
      std::vector<boost::shared_ptr<SparseMatrix> > vec = array.get_local_element(i);
      func(vec);
    }
  }
}

template<typename T2, class A> void for_all_multithread(A& array, const T2& func)
{
#pragma omp parallel default(shared)
#pragma omp for schedule(guided) nowait
    for (int i = 0; i < array.get_size(); ++i) {
      std::vector<boost::shared_ptr<SparseMatrix> > vec = array.get_local_element(i);
      func(vec);
    }
}
 
template<typename T2, class A> void for_all_operators_multithread(A& array, const T2& func)
{
  int i;
  #pragma omp parallel default(shared) private(i)
  {
    #pragma omp for schedule(guided) nowait
    for (i = 0; i < array.get_size(); ++i) {
      std::vector<boost::shared_ptr<SparseMatrix> > vec = array.get_local_element(i);
      for (int j=0; j<vec.size(); j++){
	func(*vec[j]);
      }
    }
  }
}

//*****************************************************************************


}
#endif


