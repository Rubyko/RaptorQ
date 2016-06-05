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

#ifndef  _GRAPH_H_
#define  _GRAPH_H_

#include "global.h"
#include <inttypes.h>
#include <vector>
#include "decoder/first_phase/row.h"

// https://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf
struct node
{
	uint32_t x;
	uint32_t number_of_nodes = 1;
	row* rw;
};
// quick-union with path compression
struct graph
{
	std::vector<node> id;
	std::vector<node> nodes_with_max_connections;

	graph(uint32_t N) : id(N)
	{
		for(uint32_t i = 0; i < N; i++)
			id[i].x = i;
	}

	void clear()
	{
		for(uint32_t i = 0; i < id.size(); i++)
			id[i].x = i;
		id.clear();
		nodes_with_max_connections.clear();
	}

	node root(uint32_t i)
	{
		while(i!=id[i].x)
		{
			id[i].x = id[id[i].x].x;
			i=id[i].x;
		}
		return id[i];
	}


	row* get_max_row()
	{
		return nodes_with_max_connections.back().rw;
	}

	void unite(row* rw)
	{
		node i = root(rw->rvec[0]);
		node j = root(rw->rvec[1]);
		id[i.x].x = j.x;
		id[j.x].number_of_nodes+=id[i.x].number_of_nodes;


		if( (nodes_with_max_connections.size() == 0)||					// dont add to the filter choosed row 
				((nodes_with_max_connections.back().number_of_nodes < id[j.x].number_of_nodes)))
		{
			id[j.x].rw = rw;
			nodes_with_max_connections.push_back(id[j.x]);
		}	
	}

};

#endif   /* ----- #ifndef _GRAPH_H_  ----- */
