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

#ifndef  _SYSTEMATIC_H_
#define  _SYSTEMATIC_H_

#include <iostream>
#include <algorithm>
#include <iomanip>
#include "tables/prime_table.h"
#include "tables/systematic_table.h"

struct systematic_indices {

	// K_prime is  the number of source symbols of the extended source block
	uint32_t K_prime;

	// J is the systematic index to use to define the code construction with respect to K'
	uint32_t J;

	// S is the number of LDPC symbols
	uint32_t S;

	// H is the number of HDPC symbols
	uint32_t H;

	// W is the number of LT symbols.
	uint32_t W;

	// L is the number of intermediate symbols, L = K_prime + S + H.
	uint32_t L;

	// P is the number of PI symbols, P = L - W.
	uint32_t P;

	// The number of LT symbols that are not LDPC symbols
	uint32_t B;

	// The number of PI symbols that are not HDPC symbols
	uint32_t U;
	
	// P1 is the smallest prime number that is P1 >= P
	uint32_t P1;

	// find the smallest prime number P1 that is P1 >= P
	uint32_t find_prime_number(uint32_t P){
		return *std::find_if(primes.begin(), primes.end(), [P](auto P1){return P1 >= P;});	
	}

	// user-defined constructor
	systematic_indices(uint32_t K_prime, uint32_t J, uint32_t S, uint32_t H, uint32_t W)
	: K_prime{K_prime}, J{J}, S{S}, H{H}, W{W}, L{K_prime + S + H}, P{L - W}, B{W-S}, U{P-H},
	P1{find_prime_number(P)}
	{
	}

	systematic_indices()
	{
	}

	bool operator==(const systematic_indices& sys)
	{
		return (this->K_prime == sys.K_prime) &&
    		   (this->J       == sys.J) &&
		       (this->S       == sys.S) &&
		       (this->H       == sys.H) &&
		       (this->W       == sys.W) &&
		       (this->L       == sys.L) &&
		       (this->P       == sys.P) &&
		       (this->B       == sys.B) &&
		       (this->U       == sys.U) &&
		       (this->P1      == sys.P1);
	}

	systematic_indices& operator=(const systematic_indices& sys)
	{
		this->K_prime = sys.K_prime;
		this->J       = sys.J;
		this->S       = sys.S;
		this->H       = sys.H;
		this->W       = sys.W;
		this->L       = sys.L;
		this->P       = sys.P;
		this->B       = sys.B;
		this->U       = sys.U;
		this->P1      = sys.P1;
		return *this;
	}

	systematic_indices(const systematic_indices& sys)
	{
		this->K_prime = sys.K_prime;
		this->J       = sys.J;
		this->S       = sys.S;
		this->H       = sys.H;
		this->W       = sys.W;
		this->L       = sys.L;
		this->P       = sys.P;
		this->B       = sys.B;
		this->U       = sys.U;
		this->P1      = sys.P1;
	}

	systematic_indices& operator=(systematic_indices&& sys)
	{
		this->K_prime = sys.K_prime;
		this->J       = sys.J;
		this->S       = sys.S;
		this->H       = sys.H;
		this->W       = sys.W;
		this->L       = sys.L;
		this->P       = sys.P;
		this->B       = sys.B;
		this->U       = sys.U;
		this->P1      = sys.P1;
		return *this;
	}

	// formatted output tutorial: http://www.cprogramming.com/tutorial/iomanip.html
	friend
	std::ostream& operator <<(std::ostream& os, const systematic_indices& sys){

		return os << " K_prime = " << std::setw(4) << sys.K_prime << " THE NUMBER OF SOURCE PLUS PADDING SYMBOLS \n" <<
					 " J       = " << std::setw(4) << sys.J  << " THE SYSTEMATIC INDEX TO USE TO DEFINE THE CODE CONSTRUCTION\n" <<
					 " S       = " << std::setw(4) << sys.S  << " THE NUMBER OF LDPC SYMBOLS\n" <<
					 " H       = " << std::setw(4) << sys.H  << " THE NUMBER OF HDPC SYMBOLS\n" <<
					 " W       = " << std::setw(4) << sys.W  << " THE NUMBER OF LT SYMBOLS\n" <<
					 " L       = " << std::setw(4) << sys.L  << " THE NUMBER OF INTERMEDIATE SYMBOLS\n" <<
					 " P       = " << std::setw(4) << sys.P  << " THE NUMBER OF PI SYMBOLS\n" <<
					 " P1      = " << std::setw(4) << sys.P1 << " THE SMALLEST PRIME NUMBER P1 >= P\n" <<
					 " B       = " << std::setw(4) << sys.B  << " THE NUMBER OF LT SYMBOLS THAT ARE NOT LDPC SYMBOLS\n" <<
					 " U       = " << std::setw(4) << sys.U  << " THE NUMBER OF PI SYMBOLS THAT ARE NOT HDPC SYMBOLS\n" ;
	}
};

/* The first step of encoding is to constructor an extended source block
 * by adding zero or more padding symbols such sthat the total number of
 * symbols, K', is one of the values listed in Section 5.6. Each 
 * padding symbol consists of T octets where the value of each octet is 
 * zero. K' MUST be selected as the smallest value of K' from the table
 * of Section 5.6 that is greater than or equal to K*/

// K - denotes the number os symbols in a single soure block
systematic_indices find_systematic_indices(const uint32_t K) noexcept
{
	auto rez_ptr = std::find_if(Kprime_array.begin(), Kprime_array.end(), [K](uint32_t Kprime) {return K <= Kprime;});
	auto index   = std::distance(Kprime_array.begin(), rez_ptr); 
	auto K_prime = *rez_ptr;
	auto J       = J_array[index];
	auto S       = S_array[index];
	auto H       = H_array[index];
	auto W       = W_array[index];
	return {K_prime, J, S, H, W};
}


#endif   /* ----- #ifndef _SYSTEMATIC_H_  ----- */
