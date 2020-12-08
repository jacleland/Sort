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
#include <unistd.h>
#include <string.h>
//Library includes
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
//Local includes
#include "sorter.h"

//Integer library namespace
using namespace JAC::Integer;

//For high-resolution clock
using namespace std::chrono;

/**	@brief	Sorter program entry point
 *	@param	argc	Number of arguments passed on command line
 *	@param	argv	Pointer to arguments
 *	@return On success, returns 0. Otherwise, returns -1
 */
int main(int argc, char** argv) {
	//Function-local decl
	Sorter sorter(argc, argv);
	int result = 0;

	try {
		//Timepoints before and after sort
		auto start = high_resolution_clock::now();
		IntArray_t sorted = sorter.sort();
		auto stop = high_resolution_clock::now();

		//Print sorted array to stdout w/millis
		auto duration = duration_cast<microseconds>(stop-start);
		double seconds = ((double)duration.count())/1000000;

		//Output timer
		std::cout << "Sorted using '" << sorter.algorithm() << "' algorithm in " <<
			std::to_string(seconds) << " seconds" << std::endl;
	}
	catch(const std::exception &e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}

	return result;
}

