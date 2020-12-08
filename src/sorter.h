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
#ifndef _SORTER_INCLUDED
#define _SORTER_INCLUDED
//System includes
//Library includes
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
//Project includes
#include "sortalgorithm.h"

namespace JAC::Integer {

/**	@brief	Line reader class for data file */
class FileLine {
private:
	std::string data_;
public:
	operator uint64_t() const {
		if(data_.length() > 0) return std::stoull(data_);
		else return 0UL;
	}
	friend std::istream& operator>>(std::istream& strm, FileLine& fileline) {
		return std::getline(strm, fileline.data_);
	}
};

/**	@brief	Typedefs for vector of unsigned ints
 *	Type definitions used by API
 */
typedef std::vector<uint64_t> IntArray_t;
typedef IntArray_t::const_iterator IntArrayConstIterator_t;

/**	@brief	64-bit integer sort driver class
 *	Sorts an arbitrary-length array (std::vector<uint64_t>) of values,
 *	optionally generated. Sorting is accomplished using a specified algorithm,
 *	the implementation of which is loaded by way of the Sorter static factory
 *	interface.
 *
 *	@author	jcleland@jamescleland.com'
 *
 *	If no arguments are specified, the application assumes that the data file
 * 	exists and the values contained within it will be sorted using 'radix'
 *	algorithm. If -c is specified without data max and num values, the default
 *	values of 1000 (values from 0-999) and 1000 are assumed, respectively. The
 *	file name 'isort.dat' is assumed unless specified and the algorithm used by
 *	default is 'radix' unless specified using the -a argument.
 *
 *	Defaults:
 *		filename:			isort.dat
 *		data max:			1000
 *		num values:		1000
 *
 *	Arguments:
 *		-a						Sort algorithm to use (ie: radix, bubble, etc).
 *		-f						Data file to read/write. -c will cause the file to be created.
 *		-c						Crate the data file, overwriting the existing file if it exists.
 *		-s						The max size for random values created (only value for -c).
 *		-n						The number of values to create (only valid for -c).
 *
 */
class Sorter {
	//Default values
	const std::string	DefaultFileName 	= "isort.dat";
	const std::string DefaultAlgo				= "radix";
	const uint64_t 		DefaultDataMax 		= 1000;
	const uint64_t 		DefaultNumValues 	= 1000;

public:
	/**	@brief	Default constructor */
	Sorter();

	/**	@brief	Construct with command line arguments
	 *	@param	argc	Number of command line arguments
	 *	@param	argv	Char** to command line arguments
	 */
	Sorter(int argc, char** argv);

	/**	@brief	Destructor */
	virtual ~Sorter();

	/**	@brief	Return the algorithm used by this sorter
	 *	@return	A const std::string reference containing the algorithm name.
	 */
	inline const std::string& algorithm() const { return algorithm_; }

	/**	@brief	Sort values in array. Array data is overwritten.
	 *	@return An array containing sorted values
	 *	@throws	exception On error initializing driver or performing sort.
	 */
	virtual IntArray_t sort();

protected:
	/**	@brief	Parse command line arguments from argc/argv
	 *	@param	argc	Number of command line arguments
	 *	@param	argv	Char** to command line arguments
	 *	@throws Exception on error parsing command line arguments
	 */
	virtual void parseCommandLine(int argc, char** argv);

private:
	/**	@brief	Writes data to file according to data max and num values
	 *	@throws exception On error generating data or writing file
	 */
	void generateData();

	/**	@brief	Reads integer data from the input/output file
	 *	@returns	An array (std::vector<uint64_t>) of values as read from file.
	 *	@throws	exception On error reading data.
	 */
	IntArray_t readData();

	/**	@brief	Print the contents of the array
	 *	@param	label	Text to print before the array
	 *	@param	array	The array to output
	 */
	void printArrayToConsole(const std::string& label, const IntArray_t& array);

private:
	int					argc_;					/*! Command line arg count */
	char**			argv_;					/*! Command line argument array */
	std::string algorithm_;			/*! Sort algorithm to use */
	std::string	dataFileName_;	/*! Name of the input/output file for data */
	bool				createData_;		/*! True if generating new data */
	uint64_t		dataMax_;				/*! Maximum random value to gen */
	uint64_t		numValues_;			/*! Number of values to generate */
	bool				console_;				/*!	Print output to console? */
};

}; //End namespace

#endif //Include once
