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

#ifndef  _FOURTH_PHASE_H_
#define  _FOURTH_PHASE_H_

#include "global.h"
#include <inttypes.h>

struct fourth_phase {

	Mat* A;
	Mat* D;
	Blck U_upper;
	Mat I_u;
	uint32_t i;
	uint32_t u;
	
	inline
	bool operator()() noexcept
	{
		for (int i = 0; i != U_upper.rows(); i++)
		{
			for(int j = 0; j != U_upper.cols(); j++)
			{
				if(U_upper(i, j) != 0)
				{
					octet b = U_upper(i, j);
					//	A->row(i) += A->row(U_upper.rows() + j) * b;
					xor_rows(A->row(i).data(), (A->row(U_upper.rows() + j) * b).eval().data(), A->cols());
					xor_rows(D->row(i).data(), (D->row(U_upper.rows() + j) * b).eval().data(), D->cols());
				}
			}
		}
		return true;
	}

	inline
	fourth_phase(Mat* A, Mat* D, uint32_t i, uint32_t u) : 
		A{A}, D{D}, U_upper(A->topRightCorner(i , u)), I_u(A->bottomRightCorner(u, u)), i{i}, u{u}
	{
	}

};

#endif   /* ----- #ifndef _FOURTH_PHASE_H_  ----- */
