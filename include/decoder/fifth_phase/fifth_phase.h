/*
 * =====================================================================================
 *
 *       Filename:  fifth_phase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年05月24日 05时33分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  fifth_phase_INC
#define  fifth_phase_INC

struct fifth_phase
{
	Mat* A;
	Mat* D;
	uint32_t i;

	inline
	bool operator()() noexcept
	{
		for(uint32_t j = 0; j <= i; j++)
		{
			if((*A)(j,j) != 1)
			{
				auto coeff = (*A)(j,j);
				A->row(j) /= coeff;
				D->row(j) /= coeff;
			}
			for(uint32_t l = 0; l!= j; l++)
			{
				if((*A)(j,l)!=0)
				{
					auto coeff = (*A)(j,l);
					// A->row(j) += A->row(l) * coeff;
					xor_rows(A->row(j).data(), (A->row(l) * coeff).eval().data(), A->cols());
					xor_rows(D->row(j).data(), (D->row(l) * coeff).eval().data(), D->cols());
				}
			}
		}
		return true;
	}

	inline
	fifth_phase(Mat* A, Mat* D, uint32_t i) : A{A}, D{D}, i{i}
	{
	}

};

#endif   /* ----- #ifndef fifth_phase_INC  ----- */


