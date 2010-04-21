#pragma once

template<typename Type>
class DynArray
{
	unsigned int m_nSize;
	unsigned int m_nCap;

	Type* theStuff;

public:

	DynArray();

	~DynArray();

	DynArray(const DynArray<Type>& copy);

	DynArray<Type>& operator=(const DynArray<Type>& assign);

	Type& operator[](const unsigned int& index);

	unsigned int size() const
	{return this->m_nSize;}

	unsigned int capacity() const
	{return this->m_nCap;}

	void append(const Type& item);

	void reserve(const unsigned int & newCap);

	DynArray<Type>& reserve();

	void clear();

	void DecrementSize()
	{--(m_nSize);}

	Type& DQ();
};



template<typename Type>
DynArray<Type>::DynArray()
{
	this->m_nSize = this->m_nCap = 0;
	this->theStuff = 0;
}

template<typename Type>
DynArray<Type>::~DynArray()
{
	delete[] this->theStuff;
}
template<typename Type>
DynArray<Type>::DynArray(const DynArray<Type>& copy)
{
	this->theStuff = 0;
	this->m_nCap = copy.m_nCap;
	this->m_nSize = copy.m_nSize;

	this->theStuff = new Type[this->m_nCap];
	for(unsigned int index = 0; index < copy.m_nSize; index++)
	{
		this->theStuff[index] = copy.theStuff[index];
	}


}

template<typename Type>
DynArray<Type>& DynArray<Type>::operator=(const DynArray<Type>& assign)
{
	if(this != &assign)
	{		
		delete [] this->theStuff;

		this->m_nCap = assign.m_nCap;
		this->m_nSize = assign.m_nSize;

		this->theStuff = new Type[this->m_nCap];

		for(unsigned int index = 0; index <= this->m_nCap; index++)
		{
			this->theStuff[index] = assign.theStuff[index];
		}
	}

	return *this;
}

template<typename Type>
Type& DynArray<Type>::operator[](const unsigned int& index)
{
	return this->theStuff[index];

}


template<typename Type>
void DynArray<Type>::append(const Type& item)
{
	if(this->m_nCap == this->m_nSize)
	{
		if(this->m_nCap)
		{
			DynArray<Type> temp(*this);

			delete[] this->theStuff;

			this->m_nCap *= 2;

			this->theStuff = new Type[this->m_nCap];

			for(unsigned int index = 0; index < temp.m_nSize; index++)
			{
				this->theStuff[index] = temp.theStuff[index];
			}

			this->theStuff[this->m_nSize++] = item;

		}
		else
		{
			this->m_nCap = 1;

			this->theStuff = new Type[this->m_nCap];

			this->theStuff[m_nSize++] = item;
		}

	}
	else
		this->theStuff[m_nSize++] = item;
	// 0 1 2 3 4 5 6 7 8
}

template<typename Type>
void DynArray<Type>::reserve(const unsigned int & newCap)
{
	if(newCap > this->m_nCap && this->m_nCap)
	{

		DynArray<Type> temp(*this);

		delete[] this->theStuff;

		this->m_nCap = newCap;

		this->theStuff = new Type[this->m_nCap];

		for(unsigned int index = 0; index <= temp.m_nSize; index++)
		{
			this->theStuff[index] = temp.theStuff[index];
		}
	}
	else if(newCap > this->m_nCap)
	{
		this->m_nCap = newCap;

		this->theStuff = new Type[this->m_nCap];
	}
}

template<typename Type>
DynArray<Type>& DynArray<Type>::reserve()
{

	if(this->m_nCap)
	{
		DynArray<Type> temp(*this);

		delete[] this->theStuff;

		this->m_nCap *= 2;

		this->theStuff = new Type[this->m_nCap];

		for(unsigned int index = 0; index <= temp.m_nSize; index++)
		{
			this->theStuff[index] = temp.theStuff[index];
		}
	}
	else
	{
		this->m_nCap = 1;

		this->theStuff = new Type[this->m_nCap];
	}

	return *this;
}

template<typename Type>
void DynArray<Type>::clear()
{

	delete[] this->theStuff;
	this->m_nCap = 0;
	this->m_nSize = 0;
	this->theStuff = 0;

}

