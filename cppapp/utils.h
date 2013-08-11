/**
 * \file   utils.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-16
 *
 * \brief  Header file for various utilities and macros.
 */


#ifndef UTILS_B4PCQ9QU
#define UTILS_B4PCQ9QU


#include <stdint.h>
#include <climits>
#include <map>
#include <ostream>
#include <iostream>
#include <iomanip>


using namespace std;


/**
 * \brief Root namespace of the libcppapp library.  
 */
namespace cppapp {


#define PRINT_WARNING(msg) { \
	cerr << "WARNING:" << __FILE__ << ":" << __LINE__ << ": " << msg << endl; \
}

#define PRINT_EXPR(expr) { \
	cerr << #expr << " = " << (expr) << endl; \
}


////////////////////////////////////////////////////////////////////////////////
// CONVENIENCE MACROS - DECLARATION, ITERATORS, ETC.
////////////////////////////////////////////////////////////////////////////////


#define VAR(name, expr) typeof(expr) name = (expr)

#define IN_RANGE(it_name, size_expr) \
	for (int it_name = 0; it_name < (int)(size_expr); it_name++)

#define FOR_EACH(collection, it_name) for ( \
	VAR(it_name, (collection).begin()); \
	it_name != (collection).end(); \
	it_name++)

#define FOR_EACH2(collection, it_name, end_name) \
	VAR(end_name, (collection).end());          \
	for (VAR(it_name, (collection).begin());      \
	     it_name != end_name;                   \
	     it_name++)

#define FOR_EACH_ENUM(collection, it_name, enum_name) \
	for( \
		VAR(it_name, (collection).begin()); \
		it_name != (collection).end(); \
		it_name++, enum_name++)

#define FOR_EACH_RE(collection, it_name) for ( \
	__typeof__(collection)::reverse_iterator it_name = (collection).rbegin(); \
	it_name < (collection).rend(); \
	it_name++)

#define FOR_FIRST(collection, it_name) VAR(it_name, (collection).begin()); \
	if (it_name != (collection).end())

#define FOR_REST(collection, it_name) if (it_name != (collection).end()) \
	for (it_name++; it_name != (collection).end(); it_name++)

// define FIND(iter, map, expr) typeof(map.find(expr)) iter = map.find(expr); 
#define FIND(iter, map, expr) VAR(iter, map.find(expr));


////////////////////////////////////////////////////////////////////////////////
// BIT MANIPULATION AND ARCHITECTURE
////////////////////////////////////////////////////////////////////////////////


const int ONE = 1;
#define isBigEndian() ( (*(char*)&ONE) == 0 )


int bitLength(long value);


uint32_t networkToHost(uint32_t value);
uint32_t hostToNetwork(uint32_t value);

uint32_t hostToLittleEndian(uint32_t value);
uint32_t littleEndianToHost(uint32_t value);


int ceilDiv(int a, int b);

template<class T>
bool safeAdd(T a, T b, T *result)
{
	T c = a + b;
	
	if ((a > 0) && (b > 0) && (c <= 0)) {
		return false;
	}
	if ((a < 0) && (b < 0) && (c >= 0)) {
		return false;
	}
	
	*result = c;
	return true;
}

#ifdef NDEBUG
#	define DEBUG_ADD(a, b, c) { \
		{ *(c) = (a) + (b); } \
		}
#else
#	define DEBUG_ADD(a, b, c) { \
		if (!safeAdd((a), (b), (c))) { \
			LOG_WARNING("Addition overflow: *(" #c ") = (" #a ") + (" #b ")!"); \
		}
#endif


template<class T>
int getBits(T value)
{
	T mask = 1;
	int bits = 1;
	
	for (int bit = 1; bit < sizeof(T) * CHAR_BIT; bit++) {
		mask <<= 1;
		if (value & mask) bits = (bit + 1);
	}
	
	return bits;
}


template<class T>
T getMask(int bits)
{
	T mask = 0;
	for (int i = 0; i < bits; i++)
		mask = (mask << 1) | 1;
	return mask;
}


template<class K, class V>
void dumpMap(const std::map<K, V>& m, std::ostream& output)
{
	output << "MAP DUMP" << std::endl;
	FOR_EACH(m, i) {
		output << i->first << ":\t" << i->second << std::endl;
	}
	output << std::endl;
}


template<class T>
struct ByteAccess {
	union {
		T    value;
		char bytes[sizeof(T)];
	};
	
	inline void swapBytes()
	{
		#define SWAP(a, b) { char c = (a); (a) = (b); (b) = c; }
		for (size_t i = 0; i < sizeof(T) / 2; i++) {
			SWAP(bytes[i], bytes[sizeof(T) - (i + 1)]);
		}
		#undef SWAP
	}
	
	void toLittleEndian()
	{
		if (isBigEndian()) swapBytes();
	}
	
	void toBigEndian()
	{
		if (!isBigEndian()) swapBytes();
	}
};


////////////////////////////////////////////////////////////////////////////////
// CLASS / OOP UTILITIES
////////////////////////////////////////////////////////////////////////////////


template<class T>
class StaticCtor {
public:
	StaticCtor()
	{
		T::staticCtor();
	}
};


template<class T>
struct IntMetrics {
	T min;
	T max;
	
	int   count;
	float sum;
	
	IntMetrics() :
		min(0), max(0), count(0), sum(0.0)
	{ }
	
	inline void add(T value)
	{
		if ((count == 0) || (value < min)) min = value;
		if (value > max) max = value;
		sum += (float)value;
		count++;
	}
	
	inline void dump(ostream& out)
	{
		int avg = (int)(sum / (float)count);
		if (count == 0) avg = 0;
		
		out << "[" <<
			"min: " << setw(4) << right << min <<
			",  max: " << setw(4) << right << max <<
			",  count: " << setw(6) << right << count <<
			",  avg: " << setw(4) << right << avg <<
			",  sum: " << sum <<
			"]";
	}
};


#define STATIC_CTOR(className) StaticCtor<className> DUMMY_##className##_staticCtor; void className::staticCtor()
#define STATIC_CTOR_HEADER(className) \
	static StaticCtor<className> DUMMY_##className##_staticCtor; \
	static void staticCtor();
#define STATIC_CTOR_IMPL(className) \
	StaticCtor<className> className::DUMMY_##className##_staticCtor; \
	void className::staticCtor()


} // namespace cppapp


#endif /* end of include guard: UTILS_B4PCQ9QU */

