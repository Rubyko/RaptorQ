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


#ifndef  _THIRD_PHASE_H_
#define  _THIRD_PHASE_H_

#include "global.h"

struct third_phase {

	Mat* const A;
	Mat* const D;
	Mat* const X;
	uint32_t i;

	// TODO multiplication alingment
	inline
	bool operator()()
	{
		auto A_blck  = A->topRows(i);
		A_blck  = (*X) * A_blck;

		auto D_block = D->topRows(i);
		D_block = (*X) * D_block;
		return true;
	}

	inline
	third_phase(Mat* A, Mat* D, Mat* X, uint32_t i) :  A{A}, D{D}, X{X}, i{i}
	{
	}
};

#endif   /* ----- #ifndef _THIRD_PHASE_H_  ----- */

