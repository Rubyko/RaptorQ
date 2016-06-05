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

#ifndef  _FIRST_PHASE_H_
#define  _FIRST_PHASE_H_

#include "global.h"
#include "graph.h"
#include "chooser.h"
#include "swapper.h"
#include "row.h"
#include "substractor.h"

// add improvement of parrallel substracting and parallel processing the matrix
struct first_phase
{

	const systematic_indices& sys;	
	Mat*  A;
	Mat*  D;
	Perm* Q;
	improved_chooser chooser;
	improved_swapper swapper;
	substractor sub;

	inline
	void operator()(Mat* X, uint32_t& i, uint32_t& u)
	{
		for(i = 0, u = sys.P; i+u!=sys.L;)
		{
			auto V = A->block(i, i, A->rows() - i, A->cols() - u - i);
			chooser.update(&V);
			row rw = chooser.choose();

			swapper.add(&V, rw, i, u);
			sub.add(rw);

			++i;
			u += rw.r - 1;
		}
		// create matrix X
		*X = *A;
		sub.process();
	}

	inline
	first_phase(Mat* A, Mat*D, Perm* Q, const systematic_indices& sys): 
		sys{sys}, A{A}, D{D}, Q{Q}, chooser(A, sys), swapper(A, D, Q, sys), sub(A, D)
	{
	}

};
	

#endif   /* ----- #ifndef _FIRST_PHASE_H_  ----- */


