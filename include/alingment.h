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

#ifndef  _ALINGMENT_H_
#define  _ALINGMENT_H_

#include "global.h"

using alingment = uint_fast64_t;
constexpr size_t Al = sizeof(alingment);

// row1 - target
// row2 - source
inline 
void xor_rows(octet* row1, const octet* row2, size_t size)
{
	
	alingment* bitset1;
	const alingment* bitset2;

	uint32_t offset = 0;
	bitset1 = reinterpret_cast<alingment*>(row1);
	bitset2 = reinterpret_cast<const alingment*>(row2);
	while(offset <= size - Al || (offset == 0 && (size == Al)))
	{
		*bitset1 ^= *bitset2;
		++bitset1;
		++bitset2;
		offset+=Al;
	}

	if((size  - offset) != 0)
	{  
		for (uint32_t i = 0; i != (size - offset); i++)
		{
			row1[size - i - 1] += row2[size - i - 1];
		}
	}
}


#endif   /* ----- #ifndef _ALINGMENT_H_  ----- */
