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

#ifndef  _SOURCE_H_
#define  _SOURCE_H_

#include "partition.h"
#include "fec.h"
#include "global.h"
#include <algorithm>
#include "systematic.h"

class sub_source_block;
class source_block;
class source_object;
struct prtt_parm;
struct partition;

/*
 * =====================================================================================
 *        Class:  sub_source_block
 *  Description:  
 * =====================================================================================
 */
struct sub_source_block
{
		Mat data;
		// K -  the number of sub-symbols (THE SAME FOR ALL SUB BLOCKS OF THE SOURCE BLOCK)
		// T_prime - the number of octets in one sub_symbol (not the same for each sub-block of a source block)
		inline
		sub_source_block (octet* beg, uint32_t K, uint32_t T_prime) : data{Eigen::Map<Mat>(beg, K, T_prime)}
		{
		}                        

}; /* -----  end of class sub_source_block  ----- */


/*
 * =====================================================================================
 *        Class:  source_block
 *  Description:  
 * =====================================================================================
 */
struct source_block
{

	// The source block is identified by a unique Source Block Number (SBN),
	// where the first source block has SBN zero;
	uint8_t SBN;
	Mat data;
	uint32_t K;
	const systematic_indices sys;
	partition prtt;

	// SBN - source block number
	// K   - number of source symbols (may be not the same K for each source block)
	// T   - number of octets in one source symbol (THE SAME FOR ALL SOURCE BLOCKS)
	// N   - number of sub-blocks			sub_source_block sbblck(beg, K, TL*Al);
	inline
	source_block (octet* beg, uint8_t SBN, uint32_t K, uint32_t T, partition prtt) 
		: SBN{SBN}, data{Eigen::Map<Mat>(beg, K, T)}, K{K}, sys{find_systematic_indices(K)}, prtt{prtt}
	{
		auto TL = prtt.sub.IL;
		auto TS = prtt.sub.IS;
		auto NL = prtt.sub.JL;
		auto NS = prtt.sub.JS;
		auto Al = prtt.Al;

		for(uint32_t N = 0; N!=NL; N++)
		{
			sub_source_block sbblck(beg, K, TL*Al);
			sub_source_blocks.push_back(sbblck);
			beg = beg + K*TL*Al;
		}
		for(uint32_t N = 0; N!=NS; N++)
		{
			sub_source_block sbblck(beg, K, TS*Al);
			sub_source_blocks.push_back(sbblck);
			beg = beg + K*TS*Al;
		}

	}

	// Each source block with K source symbols is divided into N >= 1 subblocks,
	// which are small enough to be decoded in the working memory.
	std::vector<sub_source_block> sub_source_blocks;
}; /* -----  end of class source_block  ----- */



template< typename T >
struct array_deleter
{
  void operator ()( T const * p)
  { 
    delete[] p; 
  }
};


/*
 * =====================================================================================
 *        Class:  source_object
 *  Description:  
 * =====================================================================================
 */
struct source_object
{
	fec_oti oti;
	partition prtt;
	// In order to apply the RaptorQ encoder to a source object, the object may be 
	// broken into Z>= 1 blocks, known as source blocks.
	std::vector<source_block> source_blocks;

	inline
	source_object()
	{
	}

	inline
	source_object(octet* beg, octet* end, partition prtt) : oti{prtt.oti}, prtt{prtt}
	{
		auto KL = prtt.src.IL;
		auto KS = prtt.src.IS;
		auto ZL = prtt.src.JL;
		auto ZS = prtt.src.JS;
		auto T  = prtt.T;

		auto shared_beg = std::shared_ptr<octet>(new octet[prtt.Kt*prtt.T](), array_deleter<octet>());
		std::copy(beg, end, shared_beg.get());
		beg = shared_beg.get();

		auto SBN = 0;
		for(uint32_t Z = 0; Z!=ZL; Z++)		// first ZL
		{
			source_block blck(beg, SBN++, KL, T, prtt);
			source_blocks.push_back(blck);
			beg = beg + KL*T;
		}
		for(uint32_t Z = 0; Z!=ZS; Z++)		// last ZS
		{
			source_block blck(beg, SBN++, KS, T, prtt);
			source_blocks.push_back(blck);
			beg = beg + KS*T;
		}
	}


	inline
	source_object(partition prtt) : oti{prtt.oti} , prtt{prtt}
	{
		auto KL = prtt.src.IL;
		auto KS = prtt.src.IS;
		auto ZL = prtt.src.JL;
		auto ZS = prtt.src.JS;
		auto T  = prtt.T;

		auto shared_beg = std::shared_ptr<octet>(new octet[prtt.Kt*prtt.T](), array_deleter<octet>());
		auto beg = shared_beg.get();

		auto SBN = 0;
		for(uint32_t Z = 0; Z!=ZL; Z++)		// first ZL
		{
			source_block blck(beg, SBN++, KL, T, prtt);
			source_blocks.push_back(blck);
			beg = beg + KL*T;
		}
		for(uint32_t Z = 0; Z!=ZS; Z++)		// last ZS
		{
			source_block blck(beg, SBN++, KS, T, prtt);
			source_blocks.push_back(blck);
			beg = beg + KS*T;
		}
	}

	inline
	void get_data(int F, octet* beg)
	{
		auto KL = prtt.src.IL;
		auto KS = prtt.src.IS;
		auto ZL = prtt.src.JL;
		auto ZS = prtt.src.JS;
		auto T  = prtt.T;

		octet loc[prtt.Kt * prtt.T];
		octet* loc_beg = loc;
		
		auto SBN = 0;
		for(uint32_t Z = 0; Z!=ZL; Z++)		// first ZL
		{
			auto mat = source_blocks[SBN++].data;
			Eigen::Map<Mat>(loc_beg, mat.rows(), mat.cols()) = mat;
			loc_beg = loc_beg + KL*T;
		}
		for(uint32_t Z = 0; Z!=ZS; Z++)		// last ZS
		{
			auto mat = source_blocks[SBN++].data;
			Eigen::Map<Mat>(loc_beg, mat.rows(), mat.cols()) = mat;
			loc_beg = loc_beg + KS*T;
		}
		std::copy(loc, loc + F, beg);
	}


}; /* -----  end of class Source_object  ----- */

struct extended_source_block
{
	Mat data;

	inline
	extended_source_block(source_block blck, uint32_t K_prime)
	{
		data = Mat(K_prime, blck.data.cols());
		data.setZero();
		data.topRows(blck.data.rows()) = blck.data;
	}
};


#endif   /* ----- #ifndef _SOURCE_H_  ----- */
