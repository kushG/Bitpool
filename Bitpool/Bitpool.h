#ifndef __BITPOOL_H__
#define __BITPOOL_H__
#include <iostream>
#include <string.h>
// This BitPool will allocate a contiguous array of objects of type [T]. It
// internally keeps track of object state as 'in use' and 'free' using a separate
// allocation block of bits; one bit per object in the pool.
// An object is marked as 'in use' when GetObject() is called and it is
// considered to be in use by the calling system. An object is 'free' 
// when it either has not yet been retrieved by GetObject() or when it has 
// been returned to the pool by ReturnObject().
using namespace std;
template <typename T>
class BitPool
{
public:
	// Allocates a contiguous array of type [type]. Allocates some additional
	// things to keep track of BitPool state.
	// @poolSize - The number of elements to allocate for the BitPool
	BitPool(size_t poolSize);
	
	// Deallocates all items which were allocated in the initial constructor.
	~BitPool();

	// @returns A new object, internally marked as 'in use', or nullptr if
	// error (i.e. There are no other objects that can be used in this pool)
	T * GetObject();

	// Given a pointer to the [pType] object, returns it to the pool
	// for use at another time, internally marks the object as 'free'.
	// @pType The object to return to the pool
	// @returns Whether or not the return was successful. 
	bool ReturnObject(T *pType);

	// Returns an object at the given index, whether the object is 'in use' or
	// 'free'. An index out of range returns nullptr
	// @index The zero-indexed object to return from the polo
	// @returns An object at the given index, nullptr if the object is out of
	// bounds
	T * GetObjectAt(size_t index);

	// @returns The number of objects currently marked 'in use' in this BitPool
	size_t GetObjectsInUse() const;

	// @returns The pool size of this BitPool, the maximum potential
	// objects that can be called from GetObject().
	size_t GetMaxObjects() const;

private:
	T *pTypeArray_;       //! The pointer to the contiguous array of [T]
						  //TODO: add more internal control variables

	/* Each object will have a header that points to next object & 
	a boolean that checks if it is free or in use*/
	struct header {
		unsigned short* nextAddress;
		bool free;
	};

	//Based on the size provided, set the pool size
	int poolSizeInBytes;
	char *head;
	char *p;
};

template <typename T>
BitPool<T>::BitPool(size_t poolSize)
{
	/* Allocate memory based on number of objects(poolSize) & header for each object
	header = First 2 bytes to store next object address,
	         Next 1 byte for a bool to indicate if current object is free or in use*/
	poolSizeInBytes = poolSize * (sizeof(T) + 3);
	char* bitPool = new char[poolSizeInBytes];
	p = bitPool;
	head = bitPool;

	// Initialise array with null characters
	for (int i = 0; i < poolSizeInBytes; i++) {
		bitPool[i] = '/0';
	}
	cout << *(p + 1) << endl;
}

template <typename T>
BitPool<T>::~BitPool()
{
	//TODO: implement
}

template<typename T>
inline T * BitPool<T>::GetObject()
{
	unsigned short* current = (unsigned short*) head;	

	// while the free bool is not found keep searching
	while (*(current +1) != '\0') {
		current = *current;
	}
	// Set current object's header's first two bytes with address of next object
	*current = (char*)current + 2 + sizeof(T);
	// Set current object's third byte to 1 to indicate "in use"
	*(current + 1) = 1;
	// return the pointer from where the object begins
	return current +3;
}

template<typename T>
inline bool BitPool<T>::ReturnObject(T * pType)
{
	return false;
}

template<typename T>
inline T * BitPool<T>::GetObjectAt(size_t index)
{
	return nullptr;
}

template<typename T>
inline size_t BitPool<T>::GetObjectsInUse() const
{
	return size_t();
}

template<typename T>
inline size_t BitPool<T>::GetMaxObjects() const
{
	int count = 0;
	unsigned short* current = (unsigned short*) head;

	while (*(current + 1) == '\0' && current < (unsigned short*)head + poolSizeInBytes) {
		current = (unsigned short*)((char*)current + 2 + sizeof(T);)
		count++;
	}
	return size_t(count);
}

#endif //__BITPOOL_H__
