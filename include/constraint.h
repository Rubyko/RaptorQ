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

#ifndef _CONSTRAINT_H_ 
#define _CONSTRAINT_H_

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <Eigen/Dense>
#include "octet.h"
#include <algorithm>
#include "global.h"
#include "systematic.h"
#include "debug.h"
#include "generators/random.h"


/*
 * =====================================================================================
 *        Class:  constraint
 *  Description:  The constraint matrix defines the constraint relationship among the L intermediate symbols C[0],...,C[L-1].  
 * =====================================================================================
 */
class constraint
{
		const systematic_indices& sys;		
		Mat* const A;

		inline
		void G_LDPC_1(){
			auto G_LDPC_1 = this->A->topLeftCorner(sys.S, sys.B);
			// for i = 0, ..., B-1, page 23
			for(uint32_t i = 0; i != sys.B; i++){
				uint32_t a = 1 + std::floor(i/sys.S);
				uint32_t b = i % sys.S;
				G_LDPC_1(b, i) = 1;
				b = (b + a) % sys.S;
				G_LDPC_1(b, i) = 1;
				b = (b + a) % sys.S;
				G_LDPC_1(b, i) = 1;
			}
		}

		inline
		void G_LDPC_2(){
			auto G_LDPC_2 = this->A->topRightCorner(sys.S, sys.P); 
			// for i = 0, ..., S-1, page 23
			for(uint32_t i = 0; i != sys.S; i++) {
				G_LDPC_2(i, i % sys.P) = 1;
				G_LDPC_2(i, (i + 1) % sys.P) = 1;
			}
		}

		inline
		void I_H(){
			this->A->block(sys.S, sys.K_prime + sys.S,sys.H, sys.H).setIdentity();
		}

		inline
		void I_S(){
			this->A->block(0, sys.B, sys.S, sys.S).setIdentity();
		}

		inline
		auto GAMMA() -> Mat {
			auto GAMMA = Mat(sys.K_prime + sys.S, sys.K_prime + sys.S);
			GAMMA.setZero();
			for(int i = 0; i != GAMMA.rows(); i++){
				for(int j = 0; j != GAMMA.cols(); j++){
					if(i>=j){
						GAMMA(i, j) = alpha_pow(i - j);
					} 
				}
			}
			return GAMMA;
		}

		inline
		auto MT() -> Mat{
			auto MT = Mat(sys.H, sys.K_prime + sys.S);
			MT.setZero();
			random_generator rnd;

			// from j = 0 to K' + S - 2
			for (uint32_t j = 0; j != sys.K_prime + sys.S - 1; j++) {
				auto i = rnd.generate(j + 1, 6, sys.H);
				MT(i, j) = 1;

				i = (rnd.generate(j + 1, 6, sys.H) + rnd.generate(j + 1, 7, sys.H - 1) + 1) % sys.H;
				MT(i, j) = 1;
			}

			{
				auto j = sys.K_prime + sys.S - 1;
				// from i = 0 to H - 1
				for (uint32_t i = 0; i != sys.H; i++){
					MT(i, j) = alpha_pow(i);	
				}
			}
			return MT;
		}
		
		inline
		void G_HDPC(){
			auto MT = this->MT();
			auto GAMMA = this->GAMMA();
			auto G_HDPC = MT * GAMMA;
			this->A->block(sys.S, 0, G_HDPC.rows(), G_HDPC.cols()) = G_HDPC;
		}


	public:
		/* ====================  LIFECYCLE     ======================================= */
		inline
		constraint(const systematic_indices& sys, Mat* A) : sys{sys}, A{A}
		{
			// L = P + W or L = K' + S + H
			G_LDPC_1();			
			G_LDPC_2();		

			I_S();		
			I_H();		
			
			G_HDPC();	
		}

		inline friend 
		std::ostream& operator<<(std::ostream& os, const constraint& cnstr){
			return os << std::hex << std::uppercase << cnstr.A;
		}

}; /* -----  end of class constraint  ----- */



#endif   /* ----- #ifndef _CONSTRAINT_H_ ----- */
