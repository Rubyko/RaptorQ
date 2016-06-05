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

#ifndef  _INTERMEDIATE_H_
#define  _INTERMEDIATE_H_

// TODO rearrange headers

#include <iostream>
#include <chrono>
#include "global.h"
#include "constraint.h"
#include "systematic.h"
#include "generators/encsym.h"
#include "matrixa.h"
#include "source.h"
#include "decoder/decoder.h"

/*
 * =====================================================================================
 *        Class:  Intermediate_symbols
 *  Description: TODO 
 * =====================================================================================
 */
struct intermediate_symbols
{
		const systematic_indices sys;
		Mat C;

		// TODO add comments  K = blck.data.rows() !!!!!	
		intermediate_symbols (source_block block) : sys{block.sys}
		{  
			extended_source_block ext(block, sys.K_prime);
			//std::cout << std::endl << " extended source block " << std::endl << std::endl;
			//std::cout << ext.data << std::endl;
			// The intermediate symbols can then be calculated as:
			// C = (A^^-1)*D
			//
			// To efficiently generate the intermediate symbols from the source symbols,
			// it is recommended that an efficient decoder implementation such as that
			// described in Section 5.4 be used
			Mat D = Mat(sys.S + sys.H + sys.K_prime, block.data.cols());
			D.setZero();
			D.bottomRows(sys.K_prime) = ext.data;	
			decoder dec(&C, D, sys);
			dec.decode(); // now C have its symbols 
		}; 

		intermediate_symbols (std::vector<int> map, Mat data, source_block block) : sys{block.sys}
		{  
			Mat D = Mat(sys.S + sys.H + data.rows(), data.cols());
			D.setZero();
			D.bottomRows(data.rows()) = data;
			decoder dec(&C, D, sys);
			dec.decode(map);
		}; 


}; /* -----  end of class Intermediate_symbols  ----- */

#endif   /* ----- #ifndef intermediate_INC  ----- */
