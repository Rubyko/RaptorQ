/*
 * =====================================================================================
 *
 *       Filename:  test1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月01日 08时07分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "alingment.h"
#include <chrono>

int main()
{
	Mat mat1(2, 2);
	Mat mat2(2, 2);

	mat1.setRandom();
	mat2.setZero();

	mat2 = mat1.eval();

	AlMat* mt1 = static_cast<AlMat*>(&mat1);
	AlMat* mt2 = static_cast<AlMat*>(&mat2);


//	std::cout << mt1->row(0).blck << std::endl;	
	long i = 0;
	long average = 0;
	while(true)
	{
		auto start = std::chrono::high_resolution_clock::now();
	//	mt1->row(0) += mt2->row(0);

//		mat1 = mat1*mat2;

		auto end = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		average+=diff;
		std::cout << average / (++i) << std::endl;
	}
//	std::cout << mt1->row(0).blck << std::endl;

	return 0;
}
