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

#ifndef  _PARTITION_H_
#define  _PARTITION_H_ 

#include <iostream>
#include <vector>
#include "fec.h"
#include <string>
#include <memory>
#include <Eigen/Dense>
#include "tables/systematic_table.h"
#include <string>
#include "global.h"


constexpr int K_prime_max = 56403;

struct prtt_parm {
	uint32_t IL;
	uint32_t IS;
	uint32_t JL;
	uint32_t JS;
};

// the function (in this case functor) partition[i,j]
class prtt_func {
	public:

		prtt_func(fec_oti oti) : oti{oti} 
		{
			assert(std::ceil(std::ceil(oti.F / oti.T) / oti.Z) < K_prime_max 
			&& " the parameters must be set so that ceil(ceil(f/t)/z) <= k'max");
		}	

		auto operator ()(uint32_t I, uint32_t J)
		{
			uint32_t IL = std::ceil  ((double) I / J);
			uint32_t IS = std::floor ((double) I / J);

			uint32_t JL = I - IS * J;
			uint32_t JS = J - JL;

			return prtt_parm{IL, IS, JL, JS};
		}
	private:
		// fec parameters
		fec_oti oti;
};


struct partition
{

	uint64_t F;
	uint64_t WS;
	uint8_t T;
	uint8_t Al;
	uint8_t SS;

	int Kt;
	prtt_parm src, sub;
	fec_oti oti;

	uint32_t KL(uint32_t n){
		auto val = WS / (Al * std::ceil((double)T/(Al*n)));
		for(int i = 1; i != Kprime_array.size(); i++)
		{
			if ((Kprime_array[i-1] <= val) && (val < Kprime_array[i]))
				return Kprime_array[i-1];
		}
		//assert(0!=0&&"we've got problem in struct partition");
		return -1;
	}

	partition(){}

	partition(uint64_t F, uint64_t WS, uint8_t P_prime, uint8_t Al, uint8_t SS) :
		F{F}, WS{WS}, T{P_prime}, Al{Al}, SS{SS}
	{
		Kt = std::ceil((double) F/ T);
		auto N_max = std::floor((double)T / (SS*Al));
		auto Z = std::ceil((double) (Kt/N_max));
		for(int n = 1; n <= N_max; n++)
		{
			if(std::ceil((double) Kt/Z) <= KL(n))
			{
				oti.F  = F;
				oti.Al = Al;
				oti.T  = T;
				oti.Z  = Z;
				oti.N  = n;

				src = prtt_func(oti)(Kt, oti.Z);
				sub = prtt_func(oti)(T/Al, oti.N);
				//print_debug();
				break;
			}
		}
	}

	void print(int B, int S, int O)
	{
		for(int i = 0; i!= B; i++)
		{
			std::cout << "b{";
			for(int j = 0; j!=S; j++)
			{
				std::cout << "s<";
				for(int k = 0; k!=O; k++)
				{
					std::cout << "o";
				}
				std::cout << ">s";
			}
			std::cout << "}b ";
		}
	}


	void print_debug()
	{
		auto KL = src.IL;
		auto KS = src.IS;
		auto ZL = src.JL;
		auto ZS = src.JS;
		std::cout << " ZL:  ";
		print(ZL, KL, T);
		std::cout << " ZS:  ";
		print(ZS, KS, T);
		std::cout << std::endl;
		
		auto TL = sub.IL;
		auto TS = sub.IS;
		auto NL = sub.JL;
		auto NS = sub.JS;
		std::cout << " for ZL block: ";
		print(NL, KL, TL*Al);
		std::cout << " "; 
		print(NS, KL, TS*Al);
		std::cout << std::endl;

		std::cout << " for ZS  block: ";
		print(NL, KS, TL*Al);
		std::cout << " "; 
		print(NS, KS, TS*Al);
		std::cout << std::endl;
	}

};



#endif   /* ----- #ifndef _PARTITION_H_ ----- */
 
