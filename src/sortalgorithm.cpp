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
#include <dlfcn.h>
//Library includes
//Project includes
#include "sortalgorithm.h"

namespace JAC::Integer {

//Static init
SortAlgorithm::AlgoFunctionMap_t SortAlgorithm::functionMap_;

/**	@brief	Creates an instance of the sort object with the specified name
 *	@param	name	The well-known name of the sorter to create (ie: radix)
 *	@return	An instance of the requested sort object
 *	@throws	On error creating instance, locating shared object, etc.
 */
SortAlgorithm* SortAlgorithm::create(const std::string& name) {
	//Get functions for algorithm name
	LibFunctions functions = SortAlgorithm::instanceApiFor(name);
	SortAlgorithm* sorter = (*functions.create_)();
	sorter->setTypeName(name);
	return sorter;
}

/**	@brief	Deletes the instance of the SortAlgorithm provided
 *	@param	obj	A pointer to a sorter object, created with create()
 */
void SortAlgorithm::destroy(SortAlgorithm*& obj) {
	//Get functions for algorithm name
	LibFunctions functions = SortAlgorithm::instanceApiFor(obj->typeName());
	(*functions.destroy_)(obj);
}


/**	@brief	Returns create/destroy functions for the appropriate sort algorithm
 *	@param	val	The well-known algorithm name
 *	@return	A std::pair instance creating create/destroy functions for the algo library
 */
LibFunctions SortAlgorithm::instanceApiFor(const std::string& name) {
	LibFunctions functions;

	//Create a library name from the sorter simple name
	std::string libname = std::string(LIBPREFIX)+name+LIBSUFFIX;

	//Load library and get pointers to export functions
	void* handle = ::dlopen(libname.c_str(), RTLD_NOW);

	//handle = dlopen("/lib/libchm.so.1.0.0", RTLD_NOW);
	//TODO: valid handle to loaded shared object
	if(handle == 0)
		throw std::exception();

	functions.create_ = (CreatePtr_t)dlsym(handle, "create");
	functions.destroy_ = (DestroyPtr_t)dlsym(handle, "destroy");

	//TODO: Valid function pointers
	if(functions.create_ == NULL || functions.destroy_ == NULL)
		throw std::exception();

	//Return the pair of create/destroy functions
	return functions;
}

}; //End namespace
