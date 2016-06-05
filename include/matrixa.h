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


#ifndef  _MATRIXA_H_
#define  _MATRIXA_H_

#include "constraint.h"
#include "systematic.h"
#include "generators/encsym.h"
#include "global.h"
#include "debug.h"
#include <inttypes.h>

static class Cache 
{
		Mat A;
		uint32_t overhead;
		systematic_indices sys;

	public:
		inline
		bool is_cache_valid(const systematic_indices& sys, const uint32_t& overhead)
		{
			return (this->sys == sys) && (this->overhead == overhead);
		}
	
		inline
		void update(Mat A, systematic_indices sys, uint32_t overhead)
		{
			this->A = A;
			this->overhead = overhead;
			this->sys = sys;
		}

		inline
		Mat get_A()
		{
			return this->A;
		}

} cache;

struct matrixa {

	// L = P + W or L = K' + S + H
	inline
	static void construct_matrix_A (Mat* const A, const systematic_indices sys, const uint32_t overhead) noexcept 
	{
		if (cache.is_cache_valid(sys, overhead))
		{
			*A = cache.get_A();
			return;
		}

		*A = {sys.S + sys.H + sys.K_prime + overhead, sys.W + sys.P};
		A->setZero();
		// Matrix A is passed by address and it's modified.
		constraint(sys, A);
		// generate G_ENC. G_ENC establishes the LT-PI relationship 
		// between the encoded symbols C'[0],...,C'[K'-1], C'[K'], ...
		// and the intermediate symbols C[0],...,C[L-1].
		encsym_generator(sys, A, overhead);
		// cache
		cache.update(*A, sys, overhead);
	}  

	// L = P + W or L = K' + S + H
	inline
	static void construct_matrix_A (std::vector<int> map, Mat* const A, const systematic_indices& sys, const uint32_t overhead) noexcept 
	{
		if (cache.is_cache_valid(sys, overhead))
		{
			*A = cache.get_A();
			A->block(sys.S + sys.H, 0, sys.K_prime + overhead, sys.W + sys.P).setZero();
		} 
		else
		{
			*A = {sys.S + sys.H + sys.K_prime + overhead, sys.W + sys.P};
			A->setZero();
			// Matrix A is passed by address and it's modified.
			constraint(sys, A);
		}
		// generate G_ENC. G_ENC establishes the LT-PI relationship 
		// between the encoded symbols C'[0],...,C'[K'-1], C'[K'], ...
		// and the intermediate symbols C[0],...,C[L-1].
		encsym_generator(map, sys, A, overhead);

		// cache
		cache.update(*A, sys, overhead);
	}  
                              
};
#endif   /* ----- #ifndef _MATRIXA_H_  ----- */
