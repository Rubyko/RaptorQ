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


#ifndef  _RANDOM_H_
#define  _RANDOM_H_


/*
 * =====================================================================================
 *        Class:  random_generator
 *  Description:  FROM RFC 6330: 
 *  			  The random number generator Rand[y ,i, m] is defined as follows,
 *  			  where y is a non-negative integer, i is a non-negative integer less
 *  			  than 256, and m is a positive integer, and the value produced is an
 *  			  integer between 0 and m-1. Let V0, V1, V2, and V3 be the arrays 
 *  			  provided in Section 5.5.
 * =====================================================================================
 */
#include "random.h"
#include <cmath>
#include "tables/random_numbers_table.h"
#include <inttypes.h>

struct random_generator
{

	// Clearly it is explained on page 27
	inline
	uint32_t generate(uint32_t y, uint8_t i, uint32_t m){
		constexpr uint32_t two_pow_8  = std::pow(2, 8);  
		constexpr uint32_t two_pow_16 = std::pow(2, 16);
		constexpr uint32_t two_pow_24 = std::pow(2, 24);

		uint32_t x0 = (y + i) % two_pow_8;
		uint32_t x1 = static_cast<uint32_t>(std::floor((double) y/two_pow_8)  + i) % two_pow_8;	
		uint32_t x2 = static_cast<uint32_t>(std::floor((double) y/two_pow_16) + i) % two_pow_8;
		uint32_t x3 = static_cast<uint32_t>(std::floor((double) y/two_pow_24) + i) % two_pow_8;

		return (V0[x0] xor V1[x1] xor V2[x2] xor V3[x3]) % m;
	}


}; /* -----  end of class Rand  ----- */



#endif   /* ----- #ifndef _RANDOM_H_  ----- */
