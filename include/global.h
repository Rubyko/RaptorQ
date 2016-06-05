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

#ifndef  _GLOBAL_H_
#define  _GLOBAL_H_

#include "octet.h"
#include "iostream"

using Mat  = Eigen::Matrix<octet, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using Blck = Eigen::Block<Mat, -1, -1, false>;
using Perm = Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic>;

using ISI = uint32_t;
using ESI = uint32_t;

#endif   /* ----- #ifndef _GLOBAL_H_  ----- */