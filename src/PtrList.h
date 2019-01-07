#ifndef PTR_LIST_H
#define PTR_LIST_H

//
template<class TYPE, int MAX_VALUE>
class cPtrList
{
	using PTR = TYPE * ;
	using value_type = TYPE;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = TYPE * ;
	using const_pointer = const TYPE *;
	using reference = TYPE & ;
	using const_reference = const TYPE&;

	struct NODE
	{
		unsigned short Sequence = 0;
		int Next = 0;
		PTR Data = nullptr;
	};

	int		m_DataIndex;		// 데이터의 인덱스(시작)
	int		m_FreeHead;			// free 노드의 인덱스(시작)
	int		m_FreeTail;
	NODE*	m_Data;				// 노드 정보

	size_t	m_Cnt;
public:
	template<class TYPE, size_t MAX_VALUE>
	class _const_iterator
	{
	public:
		using Container = cPtrList<TYPE, MAX_VALUE>;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = TYPE;
		using difference_type = ptrdiff_t;
		using pointer = const TYPE *;
		using reference = const TYPE*;
		Container* m_Cont;
		size_t m_Index;

		_const_iterator() : m_Cont(nullptr), m_Index(-1)
		{
		}

		explicit _const_iterator(Container* Cont, size_t Index) : m_Cont(Cont), m_Index(Index)
		{
		}

		reference operator*() const
		{
			return m_Cont->GetObj(m_Index);
		}

		pointer operator->() const
		{
			return m_Cont->GetObj(m_Index);
		}

		_const_iterator& operator++()
		{
			m_Index = m_Cont->GetNextDataIndex(m_Index);
			return (*this);
		}

		// postincrement
		_const_iterator operator++(int)
		{
			const_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		// predecrement
		_const_iterator& operator--()
		{
			m_Index = m_Cont->GetPrevDataIndex(m_Index);
			return (*this);
		}

		// postdecrement
		_const_iterator operator--(int)
		{
			const_iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		// == 
		bool operator==(const _const_iterator& _Right) const
		{
			return (m_Index == _Right.m_Index);
		}

		// !=
		bool operator!=(const _const_iterator& _Right) const
		{
			return (!(*this == _Right));
		}
	};

	template<class TYPE, size_t MAX_VALUE>
	class _iterator : public _const_iterator<TYPE, MAX_VALUE>
	{
	public:
		using Container = cPtrList<TYPE, MAX_VALUE>;
		using my_base = _const_iterator<TYPE, MAX_VALUE>;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = TYPE;
		using difference_type = ptrdiff_t;
		using pointer = TYPE * ;
		using reference = TYPE * ;

		_iterator()
		{
		}

		explicit _iterator(Container* Cont, size_t Index)
			: my_base(Cont, Index)
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
		_iterator& operator++()
		{
			++*static_cast<my_base *>(this);
			return (*this);
		}
		// postincrement
		_iterator operator++(int)
		{
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}
		// predecrement
		_iterator& operator--()
		{
			--*static_cast<my_base *>(this);
			return (*this);
		}
		// postdecrement
		_iterator operator--(int)
		{
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
		}
	};

	using const_iterator = _const_iterator<TYPE, MAX_VALUE>;
	using iterator = _iterator<TYPE, MAX_VALUE>;

	cPtrList()
	{
		m_Data = new NODE[MAX_VALUE];
		m_DataIndex = -1;

		// free list를 만든다.
		// free list head
		m_FreeHead = 0;
		for (int i = 0; i < MAX_VALUE; ++i)
		{
			m_Data[i].Next = i + 1;
		}
		// 끝노드는 -1
		m_Data[MAX_VALUE - 1].Next = -1;
		// free list tail 은 제일 마지막 노드
		m_FreeTail = MAX_VALUE - 1;

		m_Cnt = 0;
	}

	~cPtrList()
	{
		delete[] m_Data;
	}


	iterator begin()
	{
		return (iterator(this, m_DataIndex));
	}

	const_iterator begin() const
	{
		return (const_iterator(this, m_DataIndex));
	}

	iterator end()
	{
		return (iterator(this, -1));
	}

	const_iterator end() const
	{
		return (const_iterator(this, -1));
	}

	const_iterator cbegin() const
	{
		return (begin());
	}

	const_iterator cend() const
	{
		return (end());
	}

	constexpr size_t size() const
	{
		return (m_Cnt);
	}

	int Add(PTR Obj)
	{
		if (Obj == nullptr)
			return -1;

		NODE* NewNode = GetFreeNode();
		if (NewNode == nullptr)
			return -1;

		++m_Cnt;

		NewNode->Data = Obj;

		if (m_DataIndex == -1)
		{
			m_DataIndex = GetIndex(NewNode);
			assert(m_DataIndex >= 0 && m_DataIndex < MAX_VALUE);
			NewNode->Next = -1;

			int UniqueID = (NewNode->Sequence << 16 | m_DataIndex);
			Obj->SetUniqueID(UniqueID);

			return UniqueID;
		}
		else
		{
			int Index = GetIndex(NewNode);
			if (m_DataIndex > Index)
			{
				// 데이터 시작 위치보다 더 앞에 놓이는 경우는 이게 제일 먼저다.

				NewNode->Next = m_DataIndex;
				m_DataIndex = Index;
			}
			else
			{
				// 이전 데이터 인덱스를 구해서 내 인덱스를 넣어주고 내 다음 인덱스를 셋팅해준다.
				int PrevDataIndex = GetPrevDataIndex(Index);
				assert(PrevDataIndex != -1);

				NODE* Prev = &m_Data[PrevDataIndex];
				int Next = Prev->Next;
				Prev->Next = Index;
				NewNode->Next = Next;


			}

			int UniqueID = (NewNode->Sequence << 16 | Index);
			Obj->SetUniqueID(UniqueID);
			return UniqueID;
		}
	}

	int Remove(PTR Obj)
	{
		int UniqueID = Obj->GetUniqueID();
		return RemoveUnique(UniqueID);
	}

	int RemoveUnique(int UniqueID)
	{
		int Index = UniqueID & 0xffff;
		if (Index < 0 && Index >= MAX_VALUE)
			return -1;

		NODE* Node = &m_Data[Index];
		if (Node->Data == nullptr)
			return -1;

		// uniqueid 가 동일한가?
		if (UniqueID != (Node->Sequence << 16 | Index))
			return -1;

		return RemoveIndex(Index);
	}

	int RemoveIndex(int Index)
	{
		NODE* Node = &m_Data[Index];
		if (Node->Data == nullptr)
			return -1;

		--m_Cnt;

		int Next = Node->Next;
		Node->Data = nullptr;
		++Node->Sequence;

		if (Index == m_DataIndex)
		{
			m_DataIndex = Next;
		}
		else
		{
			// 이전 데이터 노드를 구해서
			int PrevDataIndex = GetPrevDataIndex(Index);
			assert(PrevDataIndex != -1);

			NODE* Prev = &m_Data[PrevDataIndex];
			Prev->Next = Next;
		}

		/*
		// free index 에 넣어준다.
		int FreeIndex = m_FreeHead;
		m_FreeHead = GetIndex(Node);
		Node->Next = FreeIndex;
		*/
		if (m_FreeTail == -1)
		{
			m_FreeHead = m_FreeTail = GetIndex(Node);
			Node->Next = -1;
		}
		else
		{
			m_Data[m_FreeTail].Next = GetIndex(Node);
			m_FreeTail = GetIndex(Node);
		}

		return Next;
	}

	PTR Find(int UniqueID)
	{
		int Index = UniqueID & 0xffff;
		if (Index < 0 && Index >= MAX_VALUE)
			return nullptr;

		NODE* Node = m_Data[Index];
		if (Node->Data == nullptr)
			return nullptr;

		if (UniqueID == (Node->Sequence << 16 | Index))
		{
			return Node->Data;
		}

		return nullptr;
	}

	int GetPrevDataIndex(int Index)
	{
		for (int Reverse = Index - 1; Reverse >= 0; --Reverse)
		{
			if (m_Data[Reverse].Data != nullptr)
			{
				return Reverse;
			}
		}

		return -1;
	}

	int GetNextDataIndex(int Index)
	{
		if (m_Data[Index].Data)
		{
			return m_Data[Index].Next;
		}
		else
		{
			for (int Next = Index + 1; Next < MAX_VALUE; ++Next)
			{
				if (m_Data[Next].Data != nullptr)
				{
					return Next;
				}
			}
		}
		return -1;
	}

	int GetIndex(NODE* Node)
	{
		return (int)(Node - m_Data);
	}

	NODE* GetFreeNode()
	{
		if (m_FreeHead == -1)
		{
			return nullptr;
		}

		int FreeIndex = m_Data[m_FreeHead].Next;
		NODE* Node = &m_Data[m_FreeHead];
		m_FreeHead = FreeIndex;

		if (FreeIndex == -1)
			m_FreeTail = -1;

		return Node;
	}

	PTR GetObj(int Index)
	{
		return m_Data[Index].Data;
	}
};



#endif