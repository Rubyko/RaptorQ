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

#ifndef  _SECOND_PHASE_H_
#define  _SECOND_PHASE_H_

#include "global.h"
#include "gauss.h"
#include "systematic.h"

struct second_phase 
{

	const systematic_indices sys;
	Mat* const A;
	Mat* const X;
	Mat* 	   D;
	Perm*      Q;
	const uint32_t i;
	const uint32_t u;

	inline
	bool operator()()
	{
		X->conservativeResize(i, i);
		// Gaussian eliminatin performed on U_lower
		gauss gss(A, D, i, u);
		if(!gss.perform())
			return false;
		A->conservativeResize(sys.L, sys.L);
		D->conservativeResize(sys.L, D->cols());
		return true;
	}

	inline
	second_phase(const systematic_indices& sys, Mat* A, Mat* X, Mat* D, Perm* Q, uint32_t i, uint32_t u) :
		sys{sys}, A{A}, X{X}, D{D}, Q{Q}, i{i}, u{u}
	{
	}

};


#endif   /* ----- #ifndef _SECOND_PHASE_H_  ----- */
