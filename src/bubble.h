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
#ifndef _BUBBLE_INCLUDED
#define _BUBBLE_INCLUDED
//System includes
#include <bits/stdint-intn.h>
//Library includes
#include <string>
#include <vector>
//Project includes
#include "sortalgorithm.h"

namespace JAC::Integer {

/**	@brief	Radix sort implementation for sorting library
 *	@author	jcleland@jamescleland.com
 */
class BubbleSort : public SortAlgorithm {
private:
	//The sort type string
	std::string 			type_ = "radix";

	//Finished when no items swapped
	bool							swapped_ = false;

public:
	/**	@brief	Default constructor */
	BubbleSort() {};

	/**	@brief	Destructor */
	virtual ~BubbleSort() {};

	/** @brief	Implementation-specific sort method for arrays of unsigned LL
	 *	@param	arr	A std::vector<uint64_t> of values to be sorted
	 *	@return	A std::vector<uint64_t> containing the values from arr in sorted order
	 */
	IntVector_t sort(IntVector_t& arr) override;

protected:
	inline void swap(IntVectorIterator_t left, IntVectorIterator_t right) {
		uint64_t temp = *left;
		*left = *right;
		*right = temp;
		swapped_ = true;
	}
};

BubbleSort __bubblesort_instance;

}; //End namespace

#endif //Include once
