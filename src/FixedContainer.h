#pragma once

#include "Allocator.h"
#include <assert.h>

#pragma pack(push, 1)

template<size_t FIXED_SIZE>
class fixed_string
{
	friend std::wostream& operator<<(std::wostream& out, const fixed_string<FIXED_SIZE>& t) {
		out << t.fixed_string_;
		return out;
	}
private:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = wchar_t;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = value_type * ;
	using const_pointer = const value_type *;
	using reference = value_type & ;
	using const_reference = const value_type&;

	value_type fixed_string_[FIXED_SIZE]{};
public:
	// default/copy ctor
	fixed_string() = default;
	fixed_string(fixed_string const&) = default;
	fixed_string(fixed_string&&) = default;
	// copy
	explicit fixed_string(const wchar_t* words) {
		assert(wcslen(words) < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, &words[0], _TRUNCATE);
	}
	explicit fixed_string(const std::wstring& str) {
		assert(str.size() < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, str.c_str(), _TRUNCATE);
	}
	explicit fixed_string(const std::string& str) {
		assert(str.size() < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, Text::s2ws(str).c_str(), _TRUNCATE);
	}
	// assign
	fixed_string& operator=(fixed_string const&) = default;
	fixed_string& operator=(fixed_string&&) = default;
	fixed_string& operator=(const wchar_t* words) {
		assert(wcslen(words) < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, &words[0], _TRUNCATE);
		return *this;
	}
	fixed_string& operator=(const std::wstring& str) {
		assert(str.size() < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, str.c_str(), _TRUNCATE);
		return *this;
	}
	fixed_string& operator=(const std::string& str) {
		assert(str.size() < FIXED_SIZE);
		wcsncpy_s(fixed_string_, FIXED_SIZE, Text::s2ws(str).c_str(), _TRUNCATE);
		return *this;
	}
	// 실제 메모리 주소 구하기
	pointer c_str() { return fixed_string_; }
	// 버퍼 사이즈
	size_t size() const { return FIXED_SIZE; }
	// 실제로 문자 길이
	size_t length() const { return wcsnlen_s(fixed_string_, FIXED_SIZE); }
	// wchar_t 형변환
	operator wchar_t*() { return fixed_string_; }
	// wchar_t 형변환
	operator const wchar_t*() const { return fixed_string_; }
	// wstring 형변환
	//operator const std::wstring() const { return fixed_string_; }
};

template<size_t FIXED_SIZE>
fixed_string<FIXED_SIZE> make_fixed_string(const wchar_t(&words)[FIXED_SIZE])
{
	return fixed_string<FIXED_SIZE>(words);
}

template<class TYPE, size_t SIZE>
class array_const_iterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = TYPE;
	using difference_type = ptrdiff_t;
	using pointer = const TYPE *;
	using reference = const TYPE&;
	pointer _Ptr;	// 시작 포인트

	array_const_iterator() : _Ptr()
	{
	}

	explicit array_const_iterator(pointer _Parg, size_t _Off = 0) : _Ptr(_Parg + _Off)
	{
	}

	using _Unchecked_type = pointer;

	reference operator*() const
	{
		return (*_Ptr);
	}

	pointer operator->() const
	{
		return (_Ptr);
	}

	array_const_iterator& operator++()
	{
		++_Ptr;
		return (*this);
	}

	// postincrement
	array_const_iterator operator++(int)
	{
		array_const_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	// predecrement
	array_const_iterator& operator--()
	{
		--_Ptr;
		return (*this);
	}

	// postdecrement
	array_const_iterator operator--(int)
	{
		array_const_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	// increment by integer
	array_const_iterator& operator+=(ptrdiff_t _Off)
	{
		_Ptr += _Off;
		return (*this);
	}

	// return this + integer
	array_const_iterator operator+(ptrdiff_t _Off) const
	{
		array_const_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	// decrement by integer
	array_const_iterator& operator-=(ptrdiff_t _Off)
	{
		return (*this += -_Off);
	}

	// return this - integer
	array_const_iterator operator-(ptrdiff_t _Off) const
	{
		array_const_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	// return difference of iterators
	ptrdiff_t operator-(const array_const_iterator& _Right) const
	{
		return (_Ptr - _Right._Ptr);
	}

	// 인덱스
	reference operator[](ptrdiff_t _Off) const
	{
		return (*(*this + _Off));
	}

	// == 
	bool operator==(const array_const_iterator& _Right) const
	{
		return (_Ptr == _Right._Ptr);
	}

	// !=
	bool operator!=(const array_const_iterator& _Right) const
	{
		return (!(*this == _Right));
	}

	bool operator<(const array_const_iterator& _Right) const { return (_Ptr < _Right._Ptr); }
	bool operator>(const array_const_iterator& _Right) const { return (_Right < *this); }
	bool operator<=(const array_const_iterator& _Right) const { return (!(_Right < *this)); }
	bool operator>=(const array_const_iterator& _Right) const { return (!(*this < _Right)); }

};

template<class TYPE, size_t SIZE>
class array_iterator : public array_const_iterator<TYPE, SIZE>
{
public:
	using my_base = array_const_iterator<TYPE, SIZE>;
	using iterator_category = std::random_access_iterator_tag;
	using value_type = TYPE;
	using difference_type = ptrdiff_t;
	using pointer = TYPE * ;
	using reference = TYPE & ;

	array_iterator()
	{
	}

	explicit array_iterator(pointer _Parg, size_t _Off = 0)
		: my_base(_Parg, _Off)
	{
	}

	reference operator*() const
	{
		return (const_cast<reference>(my_base::operator*()));
	}

	pointer operator->() const
	{
		return (const_cast<pointer>(my_base::operator->()));
	}
	// preincrement
	array_iterator& operator++()
	{
		++*static_cast<my_base *>(this);
		return (*this);
	}
	// postincrement
	array_iterator operator++(int)
	{
		array_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}
	// predecrement
	array_iterator& operator--()
	{
		--*static_cast<my_base *>(this);
		return (*this);
	}
	// postdecrement
	array_iterator operator--(int)
	{
		array_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}
	// increment by integer
	array_iterator& operator+=(ptrdiff_t _Off)
	{
		*static_cast<my_base *>(this) += _Off;
		return (*this);
	}
	array_iterator operator+(ptrdiff_t _Off) const
	{
		array_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	array_iterator& operator-=(ptrdiff_t _Off)
	{
		return (*this += -_Off);
	}

	array_iterator operator-(ptrdiff_t _Off) const
	{
		array_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	ptrdiff_t operator-(const my_base& _Right) const
	{
		return (*static_cast<const my_base *>(this) - _Right);
	}

	reference operator[](ptrdiff_t _Off) const
	{	// subscript
		return (*(*this + _Off));
	}
};


// 고정 사이즈 배열
template<class TYPE, size_t SIZE>
class fixed_array
{
public:
	using value_type = TYPE;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = TYPE * ;
	using const_pointer = const TYPE *;
	using reference = TYPE & ;
	using const_reference = const TYPE&;

	using const_iterator = array_const_iterator<TYPE, SIZE>;
	using iterator = array_iterator<TYPE, SIZE>;

	// Defaulted Constructors and Operators
	fixed_array() = default;
	fixed_array(fixed_array const&) = default;
	fixed_array(fixed_array&&) = default;
	fixed_array& operator=(fixed_array const&) = default;
	fixed_array& operator=(fixed_array&&) = default;

	static_assert(SIZE > 0, "SIZE > 0");
	TYPE _Elems[SIZE]{};

	explicit fixed_array(const TYPE(&value)[SIZE])
	{
		for (size_t Idx = 0; Idx < SIZE; ++Idx) {
			operator[](Idx) = value[Idx];
		}
	}
	explicit fixed_array(const t_vector<TYPE>& Vector)
	{
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			operator[](Idx) = Vector[Idx];
		}
	}
	template<size_t ARRAY_SIZE>
	explicit fixed_array(const std::array<TYPE, ARRAY_SIZE>& Vector)
	{
		static_assert(ARRAY_SIZE <= SIZE, "array size is too big");
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			operator[](Idx) = Vector[Idx];
		}
	}
	fixed_array& operator=(const TYPE(&value)[SIZE]) {
		for (size_t Idx = 0; Idx < SIZE; ++Idx) {
			operator[](Idx) = value[Idx];
		}
		return *this;
	}
	fixed_array& operator=(const t_vector<TYPE>& Vector) {
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			operator[](Idx) = Vector[Idx];
		}
		return *this;
	}
	template<size_t ARRAY_SIZE>
	fixed_array& operator=(const std::array<TYPE, ARRAY_SIZE>& Vector)
	{
		static_assert(ARRAY_SIZE <= SIZE, "array size is too big");
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			operator[](Idx) = Vector[Idx];
		}
		return *this;
	}

	iterator begin()
	{
		return (iterator(_Elems, 0));
	}

	const_iterator begin() const
	{
		return (const_iterator(_Elems, 0));
	}

	iterator end()
	{
		return (iterator(_Elems, SIZE));
	}

	const_iterator end() const
	{
		return (const_iterator(_Elems, SIZE));
	}

	const_iterator cbegin() const
	{
		return (begin());
	}

	const_iterator cend() const
	{
		return (end());
	}

	constexpr size_type size() const
	{
		return (SIZE);
	}

	constexpr size_type max_size() const
	{
		return (SIZE);
	}

	constexpr bool empty() const
	{
		return (false);
	}

	reference operator[](size_type _Pos)
	{
#if _DEBUG
		if (SIZE <= _Pos)
		{
			assert(!"array subscript out of range");
		}
#endif
		return (_Elems[_Pos]);
	}

	constexpr const_reference operator[](size_type _Pos) const
	{
#if _DEBUG
		if (SIZE <= _Pos)
		{
			assert(!"array subscript out of range");
		}
#endif
		return (_Elems[_Pos]);
	}

	reference front()
	{
		return (_Elems[0]);
	}

	constexpr const_reference front() const
	{
		return (_Elems[0]);
	}

	reference back()
	{
		return (_Elems[SIZE - 1]);
	}

	constexpr const_reference back() const
	{
		return (_Elems[SIZE - 1]);
	}

	TYPE * data()
	{
		return (_Elems);
	}

	const TYPE * data() const
	{
		return (_Elems);
	}
};

// 가변길이 사이즈 배열
template<class TYPE, size_t SIZE, class CTYPE = int>
class varying_array
{
public:
	using value_type = TYPE;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = TYPE * ;
	using const_pointer = const TYPE *;
	using reference = TYPE & ;
	using const_reference = const TYPE&;

	using const_iterator = array_const_iterator<TYPE, SIZE>;
	using iterator = array_iterator<TYPE, SIZE>;

	CTYPE _Cnt = 0;
	TYPE _Elems[SIZE]{};

	// Defaulted Constructors and Operators
	varying_array() = default;
	varying_array(varying_array const&) = default;
	varying_array(varying_array&&) = default;
	varying_array& operator=(varying_array const&) = default;
	varying_array& operator=(varying_array&&) = default;

	static_assert(SIZE > 0, "SIZE > 0");

	explicit varying_array(const TYPE(&value)[SIZE])
	{
		clear();
		for (size_t Idx = 0; Idx < SIZE; ++Idx) {
			insert(value[Idx]);
		}
	}
	explicit varying_array(const t_vector<TYPE>& Vector)
	{
		clear();
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			insert(Vector[Idx]);
		}
	}
	template<size_t ARRAY_SIZE>
	explicit varying_array(const std::array<TYPE, ARRAY_SIZE>& Vector)
	{
		static_assert(ARRAY_SIZE <= SIZE, "array size is too big");
		clear();
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			insert(Vector[Idx]);
		}
	}
	varying_array& operator=(const TYPE(&value)[SIZE]) {
		clear();
		for (size_t Idx = 0; Idx < SIZE; ++Idx) {
			insert(value[Idx]);
		}
		return *this;
	}
	varying_array& operator=(const t_vector<TYPE>& Vector) {
		clear();
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			insert(Vector[Idx]);
		}
		return *this;
	}
	template<size_t ARRAY_SIZE>
	varying_array& operator=(const std::array<TYPE, ARRAY_SIZE>& Vector)
	{
		static_assert(ARRAY_SIZE <= SIZE, "array size is too big");
		clear();
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			insert(Vector[Idx]);
		}
		return *this;
	}
	template<size_t ARRAY_SIZE>
	varying_array& operator=(const fixed_array<TYPE, ARRAY_SIZE>& Vector)
	{
		static_assert(ARRAY_SIZE <= SIZE, "array size is too big");
		clear();
		for (size_t Idx = 0; Idx < Vector.size(); ++Idx) {
			if (SIZE <= Idx)
			{
				assert(!"array subscript out of range");
				break;
			}
			insert(Vector[Idx]);
		}
		return *this;
	}

	iterator begin()
	{
		return (iterator(_Elems, 0));
	}

	const_iterator begin() const
	{
		return (const_iterator(_Elems, 0));
	}

	iterator end()
	{
		return (iterator(_Elems, _Cnt));
	}

	const_iterator end() const
	{
		return (const_iterator(_Elems, _Cnt));
	}

	const_iterator cbegin() const
	{
		return (begin());
	}

	const_iterator cend() const
	{
		return (end());
	}

	constexpr CTYPE size() const
	{
		return (_Cnt);
	}

	void resize(CTYPE Cnt)
	{
		if (SIZE <= Cnt)
		{
			assert(!"array subscript out of range");
			return;
		}
		_Cnt = Cnt;
	}

	constexpr CTYPE max_size() const
	{
		return (SIZE);
	}

	constexpr bool empty() const
	{
		return (_Cnt == 0);
	}

	constexpr bool full() const
	{
		return (_Cnt == SIZE);
	}

	void clear()
	{
		_Cnt = 0;
	}

	void insert(const TYPE& _Val)
	{
		if (_Cnt >= SIZE)
		{
			assert(!"array subscript out of range");
			return;
		}

		_Elems[_Cnt] = _Val;
		_Cnt++;
	}

	void insert(const TYPE&& _Val)
	{
		if (_Cnt >= SIZE)
		{
			assert(!"array subscript out of range");
			return;
		}

		_Elems[_Cnt] = _Val;
		_Cnt++;
	}

	TYPE& new_ref()
	{
		if (_Cnt >= SIZE)
		{
			assert(!"array subscript out of range");
			return _Elems[SIZE-1];
		}

		TYPE& Tmp = _Elems[_Cnt];
		_Cnt++;

		return Tmp;
	}

	void push_back(const TYPE& _Val)
	{
		insert(_Val);
	}

	void push_back(TYPE&& _Val)
	{
		insert(std::forward<TYPE>(_Val));
	}

	reference operator[](size_type _Pos)
	{
		if (SIZE <= _Pos)
		{
			assert(!"array subscript out of range");
			return _Elems[SIZE-1];
		}

		if (_Cnt <= _Pos)
		{
			_Cnt = (CTYPE)(_Pos + 1);
		}

		return (_Elems[_Pos]);
	}

	constexpr const_reference operator[](size_type _Pos) const
	{
#if _DEBUG
		if (_Cnt <= _Pos)
		{
			assert(!"array subscript out of range");
			return (_Elems[SIZE-1]);
		}
#endif
		return (_Elems[_Pos]);
	}

	reference front()
	{
		return (_Elems[0]);
	}

	constexpr const_reference front() const
	{
		return (_Elems[0]);
	}

	reference back()
	{
		return (_Elems[SIZE - 1]);
	}

	constexpr const_reference back() const
	{
		return (_Elems[SIZE - 1]);
	}

	TYPE * data()
	{
		return (_Elems);
	}

	const TYPE * data() const
	{
		return (_Elems);
	}

};

#pragma pack(pop)

