// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <algorithm>
#include <iterator>
#include <type_traits>

namespace core::record {

/// The `RecordIterator` template class implements the
/// `std::random_access_iterator` concept for a contiguous sequence of
/// equal sized records. This faciliates using generic algorithms,
/// such as `std::sort`, with run-time sized value types.
/// 
template<class T, bool SwapRanges = true, bool HeapValueType = true>
struct Iterator {
    using storage_type = T;
    using storage_pointer = storage_type*;

    using iterator_category = std::random_access_iterator_tag;

    template<size_t N> struct stack_value_type;
    struct heap_value_type;
    using value_type = std::conditional_t<HeapValueType, heap_value_type, stack_value_type<56>>;
    
    using difference_type = std::ptrdiff_t;

    using pointer = storage_pointer;
    
    struct reference {
	explicit reference(storage_pointer data, size_t size)
	    : data_(data)
	    , size_(size) {
	}

	reference(const reference& other) = default;
	
	reference& operator=(const reference other) {
	    std::copy(other.data_, other.data_ + size_, data_);
	    return *this;
	}
	
	reference(value_type&);
	
	reference& operator=(const value_type&);
	
	operator const storage_pointer() const {
	    return data_;
	}

	storage_pointer data() const {
	    return data_;
	}

	size_t size() const {
	    return size_;
	}

	friend void swap(reference a, reference b) {
	    if constexpr (SwapRanges) {
		std::swap_ranges(a.data(), a.data() + a.size(), b.data());
	    } else {
		auto size = a.size();
		storage_pointer buffer = (storage_pointer)alloca(size * sizeof(storage_type));
		std::copy(a.data(), a.data() + size, buffer);
		std::copy(b.data(), b.data() + size, a.data());
		std::copy(buffer, buffer + size, b.data());
	    }
	}
	
    private:
	storage_pointer data_;
	size_t size_;
    };

    template<size_t N = 56>
    struct stack_value_type {
	stack_value_type()
	    : size_(0)
	    , data_(nullptr) {
	}
	
	stack_value_type(reference r)
	    : stack_value_type(r.size(), r.data()) {
	}

	stack_value_type(const stack_value_type& other) noexcept
	    : stack_value_type(other.size(), other.data()) {
	}

	stack_value_type(stack_value_type&& other) noexcept
	    : size_(other.size())
	    , data_(use_stack() ? &arr[0] : other.data_) {
	    if (use_stack())
		std::copy(other.data(), other.data() + other.size(), data_);
	    other.data_ = nullptr;
	}

	stack_value_type& operator=(stack_value_type&& other) noexcept {
	    std::swap(size_, other.size_);
	    std::swap(data_, other.data_);
	    std::swap(arr, other.arr);
	    return *this;
	}

	~stack_value_type() {
	    if (not use_stack())
		free(data_);
	}

	operator const storage_pointer() const {
	    return data_;
	}

	const storage_pointer data() const {
	    return data_;
	}

	storage_pointer data() {
	    return data_;
	}

	size_t size() const {
	    return size_;
	}

    private:
	stack_value_type(size_t n, const T *src) noexcept
	    : size_(n)
	    , data_(use_stack() ? &arr[0] : (storage_pointer)std::malloc(sizeof(T) * n)) {
	    std::copy(src, src + n, data_);
	}
	
	bool use_stack() const {
	    return size_ <= N;
	}
	
	size_t size_;
	T *data_;
	T arr[N];
    };

    struct heap_value_type {
	heap_value_type()
	    : data_(nullptr)
	    , size_(0) {
	}
	    
	heap_value_type(reference r)
	    : heap_value_type(r.size(), r.data()) {
	}

	heap_value_type(const heap_value_type& other)
	    : heap_value_type(other.size(), other.data()) {
	}
	
	heap_value_type(heap_value_type&& other) noexcept
	    : data_(other.data_)
	    , size_(other.size()) {
	    other.data_ = nullptr;
	}

	heap_value_type& operator=(heap_value_type&& other) noexcept {
	    std::swap(data_, other.data_);
	    std::swap(size_, other.size_);
	    return *this;
	}

	~heap_value_type() {
	    free(data_);
	}

	operator const storage_pointer() const {
	    return data_;
	}

	const storage_pointer data() const {
	    return data_;
	}

	storage_pointer data() {
	    return data_;
	}

	size_t size() const {
	    return size_;
	}

    private:
	heap_value_type(size_t n, const T *src)
	    : data_((storage_pointer)std::malloc(sizeof(T) * n))
	    , size_(n) {
	    std::copy(src, src + n, data_);
	}
	
	storage_pointer data_{nullptr};
	size_t size_;
    };

    Iterator()
	: data_(nullptr)
	, size_(0) {
    }

    Iterator(storage_pointer data, size_t size)
	: data_(data)
	, size_(size) {
    }

    template<template<class...> class Container>
    Iterator(Container<storage_type>& data, size_t size)
	: Iterator(data.data(), size) {
    }

    storage_pointer data() {
	return data_;
    }

    const storage_pointer data() const {
	return data_;
    }

    size_t size() const {
	return size_;
    }

    reference operator*() const {
	return reference{data_, size_};
    }

    reference operator[](size_t index) const {
	return *(*this + index);
    }
    
    Iterator& operator++() {
	data_ += size_;
	return *this;
    }
    
    Iterator& operator--() {
	data_ -= size_;
	return *this;
    }
    
    Iterator operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
    }

    Iterator operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
    }

    Iterator& operator+=(size_t n) {
	data_ += n * size_;
	return *this;
    }

    Iterator operator+(size_t n) const {
	auto r = *this;
	r += n;
	return r;
    }

    Iterator& operator-=(size_t n) {
	data_ -= n * size_;
	return *this;
    }

    Iterator operator-(size_t n) const {
	auto r = *this;
	r -= n;
	return r;
    }

    friend auto operator+(difference_type n, const Iterator& a) {
	Iterator b(a);
	b += n;
	return b;
    }

    friend difference_type operator-(const Iterator& a, const Iterator& b) {
	return (a.data_ - b.data_) / a.size_;
    }

    friend auto operator<=>(const Iterator& a, const Iterator& b) = default;
    friend bool operator==(const Iterator& a, const Iterator& b) = default;

private:
    storage_pointer data_;
    size_t size_;
};

template<class T, bool SwapRanges, bool HeapValueType>
Iterator<T, SwapRanges, HeapValueType>::reference::reference(Iterator<T, SwapRanges, HeapValueType>::value_type& value)
    : data_(value.data())
    , size_(value.size()) {
}

template<class T, bool SwapRanges, bool HeapValueType>
typename Iterator<T, SwapRanges, HeapValueType>::reference& Iterator<T, SwapRanges, HeapValueType>::reference::operator=
(const Iterator<T, SwapRanges, HeapValueType>::value_type& value) {
    std::copy(value.data(), value.data() + value.size(), data());
    return *this;
}

/// Return the begin iterator for the given container `data` with records of `n` elements.
template<class T>
auto begin(T& data, size_t n) {
    return Iterator(data.data(), n);
}

/// Return the end iterator for the given container `data` with records of `n` elements.
template<class T>
auto end(T& data, size_t n) {
    return Iterator(data.data() + data.size(), n);
}

}; // core::record
