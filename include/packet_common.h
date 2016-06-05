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

#ifndef  _PACKET_COMMON_H_
#define  _PACKET_COMMON_H_


struct packet
{
	fec_payload payload;
	fec_oti     oti;
	Mat 		symbol;

	packet(fec_payload payload, fec_oti oti, Mat symbol)
		: payload{payload}, oti{oti}, symbol{symbol}
	{
	}
};

inline 
ISI convert_ESI_to_ISI(uint32_t K_prime, uint32_t K, ESI X)
{
	if(X < K)
	{
		return X;
	} 
	else 
	{
		return X + (K_prime - K);
	}
}


#endif   /* ----- #ifndef _PACKET_COMMON_H_  ----- */
