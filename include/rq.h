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

#ifndef  _RQ_H_
#define  _RQ_H_

#include <future>
#include "source.h"
#include "partition.h"
#include "octet.h"
#include "intermediate.h"
#include "encoder.h"
#include "packet.h"
#include "parallel.h"
#include "alingment.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_encoding_packet_producer
 *
 *  Description:  I       - iterator type;
 *
 *  			  pbegin  - the iterator to the first element of the data (required);
 *
 *  			  pend    - the iterator to the last element of the data (required);
 *
 *  			  WS      - the maximum size block that is decodable in working memory,
 *  			  			in octets (required);
 *
 *  			  P_prime - the maximum payload size in octets, which is assumed to be
 *  			  			a multiple of Al (required);
 *
 *  			  This function returns an object of type packet_producer. Use this 
 *  			  object to produce the packets to be sent to the appropriate receivers.
 * =====================================================================================
 */
template<class I>
inline packet_producer
create_encoding_packet_producer(I pbegin, I pend, const uint64_t WS, const uint32_t P_prime)
{
	const uint8_t SS = 1;
	// (1): cast provided iterators to pointers to octets.
	octet* beg = reinterpret_cast<octet*>(&(*pbegin));
	octet* end = reinterpret_cast<octet*>(&(*pend));
	// (2): calculate the length of the object to transfer.
	const uint64_t F = end - beg;
	// (3): partition this objects.
	partition prtt(F, WS, P_prime, Al, SS);
	// (4): create the source object and pass partition results and pointer to the data.
	source_object obj(beg, end, prtt);
	// (5): prepair a container for futures with encoder for each source block.s
	std::vector<std::future<encoder>> future_encoders;
	// (6): get all blocks to begin iteration.
	auto blcks = obj.source_blocks;
	// (7): iterate over the all blocks. 
	for(auto ptr = blcks.begin(); ptr!=blcks.end(); ptr++)
	{	
		// (8): create futures for encoders
		std::future<encoder> future_enc = create_future_encoder(*ptr); 
		// (9): save them into container
		future_encoders.push_back(std::move(future_enc));
	}
	// return a packet producer with futures for encoders, related blocks and other parameters.
	return packet_producer(std::move(future_encoders), blcks, prtt.oti);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_encoding_packet_consumer
 *
 *  Description:  WS      - the maximum size block that is decodable in working memory,
 *  			  			in octets (required);
 *
 *  			  P_prime - the maximum payload size in octets, which is assumed to be
 *  			  			a multiple of Al (required);
 *
 *				  This function returns an object of packet_consumer type. Use this object
 *				  to receive the packets, to check wheather the data is ready or not and to get
 *				  the recovered data. 
 * =====================================================================================
 */
inline packet_consumer
create_encoding_packet_consumer (uint64_t WS, uint32_t P_prime)
{
	const uint8_t SS = 1;
	// (1) : There's nothing what can we do now, so let's wait for the first packet arrive,
	// 	 	 after we can reconstruct the structure of the source object.
	return packet_consumer(WS, P_prime, SS, Al);
}

#endif   /* ----- #ifndef _RQ_H_  ----- */
