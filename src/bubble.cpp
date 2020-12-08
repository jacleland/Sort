/*!
 *  The latest source code can be downloaded from: [[URL]]
 *
 *  Copyright (c) 2020, James A. Cleland <jcleland at jamescleland dot com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed Addin the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//System includes
#include <math.h>
//Library includes
//Project includes
#include "bubble.h"

/**	@brief	Create instance of the radix sort class from shared object
 *	@return	A new instance of BubbleSort as Sorter*
 */
extern "C" JAC::Integer::SortAlgorithm* create() {
	return new JAC::Integer::BubbleSort();
}

/**	@brief	Deletes the specified instance of a BubbleSort, allocated by this module
 *	@param	val	Pointer to a BubbleSort that was allocated by this module's
 * 							CreateInstance() function
 */
extern "C" void destroy(JAC::Integer::SortAlgorithm*& obj) {
	if(obj != nullptr) delete obj;
	obj = nullptr;
}

namespace JAC::Integer {

/** @brief	Implementation-specific sort method for arrays of unsigned LL
 *	@param	arr	A std::vector<uint64_t> of values to be sorted
 *	@return	A std::vector<uint64_t> containing the values from arr in sorted order
 */
SortAlgorithm::IntVector_t BubbleSort::sort(SortAlgorithm::IntVector_t& arr) {
	IntVectorIterator_t itr;
	do {
		swapped_ = false;
		for(itr = arr.begin(); itr != arr.end()-1; itr++) {
			if((*itr) > (*(itr+1))) {
				swap(itr, itr+1);
			}
		}
	} while(swapped_ == true);
	return arr;
}

}; //End namespace
