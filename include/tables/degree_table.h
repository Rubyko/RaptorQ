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


#ifndef  _DEGREE_TABLE_H_
#define  _DEGREE_TABLE_H_

#include <inttypes.h>
#include <array>

const constexpr std::array<uint32_t, 31> degree_table = { 0, 5243, 529531, 704294, 791675, 844104, 879057,
		904023, 922747, 937311, 948962, 958494, 966438, 973160, 978921, 983914,
		988283, 992138, 995565, 998631, 1001391, 1003887, 1006157, 1008229,
		1010129, 1011876, 1013490, 1014983, 1016370, 1017662, 1048576 };

#endif   /* ----- #ifndef _DEGREE_TABLE_H_ ----- */
