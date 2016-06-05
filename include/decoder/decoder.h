/* 
 * Copyright (c) 2016 Savchenko Yehor
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef  _DECODER_H_
#define  _DECODER_H_

#include "systematic.h"
#include "global.h"
#include "debug.h"
#include "matrixa.h"
#include "decoder/first_phase/first_phase.h"
#include "decoder/second_phase/second_phase.h"
#include "decoder/third_phase/third_phase.h"
#include "decoder/fourth_phase/fourth_phase.h"
#include "decoder/fifth_phase/fifth_phase.h"

/*
 * =====================================================================================
 *        Class:  decoder
 *  Description:  
 * =====================================================================================
 */
struct decoder
{
	// TODO add comments	

	Mat* C;
	Mat D;
	Mat A;
	Mat X;
	Perm Q;
	
	const systematic_indices sys;	
	// TODO add comment
	inline
	decoder (Mat* C, Mat D, const systematic_indices& sys) 
	: C{C}, D{D}, sys{sys}
	{	
	}     

	inline
	bool decode(std::vector<int> map)
	{
		uint32_t i = 0, u = 0;
		// Initialize the matrix A
		matrixa::construct_matrix_A(map, &A, sys, D.rows() - sys.L);
		// construct permutation matrices
		// permutation matrix for column exchange
		Q = Perm(A.cols()); 
		Q.setIdentity();
		// first phase
		first_phase(&A, &D, &Q, sys)(&X, i, u);

		// second phase		
		second_phase(sys, &A, &X, &D, &Q, i, u)();

		// third phase
		third_phase(&A, &D, &X, i)();

		// fourth phase
		fourth_phase(&A, &D, i, u)();

		// fifth phase
		fifth_phase(&A, &D, i)();

		*C = Q * D;

		return true;
	}



	inline
	bool decode()
	{
		uint32_t i = 0, u = 0;
		// Initialize the matrix A
		matrixa::construct_matrix_A(&A, sys, D.rows() - sys.L);
		// construct permutation matrices
		// permutation matrix for column exchange
		Q = Perm(A.cols()); 
		Q.setIdentity();
		// first phase
		first_phase(&A, &D, &Q, sys)(&X, i, u);

		// second phase		
		second_phase(sys, &A, &X, &D, &Q, i, u)();

		// third phase
		third_phase(&A, &D, &X, i)();

		// fourth phase
		fourth_phase(&A, &D, i, u)();

		// fifth phase
		fifth_phase(&A, &D, i)();

		*C = Q * D;

		return true;
	} 

}; /* -----  end of class decoder  ----- */

#endif   /* ----- #ifndef _DECODER_H_  ----- */
