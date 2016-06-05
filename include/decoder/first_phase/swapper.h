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

#ifndef  _SWAPPER_H_
#define  _SWAPPER_H_

#include "global.h"
#include "systematic.h"

struct improved_swapper 
{
	Mat*  A;
	Mat*  D;
	Perm* Q;
	const systematic_indices sys;

	inline
	improved_swapper(Mat* A, Mat* D, Perm* Q,  const systematic_indices& sys) : A{A}, D{D}, Q{Q}, sys{sys}
	{
	}

	inline
	void reoder_columns(row choosen, uint32_t i_param, int32_t u_param)
	{
  		if(i_param + u_param != sys.L - 1)
		{
			// place the first r in the first column
			auto vec = choosen.rvec;
			auto size = vec.size();
			for (uint32_t i = 0; i < size; i++)
			{
				auto pos = vec[i] + i_param;
				if(i == 0)
				{
					A->col(i_param).swap(A->col(pos));
					Q->applyTranspositionOnTheRight(i_param, pos);
				}
				else 
				{
					auto tmp = A->cols() - u_param + (i - size);
					A->col(tmp).swap(A->col(pos));
					Q->applyTranspositionOnTheRight(tmp, pos);
				}
			}
		}
	}

	inline
	void swap_rows(row choosen, uint32_t i_param, int32_t u_param)
	{
		// swap rows
		auto target_row_indx = 0;
		auto choosen_row_indx = choosen.index; 
		A->row(target_row_indx + i_param).swap(A->row(choosen_row_indx + i_param));
		D->row(target_row_indx + i_param).swap(D->row(choosen_row_indx + i_param));
	}

	inline
	void add(Blck* V, row chosen, int i, int u)
	{
		swap_rows(chosen, i, u);
		reoder_columns(chosen, i, u);
	}

};

#endif   /* ----- #ifndef _SWAPPER_H_  ----- */
