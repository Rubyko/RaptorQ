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

#ifndef  _GAUSS_H_
#define  _GAUSS_H_

#include "global.h"
#include <cmath>

struct gauss
{
	Mat* A;
	Mat* D;
	Blck U_lower;
	uint32_t i;

	inline
	bool perform()
	{
		const uint32_t m = U_lower.rows();
		const uint32_t n = U_lower.cols();
		const uint32_t min = std::min(m, n);

		for(uint32_t k = 0; k != min; k++)
		{
			auto col = U_lower.col(k).bottomRows(U_lower.rows() - k).cast<uint32_t>().array();
			uint32_t max_pos;
			for(max_pos = 0; max_pos != m; max_pos++)
			{
				if(col[max_pos] != 0)
				{
					break;
				} 
			}
			if(col[max_pos] == 0)
				return false; // failed

			max_pos = max_pos + k; // + k because we shrinked the column from the above with k
			// swap
			U_lower.row(max_pos).swap(U_lower.row(k));
			D->row(max_pos + this->i).swap(D->row(k + this->i));

			// make this one
			octet alpha = U_lower(k, k);
			U_lower.row(k)/=alpha;
			D->row(k + this->i)/=alpha;

			// destroy bottom
			for(uint32_t i = k + 1; i != m; i++) 
			{
				octet beta  = U_lower(i, k);
				if(beta != 0) 
				{
					// A->row(this->i + i)  += A->row(this->i + k) * beta;
					xor_rows(A->row(this->i + i).data(), (A->row(this->i + k) * beta).eval().data(), A->cols());
					xor_rows(D->row(this->i + i).data(), (D->row(this->i + k) * beta).eval().data(), D->cols());
				}
			}
			// destroy up
			for(uint32_t i = 0; i != k; i++) 
			{
				octet beta  = U_lower(i, k);
				if(beta != 0)
				{
					// A->row(this->i + i)  += A->row(this->i + k) * beta;
					xor_rows(A->row(this->i + i).data(), (A->row(this->i + k) * beta).eval().data(), A->cols());
					xor_rows(D->row(this->i + i).data(), (D->row(this->i + k) * beta).eval().data(), D->cols());
				}
			}
		}
		return true;
	}

	inline
	gauss(Mat* A, Mat* D, uint32_t i, uint32_t u): A{A}, D{D}, U_lower(A->bottomRightCorner(A->rows()-i, u)), i{i}
	{
	}

};


#endif   /* ----- #ifndef _GAUSS_H_  ----- */
