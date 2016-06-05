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

#ifndef  _ROW_H_
#define  _ROW_H_

struct row
{
	uint32_t index;
	bool is_hdpc;
	uint32_t original_degree;
	bool choosen = false;
	int r = -1;
	bool all_ones = false;
	// map with r value positionss
	std::vector<uint32_t> rvec;

	uint32_t i;
	uint32_t u;

	row() = default;
	row& operator=(const row& rw) = default;
	row& operator=(row&& rw) = default;
	row(const row& rw) = default;
	~row() = default;

	row(uint32_t index, bool is_hdpc, uint32_t original_degree) : index{index}, is_hdpc{is_hdpc}, original_degree{original_degree}
	{
	}

};


#endif   /* ----- #ifndef _ROW_H_  ----- */
