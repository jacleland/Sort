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
#include <iostream>
//Project includes
#include "radix.h"

/**	@brief	Create instance of the radix sort class from shared object
 *	@return	A new instance of RadixSort as Sorter*
 */
extern "C" JAC::Integer::SortAlgorithm* create() {
	return new JAC::Integer::RadixSort();
}

/**	@brief	Deletes the specified instance of a RadixSort, allocated by this module
 *	@param	val	Pointer to a RadixSort that was allocated by this module's
 * 							CreateInstance() function
 */
extern "C" void destroy(JAC::Integer::SortAlgorithm*& obj) {
	if(obj != nullptr) delete obj;
	obj = nullptr;
}

namespace JAC::Integer {

/**	@brief	Default constructor */
RadixSort::RadixSort() {
	count_.resize(10);
}

/**	@brief	Destructor */
RadixSort::~RadixSort() {
}

/** @brief	Implementation-specific sort method for arrays of unsigned LL
 *	@param	arr	A std::vector<uint64_t> of values to be sorted
 *	@return	A std::vector<uint64_t> containing the values from arr in sorted order
 */
SortAlgorithm::IntVector_t RadixSort::sort(SortAlgorithm::IntVector_t& arr) {
	//Method-local data declaration
	uint16_t radix = 1;
	IntVectorRIterator_t ritr;
	uint64_t idx;
	uint64_t maxval = 0;

	//Declare an output vector that matches the array argument
	SortAlgorithm::IntVector_t sorted(arr.size());

	//Pointers to source and destination arrays
	SortAlgorithm::IntVector_t* pinput = &arr;
	SortAlgorithm::IntVector_t* poutput = &sorted;

	//Loop for radix
	for(uint32_t div = 1; div < pow(10, radix); div *= 10) {
		//Set all counters to 0 and fill counters for input array
		count_.assign(10, 0);

		//For each value in the input array
		for(int arrayidx = 0; arrayidx < arr.size(); arrayidx++) {
			uint64_t val = (*pinput)[arrayidx];

			//Count digits on first pass through data
			if(div == 1 && val > maxval)
				radix = ((maxval = val) > 0) ? (int) log10 ((double) maxval) + 1 : 1;

			uint8_t countidx = (val/div)%10;
			count_[countidx]++;
		}

		//Fix up count array
		for(uint8_t countidx = 1; countidx < 10; countidx++) {
			count_[countidx] += count_[countidx - 1];
		}

		//Move elements to output array
		idx = pinput->size()-1;
		for(ritr = pinput->rbegin(); ritr != pinput->rend(); ritr++) {
			uint8_t countidx = ((*pinput)[idx]/div)%10;
			(*poutput)[count_[countidx] - 1] = (*pinput)[idx];
			count_[countidx]--; //Decrement output array index
			idx--; //Decrement source index
		}

		//Swap input/output
		SortAlgorithm::IntVector_t* temp = poutput;
		poutput = pinput;
		pinput = temp;
	}

	if(pinput != &arr)
		arr = std::move(*pinput);

	return arr;
}

}; //End namespace
