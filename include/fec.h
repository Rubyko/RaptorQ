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

#ifndef  _FEC_H_
#define  _FEC_H_

#include <cstdint>

//https://msdn.microsoft.com/en-us/library/ewwyfdbe.aspx

// The encoded FEC Object Transmission Informatioin is a 12-octet field
// consisting of the concatenation of the encoded Commom FEC Object Transmission
// Information and the encoded Scheme-specific FEC Object Transmission Information.
struct fec_oti {
	// The common FEC Object Transmission Information:
	// Transfer Length (F) : 40 bit unsigned integer.
	// This is the transfer length of the object in units of octets.
	uint64_t F : 40;
	// reserved
	unsigned   : 8;
	// Symbol Size (T) : 16 bit unsigned integer.
	// This is the size of a symbol in units of octets.
	uint16_t T : 16;


	// The following parameters are carried in he Scheme-Specific FEC 
	// Object Transmission Information element for this FEC scheme:
	// the number of sub-blocks (N): 16-bits unsigned integer.
	uint16_t N : 16;
	// the number of source blocks (Z): 8-bit unsigned integer.
	uint8_t Z : 8;
	// a symbol alingment parameter (Al): 8-bit unsigned integer. 
	uint8_t Al : 8;

}  __attribute__ ((__packed__));

struct fec_payload
{ 
	uint8_t  SBN : 8;
	uint32_t ESI : 24; 
}  __attribute__ ((__packed__));

#endif   /* ----- #ifndef _FEC_H_  ----- */
