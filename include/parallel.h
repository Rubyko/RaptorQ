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

#ifndef  _PARALLEL_H_
#define  _PARALLEL_H_

#include <future>
#include "encoder.h"
#include "source.h"
#include "packet_common.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_encoder
 *  Description:  This version for non parallel execution is used by parallel execution 
 *  			  function presented below.
 * =====================================================================================
 */
inline static
encoder create_encoder(source_block block) noexcept
{
		// create intermediate symbols.
		intermediate_symbols inter(block);
		// create an encoder using itermediate symbols.
		encoder enc(block, inter);	
		// return the result
		return enc;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_future_encoder
 *  Description:  Creates a future with encoder for a current source block. 
 * =====================================================================================
 */
inline
std::future<encoder> create_future_encoder(source_block block) noexcept
{
	return std::async(std::launch::async, create_encoder, block);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  decode_block
 *  Description:  This function is used by packet_consumer class to recover the data
 *  			  from received packets.
 * =====================================================================================
 */
inline
void decode_block(source_block* block, std::vector<packet> packets, std::vector<int>* ready, u_int8_t SBN) noexcept
{
	(*ready)[SBN] = 2;

	//std::cout << " starting decoding " << (int) SBN << std::endl; 
	// (0) : get the initial data
	auto oti 	   = block->prtt.oti;
	auto prtt      = block->prtt;
	auto KL        = prtt.src.IL;
	auto KS        = prtt.src.IS;
	auto ZL        = prtt.src.JL;
	uint32_t K     = SBN < ZL ? KL : KS;
	auto sys       = block->sys;
	// (1) : if we received less than K' + 4 then skip until we get enough packets to decode
	if(sys.K_prime + 4 > packets.size())
	{
		(*ready)[SBN] = 0;
		return;
	}
	// (2) : sort the packets from the less ESI to the biggest
	std::sort(packets.begin(), packets.end(), 
	[](packet pkt1, packet pkt2){return pkt1.payload.ESI < pkt2.payload.ESI;});
	// (3) : create a map to mark gaps with repair packets
	std::vector<int> map(packets.size(), -1);
	// (4) : prepair the data D to contain the data
	Mat D = Mat(packets.size(), oti.T);
	D.setZero();
	// (5) : number of gaps initially is K'
	auto gaps = sys.K_prime;
	// (6) : insert all source packets into their position
	for(uint32_t i = 0; i < sys.K_prime; i++)
	{
		auto pkt = packets[i];
		auto ESI = pkt.payload.ESI;
		ISI X  = convert_ESI_to_ISI(sys.K_prime, K, ESI); 
		if(X > block->K) 
		{
			break;
		}
		D.row(X) = packets[i].symbol.row(0);
		map[X] = X;
		--gaps;
	}
	// (7) : calculate the gaps being filled
	auto diff = sys.K_prime - gaps;
	packets.erase(packets.begin(), packets.begin() + diff);
	// (8) : now fill the gaps with repair packets
	for(uint32_t i = 0; gaps != 0; i++)
	{
		for(uint32_t j = i; j != sys.K_prime; j++)
		{
			if(map[j] == -1)
			{
				auto pkt = packets[i];
				auto ESI = pkt.payload.ESI;
				ISI X  = convert_ESI_to_ISI(sys.K_prime, K, ESI); 
				D.row(j) = pkt.symbol.row(0);
				map[j] = X;
				gaps--;
				break;
			} 
		}
	}
	packets.erase(packets.begin(), packets.begin() + sys.K_prime - diff);
	// (9) : append to the end all other packets
	for(uint32_t i = 0; i != packets.size(); i++)
	{
		auto pkt = packets[i];
		auto ESI = pkt.payload.ESI;
		ISI X  = convert_ESI_to_ISI(sys.K_prime, K, ESI); 
		D.row(i + sys.K_prime) = pkt.symbol.row(0);
		map[i + sys.K_prime] = X;
	}
	// (10) : calculate intermediate symbols
	intermediate_symbols inter(map, D, *block);
	// (11) : create encoder for this block
	encoder enc(*block, inter);
	// (12) : recover the data
	for(uint32_t i = 0; i < K; i++)
	{
		auto symbol = enc.generate_symbol(i);
		auto target = block->data.row(i); 
		target = symbol.eval();
	//	std::cout << target.cast<char>() << std::endl;
	}
	// (13) : mark the block is recovered
	(*ready)[SBN] = 1;
}

#endif   /* ----- #ifndef _PARALLEL_H_  ----- */


