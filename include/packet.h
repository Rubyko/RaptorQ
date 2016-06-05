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

#ifndef  _PACKET_H_
#define  _PACKET_H_

#include "fec.h"
#include "encoder.h"
#include "systematic.h"
#include "global.h"
#include <vector>
#include "source.h"
#include "intermediate.h"
#include <unordered_map>
#include <iterator>
#include <future>
#include "parallel.h"
#include "packet_common.h"


struct packet_producer
{
	std::vector<std::future<encoder>> future_encs;
	std::unordered_map<uint32_t, encoder> enc_map;

	std::vector<source_block> blocks;
	const fec_oti oti;
	std::vector<uint32_t> counter;

	packet_producer(std::vector<std::future<encoder>> future_encs, 
			const std::vector<source_block> blocks, const fec_oti oti)
		: future_encs{std::move(future_encs)}, blocks{blocks}, oti{oti}, counter(blocks.size(), 0)
	{
	}

	packet produce(uint32_t SBN)
	{
		ESI esi = counter[SBN]++;
		ISI X   = convert_ESI_to_ISI(blocks[SBN].sys.K_prime,  blocks[SBN].K, esi);
		if(enc_map.count(SBN) == 0)
		{
			enc_map[SBN] = future_encs[SBN].get();
		}
		Mat sym = enc_map[SBN].generate_symbol(X);
		fec_payload payload;
		payload.ESI = esi;
		payload.SBN = SBN;
		return packet(payload, oti, sym);
	}
};


struct packet_consumer
{
	bool partitioned = false;
	std::unordered_map<uint32_t, std::vector<packet>> packets_per_block;
	std::vector<int> ready; // 0 - not ready, 1 - ready, 2 - processing
	fec_oti oti;
	uint64_t WS;
	uint32_t P_prime;
	uint8_t SS;
	uint8_t Al;
	partition prtt;
	source_object obj;

	packet_consumer(uint64_t WS, uint32_t P_prime, uint8_t SS, uint8_t Al) :
		WS{WS}, P_prime{P_prime}, SS{SS}, Al{Al}
	{
	}

	void consume(packet pck)
	{
		if(!partitioned)
		{
			oti = pck.oti;
			prtt = partition(oti.F, WS, P_prime, Al, SS);
			obj = source_object(prtt);
			ready = std::vector<int>(oti.Z, 0);
			partitioned = true;
		}
		auto SBN = pck.payload.SBN;
		if((ready[SBN] == 2) || (ready[SBN] == 0))
		{
			packets_per_block[SBN].push_back(pck);
			if(ready[SBN] == 0){
				ready[SBN] = 2;
				std::future<void> future_rez = 
				std::async(std::launch::async, 
				decode_block, 
				&obj.source_blocks[SBN], 
				packets_per_block[SBN], 
				&ready, uint8_t{SBN});
			}
		}
	}

	inline
	bool is_data_ready()
	{
		for(int var : ready)
		{
			if(var != 1)
				return false;
		}
		return true;
	}

	template<typename I>
	inline void get_data(I pbegin, I pend)
	{
		octet* beg = reinterpret_cast<octet*>(&(*pbegin));
		octet* end = reinterpret_cast<octet*>(&(*pend));
		uint64_t F = end - beg;
		if(F < prtt.F)	
		{
			obj.get_data(F, beg);
		}
		else 
		{
			obj.get_data(prtt.F, beg);
		}
	}
};


#endif   /* ----- #ifndef _PACKET_H_  ----- */
