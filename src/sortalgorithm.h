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
#ifndef _SORTALGORITHM_INCLUDED
#define _SORTALGORITHM_INCLUDED
//System includes
//Library includes
#include <string>
#include <vector>
#include <map>
#include <utility>

//TODO: Platform-specific library prefix and extension
#ifdef __gnu_linux__
	#define LIBPREFIX "lib"
	#define LIBSUFFIX ".so"
#else
	#define LIBPREFIX ""
	#define LIBSUFFIX ".dll"
#endif

namespace JAC::Integer {

//Forward decl
class SortAlgorithm;

//Built-in vector types and other typedefs
typedef SortAlgorithm* (*CreatePtr_t)();
typedef void (*DestroyPtr_t)(SortAlgorithm*&);

/**	@brief	Object used to hold create/destroy function pointers
 *	@author	jcleland@jamescleland.com
 */
class LibFunctions {
public:
	LibFunctions() : create_(nullptr), destroy_(nullptr) {};
	LibFunctions(CreatePtr_t c, DestroyPtr_t d) :
		create_(c), destroy_(d) {};
	CreatePtr_t create_;
	DestroyPtr_t destroy_;
};

/**	@brief	Integer sort using dynamically loaded sorting algorithms
 *	@author	jcleland@jamescleland.com
 */
class SortAlgorithm {
public:
	//Sort vector and iterator typedefs
	typedef std::vector<uint64_t> 				IntVector_t;
	typedef IntVector_t::iterator 				IntVectorIterator_t;
	typedef IntVector_t::reverse_iterator IntVectorRIterator_t;

private:
	//Pair type for function map
	typedef std::pair<std::string, LibFunctions>	AlgoFunctionMapPair_t;

	//Map of well-known algo names to create/destroy function pointers
	typedef std::map<std::string,LibFunctions> 		AlgoFunctionMap_t;

	/**	@brief	Map instance containing well-known agorithm name to create/destroy functions
	 *	TODO: Not thread-safe
	 */
	static AlgoFunctionMap_t	functionMap_;

	//The type name of the derived sorter instance
	std::string typeName_;

protected:
	/**	@brief	Default constructor */
	SortAlgorithm() {}

public:
	/**	@brief	Destructor */
	virtual ~SortAlgorithm() {}

	/**	@brief	Creates an instance of the sort object with the specified name
	 *	@param	name	The well-known name of the sorter to create (ie: radix)
	 *	@return	An instance of the requested sort object
	 *	@throws	On error creating instance, locating shared object, etc.
	 */
	static SortAlgorithm* create(const std::string& name);

	/**	@brief	Deletes the instance of the SortAlgorithm provided
	 *	@param	obj	A pointer to a sorter object, created with create()
	 */
	static void destroy(SortAlgorithm*& obj);

	/**
	 */
	void setTypeName(const std::string& typeName) {
		typeName_ = typeName;
	}

	/**	@brief	Returns a text string identifying the name of the integer sorter
	 *	@return	A std::string reference containing the sorter type name
	 */
	const std::string& typeName() const {
		return typeName_;
	}

	/** @brief	Implementation-specific sort method for arrays of unsigned LL
	 *	@param	arr	A std::vector<uint64_t> of values to be sorted
	 *	@return	A std::vector<uint64_t> containing the values from arr in sorted order
	 */
	virtual IntVector_t sort(IntVector_t& arr) = 0;

private:
	/**	@brief	Returns create/destroy functions for the appropriate sort algorithm
	 *	@param	val	The well-known algorithm name
	 *	@return	A std::pair instance creating create/destroy functions for the algo library
	 */
	static LibFunctions instanceApiFor(const std::string& name);
};

}; //End namespace

#endif //Include once
