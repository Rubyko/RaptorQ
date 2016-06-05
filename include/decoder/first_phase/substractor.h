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

#ifndef  _SUBSTRACTOR_H_
#define  _SUBSTRACTOR_H_

#include <vector>
#include "row.h"
#include "alingment.h"

struct substractor {

	Mat* A;
	Mat* D;
	std::vector<row> rows;

	substractor(Mat* A, Mat* D) : A{A}, D{D}
	{
	}

	void add(row rw)
	{
		rows.push_back(rw);
	}

	void process()
	{
		auto i_param = 0;
		while(rows.size())
		{
			auto rloc = rows.begin()->rvec;
			for(int i = 1; i < A->rows() - i_param; i++)
			{
				octet beta  = (*A)(i + i_param, i_param);
				if(beta == 0)	
					continue;
				else if(beta == 1)
				{
				//	A->row(i + i_param) += A->row(i_param);
					xor_rows(A->row(i + i_param).data(), A->row(i_param).data(), A->cols());
					xor_rows(D->row(i + i_param).data(), D->row(i_param).data(), D->cols());
				}
				else
				{	
					octet alpha = (*A)(i_param, i_param);
				//	A->row(i + i_param) += (A->row(i_param)*(beta/alpha));
					xor_rows(A->row(i + i_param).data(),(A->row(i_param)*(beta/alpha)).eval().data(), A->cols());
					xor_rows(D->row(i + i_param).data(),(D->row(i_param)*(beta/alpha)).eval().data(), D->cols());
				}
			}
			rows.erase(rows.cbegin());
			i_param++;
		}
	}

};

#endif   /* ----- #ifndef _SUBSTRACTOR_H_  ----- */
