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


#ifndef  encoder_INC
#define  encoder_INC

#include "systematic.h"
#include "intermediate.h"
#include "global.h"
#include "generators/tuple.h"
#include "source.h"

struct encoder
{
	
	uint32_t SBN;
	systematic_indices sys;
	Mat C;
	tuple_generator tpgn;

	encoder(const source_block& block, intermediate_symbols inter) :
		SBN{block.SBN}, sys{block.sys}, C{inter.C}, tpgn{block.sys}
	{
	}

	// move assignment
	encoder& operator=(encoder&& enc)
	{
		this->SBN = enc.SBN;
		this->sys = std::move(enc.sys);
		this->C   = std::move(enc.C);
		this->tpgn = std::move(enc.tpgn);
		return *this;
	}

	encoder(const encoder& enc) : 	
		SBN{enc.SBN}, sys{enc.sys}, C{enc.C}, tpgn{enc.sys}
	{
	}

	encoder& operator=(const encoder& enc)
	{
		this->SBN = enc.SBN;
		this->sys = enc.sys;
		this->C   = enc.C;
		this->tpgn = enc.tpgn;
		return *this;
	}

	encoder()
	{
	}

	Mat generate_symbol(ISI X)
	{
		tuple tpl = tpgn(X);
		auto result = C.row(tpl.b).eval();
		for(uint32_t j = 1; j != tpl.d; j++)
		{
			tpl.b = (tpl.b + tpl.a) % sys.W;
			result += C.row(tpl.b);
		}
		while(tpl.b1 >= sys.P)
		{
			tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
		}
		result += C.row(sys.W + tpl.b1);
		for(uint32_t j = 1; j != tpl.d1; j++)
		{
			tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
			while(tpl.b1 >= sys.P) 
				tpl.b1 = (tpl.b1 + tpl.a1) % sys.P1;
			result += C.row(sys.W + tpl.b1);
		}
		return result;
	}

};




#endif   /* ----- #ifndef encoder_INC  ----- */
