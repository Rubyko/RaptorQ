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


#ifndef  _TUPLE_H_
#define  _TUPLE_H_

#include "systematic.h"
#include <inttypes.h>
#include <cmath>
#include "random.h"
#include "global.h"
#include "generators/degree.h"
#include <iostream>

/*
 * =====================================================================================
 *        Class:  tuple
 *  Description:  The output of the tuple generator is a tuple, (d, a, b, d1, a1, b1)
 * =====================================================================================
 */
struct tuple {
	
	uint32_t d;
	uint32_t a; 
	uint32_t b; 
	uint32_t d1;
	uint32_t a1;
	uint32_t b1;

	tuple(uint32_t d, uint32_t a, uint32_t b, uint32_t d1, uint32_t a1, uint32_t b1) :
		d{d}, a{a}, b{b}, d1{d1}, a1{a1}, b1{b1}
	{
	}

	friend
	std::ostream& operator<<(std::ostream& os, const tuple& tpl)
	{
		return os << "\n TUPLE WITH VALUES : \n" << " d  = " << tpl.d  << " a  = " << tpl.a  << " b  = " << tpl.b  << '\n'
												 << " d1 = " << tpl.d1 << " a1 = " << tpl.a1 << " b1 = " << tpl.b1 << '\n';
	}
};

/*
 * =====================================================================================
 *        Class: tuple_generator
 *  Description: FROM: RFC 6330.
 *
 *  			 The tuple generator Tuple[K', X] takes the following inputs:
 *  			 K': the number os source symbols in the extended source block
 *  			 X : an ISI
 *
 *  			 Let
 *  			 L be determined from K' as described in Section 5.3.3.3
 *  			 J = J(K') be the systematic index associated with K', as defined
 *  			 in Table 2 in Section 5.6
 * =====================================================================================
 */
struct tuple_generator
{

		systematic_indices sys;
		degree_generator dg;
		random_generator rnd;

		inline
		tuple_generator (const systematic_indices sys) : sys{sys}, dg{sys}, rnd{}
		{
		}

		tuple_generator()
		{
		}

		tuple_generator& operator=(const tuple_generator& tpl)
		{
			this->sys = tpl.sys;
			this->dg = tpl.dg;
			this->rnd = tpl.rnd;
			return *this;
		}

		tuple_generator& operator=(tuple_generator&& tpl)
		{
			this->sys = tpl.sys;
			this->dg = tpl.dg;
			this->rnd = tpl.rnd;
			return *this;
		}
		
		inline
		tuple operator()(ISI X)
		{
			constexpr uint64_t two_pow_32 = std::pow(2, 32);	
			constexpr uint32_t two_pow_20 = std::pow(2, 20); 

			auto A = 53591 + sys.J * 997;
			if((A % 2) == 0) A = A + 1;
			uint32_t B = 10267 * (sys.J + 1);
			const uint32_t y = (B + X*A) % two_pow_32;
			auto v = rnd.generate(y, 0, two_pow_20);
			uint32_t d = dg(v);
			auto a = 1 + rnd.generate(y, 1, sys.W - 1);
			auto b = rnd.generate(y, 2, sys.W);
			auto d1 = (d < 4) ? 2 + rnd.generate(X, 3, 2) : 2;
			auto a1 = 1 + rnd.generate(X, 4, sys.P1 - 1);
			auto b1 = rnd.generate(X, 5, sys.P1);
			tuple rez(d, a, b, d1, a1, b1);
			return rez;
		}


		
}; /* -----  end of class Tuple  ----- */



#endif   /* ----- #ifndef _TUPLE_H_  ----- */


