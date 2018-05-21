#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
	T * data;
	size_t currentLength;
	size_t maxsize;
	void doubleSpace() {
		T *tmp = data;
		maxsize *= 2;
		data = (T*)operator new(maxsize * sizeof(T));
		for (size_t i = 0; i < currentLength; ++i) 
			data[i] = tmp[i];
		operator delete(tmp);
	};

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		vector<T> * myContainer;
		size_t pos;

	public:
		iterator(vector<T> *Con, size_t position) {
			myContainer = Con;
			pos = position;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			iterator here = *this;
			here.pos += n;
			return here;
		}
		iterator operator-(const int &n) const {
			iterator here = *this;
			here.pos -= n;
			return here;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			if (myContainer != rhs.myContainer) throw invalid_iterator();
			return pos - rhs.pos;
		}
		iterator operator+=(const int &n) {
			pos += n;
			return *this;
		}
		iterator operator-=(const int &n) {
			pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator here = *this;
			++pos;
			return here;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++pos;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator here = *this;
			--pos;
			return here;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--pos;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return myContainer->data[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return (myContainer == rhs.myContainer && pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const {
			return (myContainer == rhs.myContainer && pos == rhs.pos);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return (myContainer != rhs.myContainer || pos != rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const {
			return (myContainer != rhs.myContainer || pos != rhs.pos);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
	private:
		/**
		* TODO add data members
		*   just add whatever you want.
		*/
		const vector<T> * myContainer;
		size_t pos;

	public:
		const_iterator(const vector<T> *Con, size_t position): myContainer(Con), pos(position){}
		/**
		* return a new iterator which pointer n-next elements
		*   even if there are not enough elements, just return the answer.
		* as well as operator-
		*/
		const_iterator operator+(const int &n) const {
			const_iterator here = *this;
			here.poconst_iterators += n;
			return here;
		}
		const_iterator operator-(const int &n) const {
			const_iterator here = *this;
			here.pos -= n;
			return here;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const {
			if (myContainer != rhs.myContainer) throw invalid_iterator();
			return pos - rhs.pos;
		}
		const_iterator operator+=(const int &n) {
			pos += n;
			return *this;
		}
		const_iterator operator-=(const int &n) {
			pos -= n;
			return *this;
		}
		/**
		* TODO iter++
		*/
		const_iterator operator++(int) {
			const_iterator here = *this;
			++pos;
			return here;
		}
		/**
		* TODO ++iter
		*/
		const_iterator& operator++() {
			++pos;
			return *this;
		}
		/**
		* TODO iter--
		*/
		const_iterator operator--(int) {
			const_iterator here = *this;
			--pos;
			return here;
		}
		/**
		* TODO --iter
		*/
		const_iterator& operator--() {
			--pos;
			return *this;
		}
		/**
		* TODO *it
		*/
		const T& operator*() const {
			return myContainer->data[pos];
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory).
		*/
		bool operator==(const iterator &rhs) const {
			return (myContainer == rhs.myContainer && pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const {
			return (myContainer == rhs.myContainer && pos == rhs.pos);
		}
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const {
			return (myContainer != rhs.myContainer || pos != rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const {
			return (myContainer != rhs.myContainer || pos != rhs.pos);
		}
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector() {
		currentLength = 0;
		maxsize = 15;
		data = (T*)operator new(maxsize * sizeof(T));
	}
	vector(const vector &other) {
		currentLength = other.size();
		maxsize = other.capacity();
		data = (T*)operator new(maxsize * sizeof(T));
		for (size_t i = 0; i < currentLength; ++i) {
			data[i] = other.data[i];
		}
			
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (size_t i = 0; i < currentLength; ++i)
			data[i].~T();
		if (data) operator delete(data);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return *this;
		for (size_t i = 0; i < currentLength; ++i)
			data[i].~T();
		if (data) operator delete (data);
		currentLength = other.size();
		maxsize = other.capacity();
		data = (T*)operator new(maxsize * sizeof(T));
		for (size_t i = 0; i < currentLength; ++i) {
			data[i] = other.data[i];
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= currentLength) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= currentLength) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		return at(pos);
	}
	const T & operator[](const size_t &pos) const { 
		return at(pos); 
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (currentLength == 0) throw container_is_empty();
		return at(0);
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (currentLength == 0) throw container_is_empty();
		return at(currentLength - 1);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		iterator here(this, 0);
		return here;
	}
	const_iterator cbegin() const {
		const_iterator here(this, 0);
		return here;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		iterator here(this, currentLength);
		return here;
	}
	const_iterator cend() const {
		const_iterator here(this, currentLength);
		return here;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return (currentLength == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return currentLength;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {
		return maxsize;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < currentLength; ++i)
			data[i].~T();
		if (data) operator delete (data);
		currentLength = 0;
		maxsize = 10;
		data = (T*) operator new(maxsize * sizeof(T));
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if (currentLength == maxsize) doubleSpace();
		new(data + currentLength) T(value);
		for (iterator it = end(); it != pos; --it)
			*it = *(it - 1);
		*pos = value;
		++currentLength;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > currentLength) throw index_out_of_bound();
		if (currentLength == maxsize) doubleSpace();
		new(data + currentLength) T(value);
		for (size_t i = currentLength; i > ind; --i)
			data[i] = data[i - 1];
		at(ind) = value;
		++currentLength;
		return iterator(this, ind);;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for (iterator it = pos; it != end() - 1; ++it)
			*it = *(it + 1);
		data[currentLength - 1].~T();
		--currentLength;
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= currentLength) throw index_out_of_bound();
		for (size_t i = ind; i < currentLength - 1; ++i)
			data[i] = data[i + 1];
		data[currentLength - 1].~T();
		--currentLength;
		return iterator(this, ind);;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (currentLength == maxsize) doubleSpace();
		new(data + currentLength) T(value);
		++currentLength;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (currentLength == 0) throw container_is_empty();
		data[currentLength - 1].~T();
		--currentLength;
	}
};


}

#endif
