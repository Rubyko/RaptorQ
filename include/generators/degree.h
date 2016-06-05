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


#ifndef  _DEGREE_H_
#define  _DEGREE_H_

#include <inttypes.h>
#include "systematic.h"
#include "global.h"
#include "tables/degree_table.h"
#include <algorithm>

/*
 * =====================================================================================
 *        Class:  degree_generator
 *  Description:  FROM: RFC 6330
 *  
 *  			  The degree generator Deg[v] is defined as follows, where v is a 
 *  			  non-negative integer that is less than 2^^20 = 1048576. Given v, find
 *  			  index d in Table 1 such that f[d-1] <= v < f[d], and set Deg[v] =
 *  			  min(d, W-2). Recall that W is derived from K' as described in 
 *  			  Section 5.3.3.3
 * =====================================================================================
 */
struct degree_generator
{
		systematic_indices sys;

		degree_generator()
		{
		}

		inline
		degree_generator (const systematic_indices sys) : sys{sys}
		{
		}    

		degree_generator& operator=(const degree_generator& dgr)
		{
			this->sys = dgr.sys;
			return *this;
		}
	
		inline
		uint32_t operator()(uint32_t v)
		{
			auto ptr = std::upper_bound(degree_table.begin(), degree_table.end(), v);
			auto d = std::distance(degree_table.begin(), ptr);
			auto res = std::min<uint32_t>(d, sys.W - 2);
			return res;
		}

}; /* -----  end of class Deg  ----- */



#endif   /* ----- #ifndef _DEGREE_H_  ----- */
