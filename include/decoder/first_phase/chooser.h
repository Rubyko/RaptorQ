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

#ifndef  _CHOOSER_H_
#define  _CHOOSER_H_

#include "systematic.h"
#include "graph.h"
#include <algorithm>

struct candidates 
{
	// the candidates 
	std::vector<row*> vec;
	int min_r; 

	void add_candidate(row* rw)
	{
		// we need to filter all min r
		if (rw->r == 0 || min_r < rw->r)
		{
			return;
		} 
		min_r = rw->r;
		// the row with min r will be always at the back of vecto
		vec.emplace_back(rw);
	}
	
	candidates(int min_r) : min_r{min_r}
	{
	}
};

struct improved_chooser
{
	const Blck* V;
	const systematic_indices sys;
	// all current information about rows 
	std::vector<row> rows;
	// set the flag. when i don't know 
	bool all_non_hdpc_rows_processed = false;

	improved_chooser(const Mat* const A, const systematic_indices& sys): sys{sys}
	{
		auto V  = A->leftCols(A->cols() - sys.P);
		rows.reserve(V.rows());
		const uint32_t upper_bound_hdpc = sys.S;
		const uint32_t lower_bound_hdpc = sys.S + sys.H;
		for(uint32_t i = 0; i != V.rows(); i++)
		{
			auto tmp = (V.row(i).cast<int>().array());
			row rw(i, (i<lower_bound_hdpc)&&(i>=upper_bound_hdpc), tmp.sum());
			rows.emplace_back(rw);
		}
	}

	candidates find_candidates()
	{
		candidates cndts(rows.size());
		for(uint32_t i = 0; i != rows.size(); i++)
		{
			rows[i].index = i;
			rows[i].rvec.clear();
			// update non zero
			rows[i].r = (V->row(i).cast<int>().array() != 0).count();
			cndts.add_candidate(&rows[i]);
			// is there still non hdpc row to process ??
			all_non_hdpc_rows_processed&=rows[i].is_hdpc;
		}
		return cndts;
	}

	bool find_r_position(row* const  rw)
	{
		bool all_ones = true;
		auto r = rw->r;
		auto offset = V->cols();
		while(r--)	
		{		
			auto row_part = V->row(rw->index).rightCols(offset);
			std::ptrdiff_t pos;
			auto coeff = row_part.cast<int>().maxCoeff(&pos);
			pos = pos + (V->cols() - offset);
			rw->rvec.push_back(pos);
			offset = V->cols() - (pos + 1);
			if(all_ones)
				all_ones = (coeff==1);
		}
		rw->all_ones = all_ones;
		return all_ones;
	}

	void update(const Blck* V)
	{
		this->V = V;
	}

	row r_eq_to_2(const candidates& cndts)
	{
		graph grph = graph(V->rows());
		auto vec = cndts.vec;
		for(auto ptr = vec.rbegin(); ptr!=vec.rend(); ++ptr)
		{
			// return false it is not one
			if(find_r_position(*ptr))
			{
				grph.unite(*ptr);
			}

			if(((ptr+1)==vec.rend()) || (((*ptr)->r) < ((*(1+ptr))->r)))
			{
				break;
			}
		}
		row r = *grph.get_max_row();
		rows[r.index] = rows[0];
		rows.erase(rows.cbegin());
		return r;
	}
	
	row r_neq_to_2(const candidates& cndts)
	{
		auto vec = cndts.vec;
		auto choosen_row_it = vec.rbegin();

		for(auto ptr = vec.rbegin(); ptr!=vec.rend(); ++ptr)
		{
			if(!(all_non_hdpc_rows_processed == (*ptr)->is_hdpc)){
				continue;
			}
			// before I used < try it when you are done with decoder 
			if((*ptr)->original_degree <= (*choosen_row_it)->original_degree) 			
			{
				choosen_row_it = ptr;
			}
			if(((ptr+1)==vec.rend()) || (((*ptr)->r) < ((*(1+ptr))->r)))
			{
				break;
			}
		}	
		(*choosen_row_it)->choosen = true;
		// search for position of r values
		find_r_position(*choosen_row_it);
		row chosen_row =  **choosen_row_it;
		rows[chosen_row.index] = rows[0];
		rows.erase(rows.cbegin());
		return chosen_row;
	}

	row choose()
	{
	  	auto cndts = find_candidates();
		row r = (cndts.min_r != 2) ? r_neq_to_2(cndts) : r_eq_to_2(cndts);	
		return r;
	}

};

#endif   /* ----- #ifndef _CHOOSER_H_  ----- */
