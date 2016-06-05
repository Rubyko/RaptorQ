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

#ifndef  _OCTET_H_
#define  _OCTET_H_

#include <inttypes.h>
#include <array>
#include <iostream>
#include <Eigen/Dense>
#include "tables/octet_table.h"

		constexpr int J = 255;

		struct func_ml {
			constexpr uint8_t 
			operator()(const uint8_t& u,const uint8_t& v) const noexcept
			{
				if( u == 0 || v == 0 ) 
				{
					return 0;
				} /* :REMARK:2016年05月13日 02时14分05秒:: 
					 The octet corresponding to the zero element, thich is represented by the integer 0,
					 is EXCLUDED as an index, and thus indexing starts at 1 and ranges to 255. (OCT_LOG) */
				return oct_exp[oct_log[u - 1] + oct_log[v - 1]];
			}
		};

		struct func_dv {
			constexpr uint8_t
			operator()(const uint8_t& u, const uint8_t& v) const noexcept
			{
				if( u == 0 )
				{
					return 0;
				} /* :REMARK:2016年05月13日 02时14分05秒:: 
					 The octet corresponding to the zero element, thich is represented by the integer 0,
					 is EXCLUDED as an index, and thus indexing starts at 1 and ranges to 255. (OCT_LOG) */
				return oct_exp[oct_log[u - 1] - oct_log[v - 1] + 255];
			}
		};
	
		struct func_inv {
			constexpr uint8_t 
			operator()(const uint8_t& u) const noexcept
			{	
				/* :REMARK:2016年05月13日 02时14分05秒:: 
					 The octet corresponding to the zero element, thich is represented by the integer 0,
					 is EXCLUDED as an index, and thus indexing starts at 1 and ranges to 255. (OCT_LOG) */
				return oct_exp[255 - oct_log[u - 1]];
			}
		};

		/*
		 * =====================================================================================
		 *        Class:  op
		 *  Description:  class for computing and accessing the cache(for multiplication and division)
		 * =====================================================================================
		 */
		template <typename Func>
		struct op2d {  /* :TODO:2016年05月13日 02时09分24秒:: try to use templates to merge op1d and op2d to the one common class */
			uint8_t arr[J +1][J +1];
			/* User defined constructor */
			constexpr 
			op2d(Func func) noexcept:arr{}{
				for(int i=1; i!=(J + 1); i++){
					for(int j=1; j!=(J + 1); j++){
						arr[i][j] = func(i, j);
					}
				}
			};
		};

		/*
		 * =====================================================================================
		 *        Class:  op
		 *  Description:  clas for computing and accessing the cache (for inverse)
		 * =====================================================================================
		 */
		template <typename Func>
		struct op1d { /* :TODO:2016年05月13日 02时09分24秒:: try to use templates to merge op1d and op2d to the one common class */
			uint8_t arr[J + 1];
			/* User defined constructor */
			constexpr
			op1d(Func func) noexcept:arr{}{
				for(int i=1; i!=(J + 1); i++){
					arr[i] = func(i);
				}
			};
		};

		const constexpr op2d<func_ml> ml{func_ml{}};
		const constexpr op2d<func_dv> dv{func_dv{}};
		const constexpr op1d<func_inv> inv{func_inv{}};


/*
 * =====================================================================================
 *        Class:  octet
 *  Description:  https://en.wikipedia.org/wiki/Octet_(computing)   
 * =====================================================================================
 */
struct octet
{
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet
		 * Description:  constructor
		 *--------------------------------------------------------------------------------------
		 */
		octet() noexcept : data{0}
		{
		} /* -----  end of of method octet::octet (constructor)  ----- */
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet
		 * Description:  constructor
		 *--------------------------------------------------------------------------------------
		 */
		octet(const uint8_t d) noexcept: data{d}
		{
		} /* -----  end of constructor  ----- */
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet
		 * Description:  copy constructor
		 *--------------------------------------------------------------------------------------
		 */
		octet(const octet& oct) noexcept: data{oct.data}
		{
		} /* -----  end of method Octet::Octet  (copy constructor)  ----- */
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: destructor
		 * Description:  destructor
		 *--------------------------------------------------------------------------------------
		 */
		~octet() noexcept 
		{
		} /* -----  end of destructor  ----- */   
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  Octet
		 *      Method:  operator =
		 * Description:  assignment operator
		 *--------------------------------------------------------------------------------------
		 */ 
		octet& operator=(const octet& oct) noexcept
		{
			this->data = oct.data;
			return *this;
		} /* -----  end of copy assignment operator  ----- */
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  operator =
		 * Description:  move assignment operator
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator=(octet&& oct) noexcept
		{
			this->data = oct.data;
			return *this;
		} /* -----  end of move assignment operator  ----- */
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: octet
		 * Description:  move constructor
		 *--------------------------------------------------------------------------------------
		 */
		octet(octet&& oct) noexcept : data{oct.data}
		{
		} /* -----  end of move constructor ----- */
		
		// used in eigen library for implicit conversion 
		explicit
		operator uint8_t() const {return data;}
		// used implicitly for connvertion to int
		explicit
		operator int() const { return data; }
		explicit
		operator char() const { return data; }
		
		explicit 
		operator uint32_t() const { return data;}

		bool operator==(const octet& oct)
		{
			return this->data == oct.data;
		}

		bool operator!=(const octet& oct)
		{
			return this->data != oct.data;
		}
	
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: operator+=
		 * Description:  The addition of two octets u and v is defined as the exclusive-or
		 * 				 operation, i.e. u+v = u^v.
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator+=(const octet& oct) noexcept
		{
			data^=oct.data;
			return *this;
		} /* -----  end of operator+ method ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: operator-=
		 * Description:  The substraction is defined in the same way, so we have 
		 * 				 u-v = u^v
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator-=(const octet& oct) noexcept
		{
			this->data^=oct.data;
			return *this;
		} /* -----  end of operator+ method ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: operator*=
		 * Description:  The multiplication of two octets is defined with the help of two tables
		 * 				 oct_exp and oct_log (see below). The table oct_log maps octets (other 
		 * 				 than the zero element) to non-negative integers, and oct_exp maps non-
		 * 				 negative integers to octets. 
		 *
		 * 				 For two octets u and v, we define
		 * 				 		
		 * 				 			   u*v = 0, if either a or v are 0,
		 * 				 		  oct_exp[oct_log[u] + oct_log[v]] otherwise.
		 * 				 		  
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator*=(const octet& oct) noexcept
		{
			this->data=ml.arr[this->data][oct.data];
			return *this;
		}

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: operator/=
		 * Description:  The division u / v of two octets u and v, and where v != 0, is defined 
		 * 				 as follows:
		 * 				 				
		 * 				 						u/v = 0, if u == 0,
		 * 				 		oct_exp[oct_log[u] + oct_log[v] + 255]] otherwise.
		 *
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator/=(const octet& oct) noexcept
		{
			assert(oct.data != 0  && "received zero");
			this->data=dv.arr[this->data][oct.data];
			return *this;
		} /* -----  end of operator/= method ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  octet
		 *      Method:  octet :: operator!
		 * Description:  For an octet u that is not the zero element, i.e., the multiplicative 
		 * 				 inverse of u is 
		 *
		 * 									 oct_exp[255 - oct_log[u]]
		 *
		 *--------------------------------------------------------------------------------------
		 */
		octet& operator!() noexcept
		{
			assert(this->data != 0 && "received zero");
			this->data = inv.arr[this->data];
			return *this;
		} /* -----  end of operator! method ----- */

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator+
		 *  Description:  Look the description for operator+=. 
		 * =====================================================================================
		 */
	 	friend
		octet operator+(octet lf, const octet& rh){
			return lf+=rh;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator-
		 *  Description:  Look the description for operator-=.
		 * =====================================================================================
		 */
		friend 
		octet operator-(octet lf, const octet& rh){
			return lf-=rh;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator*
		 *  Description:  Look the description for operator*=.
		 * =====================================================================================
		 */
		friend
		octet operator*(octet lf, const octet& rh)
		{
			return lf*=rh;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator/
		 *  Description:  Look the description for operator/.
		 * =====================================================================================
		 */
		friend
		octet operator/(octet lf, const octet& rh)
		{
			return lf/=rh;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator<<
		 *  Description:  Print the value of the current octet.
		 * =====================================================================================
		 */
		friend
		std::ostream& operator<<(std::ostream& os, const octet& oct)
		{
			return os << (int) oct.data;
		}

		

		 uint8_t data;

}; /* -----  end of class Octet  ----- */

// The octet denoted by alpha is the octet with the integer
// representation 2. If i is a non-negative integer 0 <= i <=256, we
// have alpha^^i = OCT_EXP[i].
inline
uint8_t alpha_pow(uint8_t i) noexcept{
	return oct_exp[i];
}

inline uint8_t abs (octet x) { return static_cast<uint8_t> (x); }

namespace Eigen {
	template<>
	struct NumTraits<octet> : NumTraits<uint8_t>{};
}


#endif   /* ----- #ifndef _OCTET_H_ ----- */
