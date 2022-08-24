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
#include <getopt.h>
//Library includes
#include <iterator>
#include <exception>
#include <algorithm>
#include <memory>
//Project includes
#include "sorter.h"

//Extern variables for command line processign using getopt
extern char*	optarg;
extern int 		optind, opterr, optopt;

namespace JAC::Integer {

/**	@brief	Default constructor */
Sorter::Sorter() :
	argc_(0),
	argv_(nullptr),
	algorithm_(DefaultAlgo),
	dataFileName_(DefaultFileName),
	outputFileName_(DefaultOutputFileName),
	createData_(false),
	dataMax_(DefaultDataMax),
	numValues_(DefaultNumValues),
	console_(true)
	{}

/**	@brief	Construct with command line arguments
	*	@param	argc	Number of command line arguments
	*	@param	argv	Char** to command line arguments
	*/
Sorter::Sorter(int argc, char** argv) :
	argc_(argc),
	argv_(argv),
	algorithm_(DefaultAlgo),
	dataFileName_(DefaultFileName),
	outputFileName_(DefaultOutputFileName),
	createData_(false),
	dataMax_(DefaultDataMax),
	numValues_(DefaultNumValues),
	console_(true)
	{}

/**	@brief	Destructor */
Sorter::~Sorter() {
	//Close input and output streams as needed
	if(ifs_.is_open()) ifs_.close();
	if(ofs_.is_open()) ofs_.close();
}

/**	@brief	Sort values in array. Array data is overwritten.
 *	@return An array containing sorted values
 *	@throws	exception On error initializing driver or performing sort.
 */
IntArray_t Sorter::sort() {
	//Declare locals
	IntArray_t array;

	try {
		//Handle command line args
		parseCommandLine(argc_, argv_);

		//New data?
		if(createData_) generateData();

		//Load data
		array = readData();

		//Sort
		std::cout << "Using Algorithm '" << algorithm_.c_str() << "'..." << std::endl;
		SortAlgorithm* psorter = SortAlgorithm::create(algorithm_);
		psorter->sort(array);

		//Output?
		if(console_) printArrayToConsole("Sorted array: ", array);

		//Write file if we have a filename for output
		if(outputFileName_.length() > 0) writeArrayToFile(array);
	}
	catch(const char* e) {
		std::cout << "Exception caught during sort: " << e << std::endl;
	}

	return array;
}

/**	@brief	Parse command line arguments from argc/argv
 *	@param	argc	Number of command line arguments
 *	@param	argv	Char** to command line arguments
 *	@throws Exception on error parsing command line arguments
 *	TODO: Bounds checking for arguments.
 */
void Sorter::parseCommandLine(int argc, char** argv) {
	//Local decl
	int opt;

	while ((opt = getopt(argc, argv, "a:f:o:cs:n:")) != -1) {
		switch (opt) {
			case 'a': //Parse sort algorithm to use
				algorithm_ = std::string(optarg);
				break;
			case 'f': //Input/output file name for data
				dataFileName_ = std::string(optarg);
				break;
			case 'o': //Output file for sorted data
				outputFileName_ = std::string(optarg);
				console_ = false;
				break;
			case 'c': //(re)create data file flag
				createData_ = true;
				break;
			case 's': //Max size of random data
				dataMax_ = atol(optarg);
				break;
			case 'n': //Number of random values to generate
				numValues_ = atol(optarg);
				break;
			case 'h': //Print usage string to stderr
			default:
				std::cout << "Generate and sort an array of unsigned 64-bit integer values." << std::endl;
				std::cout << "Usage: " << std::endl;
				std::cout << "   isort [OPTION]..." << std::endl << std::endl;
				std::cout << "Options: " << std::endl;
				std::cout << "  -a <algorithm>  The sort algorithm name - Creates an instance of the" << std::endl;
				std::cout << "                  sorter implemented by lib<algorithm>.so." << std::endl;
				std::cout << "  -f <file>       Specify the file that contains the unsorted data. " << std::endl;
				std::cout << "                  If the -c argument is specified, a new dataset will be" << std::endl;
				std::cout << "                  created and this file will be overwritten if it exists." << std::endl;
				std::cout << "  -o <file>       Output the sorted data to a file." << std::endl;
				std::cout << "  -c              Create a new unsorted dataset" << std::endl;
				std::cout << "  -n <count>      The number of random values to generate when -c is specified." << std::endl;
				std::cout << "  -s <max>        The maximum random value to generate." << std::endl;
				std::cout << "  -v              Output additional information during processing." << std::endl;
				std::cout << "  -h              Displays this help information." << std::endl << std::endl;
				std::cout << "Examples: " << std::endl << std::endl;
				std::cout << "      isort -c -s 100000 -n 1000 -a radix" << std::endl << std::endl;
				std::cout << "  Writes a new dataset containing 100000 random unsigned integer values to" << std::endl;
				std::cout << "  to the file 'isort.dat' and sorts the data using a the radix sort algorithm" << std::endl;
				std::cout << "  (Looks for implementation in libradix.so)." << std::endl << std::endl;
				std::cout << "      isort -c -s 100000 -n 1000 -f dataset.dat -a radix" << std::endl << std::endl;
				std::cout << "  Same behavior as the previous example, but the new dataset will be written" << std::endl;
				std::cout << "  to/read from the file 'dataset.dat', rather than the default 'isort.dat' file." << std::endl << std::endl;
				std::cout << "      isort -a bubble" << std::endl << std::endl;
				std::cout << "  Sorts the existing data in isort.dat using the 'bubble' algorithm (libbubble.so)." << std::endl << std::endl;
				exit(EXIT_FAILURE);
		} //switch
	} //while

	//Open input and output (if requested in args)
	ifs_.open(dataFileName_.c_str(), std::ifstream::in);
	if(!console_)
		ofs_.open(outputFileName_.c_str(), std::ifstream::out);

	return;
}

/**	@brief	Writes data to file according to data max and num values
	*	@throws exception On error generating data or writing file
	*	TODO: Bounds checking for data array
	*/
void Sorter::generateData() {
	//Locals
	IntArrayConstIterator_t itr;

	//Output
	std::cout << "Generating array data of " << numValues_ << " values between 0 and " <<
		dataMax_ << std::endl;

	//Open file for writing data
	FILE* fp = fopen(dataFileName_.c_str(), "w+");
	if(!fp) {
		//Everything is fatal here really...
		//TODO: ... Still, we can do better than const char* exception?
		std::string msg = std::string("Unable to open output file: ")
			+ dataFileName_;
		throw msg;
	}

	//Declare unsorted array of integer values
	IntArray_t array(numValues_);

	//Lambda function def for generate()ing random values
	auto f = [=]() -> uint64_t {
		return rand() % dataMax_;
	};

	//Populate array
	//TODO: Batch this rather than storing the whole thing in ram? Probably
	//	need a reasonable MAX_VALUE for numValues_ in the meantime...
	array.resize(numValues_);
	std::generate(array.begin(), array.end(), f);

	//Write data to file
	for(itr = array.begin(); itr != array.end(); itr++) {
		fprintf(fp, "%lu\n", *itr);
	}

	//Close
	fflush(fp);
	fclose(fp);
}

/**	@brief	Reads integer data from the input/output file
 *	@returns	An array (std::vector<uint64_t>) of values as read from file.
 *	@throws	exception On error reading data.
 */
IntArray_t Sorter::readData() {
	//Declare input stream for data file
	//std::ifstream ifs(dataFileName_.c_str(), std::ifstream::in);

	//Populate array data from file using line reader and close
	IntArray_t array(std::istream_iterator<FileLine>{ifs_},
		std::istream_iterator<FileLine>{});
	ifs_.close();

	//Return array data
	return array;
}

/**	@brief	Print the contents of the array
 *	@param	label	Text to print before the array
 *	@param	array	The array to output
 */
void Sorter::printArrayToConsole(const std::string& label, const IntArray_t& array) {
	bool first = true;
	IntArrayConstIterator_t itr;
	//std::cout << label;
	for(itr = array.begin(); itr != array.end(); itr++) {
		//Output CR/LF or LF if not first value
		if(first)
			first = false;
		else
			std::cout << std::endl;

		//Output value
		std::cout << *(itr);
	}
	std::cout << std::endl;
}

/**
 *
 */
void Sorter::writeArrayToFile(const IntArray_t& array) {
	bool first = true;
	IntArrayConstIterator_t itr;

	for(itr = array.begin(); ofs_.is_open() && itr != array.end(); itr++) {
		//Output CR/LF or LF if not first value
		if(first)
			first = false;
		else
			ofs_ << std::endl;

		//Output value
		ofs_ << *(itr);
	}
	ofs_ << std::endl;
}

}; //End namespace
