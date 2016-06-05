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

#ifndef  _ENCSYM_H_
#define  _ENCSYM_H_

#include "systematic.h"
#include "tuple.h"
#include "global.h"
#include <stack>
#include <algorithm>

/*
 * =====================================================================================
 *        Class:  encsym
 *  Description:  FROM RFC 6330, p28: 
 *  			  <<! 
 *  			  The encoding symbol generator 
 *  			  Enc[K', (C[0], C[1], ..., C[L-1]), (d, a, b, d1, a1, b1)] 
 *  			  takes the following inputs:
 *
 *  			  ° K' is the number of source symbols for the extended source block. 
 *  			    Let L, W, B, S, P, and P1 be derived from K' as described in
 *  			    Section 5.3.3.3.
 *
 *  			  ° (C[0], C[1], ..., C[L - 1]) is the array of L intermediate symbols
 *  			  	(sub-symbols) generated as described in Section 5.3.3.4.
 * 
 * 				  ° (d, a, b, d1, a1, b1) is a source tuple determined from ISI X
 * 				  	using the Tuple[] generator (located in tuple.h) defined in 
 * 				  	Section 5.3.5.4, whereby: 
 * 				  		* d  is a positive integer denoting an encoding symbol LT degree
 * 				  		* a  is a positive integer between 1 and W-1 inclusive
 * 				  		* b  is a non-negative integer between 0 and W-1 inclusive
 * 				  		* d1 is a positive integer that has velaue either 2 or 3
 * 				  			 denoting an encoding symbol PI degree
 * 				  		* a1 is a positive integer between 1 and P1 - 1 inclusive
 * 				  		* b1 is a non-negative integer between 0 and P1 - 1 inclusive
 * 				  	
 *  			  The encoding symbol generator produces a single encoding symbol as 
 *  			  output (referred to as result)...
 *  			  !>>
 * =====================================================================================
 */
struct encsym_generator
{

		std::vector<int> map;
		// sys indices
		const systematic_indices sys;
		// functors
		tuple_generator tpgn;
		uint32_t overhead = 0;

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  tuple_generator
		 *      Method:  tuple_generator :: indices
		 * Description:  FROM RFC 6330, p26:
		 * 				 <<! 
		 * 				 G_ENC[i, j] = 1 iff C[j] is included in the symbols 
		 * 				 that are summed to produce Enc[K', (C[0], ..., C[L-1]),
		 * 				 (d[i], a[i], b[i], d1[i], a1[i], b1[i])] and G_ENC[i, j] = 0
		 * 				 otherwise.
		 * 				 !>>
		 *				
		 *				 FROM Foundation and Trends (Raptor codes), p??
		 *				 <<!
		 *				 TODO
		 *				 !>>
		 *
		 * 				 FROM PROGRAMMER, THE RIGHT PART OF THE BRAIN:
		 * 				 <<!
		 * 				 So lets calculate these indices of those symbols. To do that we need
		 * 				 the algorithm described on p28 and p29 of the RFC 6330. Also we can read the same 
		 * 				 explanation on the p291-p292 in Foundation and Trends(Raptor codes).
		 *
		 * 				 As you see the input is ISI X. The range os the X is [0...K'...K' + repair symbols].
		 * 				 The output is indices of symbols that are summed to produce the result, but in this case
		 * 				 we don't need the actual result. We need the indices of the symbols to construct the matrix A
		 * 				 and to calculate them at once later. You understand ? I hope you do.
		 * 				 !>>
		 *--------------------------------------------------------------------------------------
		 */
		inline
		std::vector<uint32_t> indices(ISI X)
		{
			if(overhead > 0)
				X = map[X];

			 // initialize the tuple generator
			 tuple tpl = tpgn(X);
			 // initialize the vector to store the indices
			 // we are reserving the space for the vector since 
			 // there's one for loop with d-1 and another for loop with d1-1 
			 // and two additional insertion 
			 std::vector<uint32_t> vec;
			 vec.reserve(tpl.d + tpl.d1);			 
			 // result = C[b]
			 vec.push_back(tpl.b);
			 // the first for loop with d - 1 cycles	
			 for(uint32_t j = 1; j != tpl.d; j++)
			 {
				 tpl.b = (tpl.b + tpl.a) % sys.W;
				 // result = result + C[b]
			     vec.push_back(tpl.b);
			 }
			 while(tpl.b1 >= sys.P) 
			 {
				 tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
			 }
			 // result = result + C[W + b1]
			 vec.push_back(sys.W + tpl.b1);
			 // the second for loop with d1 cycles
			 for(uint32_t j = 1; j != tpl.d1; j++) 
			 {
				 tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
				 while(tpl.b1 >= sys.P)
				 {
					 tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
				 }
				 // result = result + C[W+b1]
				 vec.push_back(sys.W + tpl.b1);
			}
			return vec;
		}
		
		/* ====================  LIFECYCLE     ======================================= */
		inline
		encsym_generator (const systematic_indices& sys, Mat* const A, uint32_t overhead) : sys{sys}, tpgn{sys}, overhead{overhead}
		{
			 auto sum   = sys.K_prime + overhead;
			 auto G_ENC = A->bottomRows(sum);
			 for (ISI X = 0; X != sum; X++)
			 {
				for(auto j : this->indices(X))
					G_ENC(X, j) = 1;
			 }
		}  


		inline
		encsym_generator (std::vector<int> map, const systematic_indices& sys, Mat* const A, uint32_t overhead) 
		: map{map}, sys{sys}, tpgn{sys}, overhead{overhead}
		{
			 auto sum   = sys.K_prime + overhead;
			 auto G_ENC = A->bottomRows(sum);
			 for (ISI X = 0; X != sum; X++)
			 {
				for(auto j : this->indices(X))
					G_ENC(X, j) = 1;
			 }
		}  

//
	

}; /* -----  end of class encsym  ----- */



#endif   /* ----- #ifndef _ENCSYM_H_  ----- */
