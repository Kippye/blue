#include "blf/templatearrayconstiterator.hpp"

namespace blf
{
    TemplateArrayConstIterator& TemplateArrayConstIterator::operator++()
	{
		m_pointer++;
		return *this;
	}

	TemplateArrayConstIterator TemplateArrayConstIterator::operator++(int)
	{
		TemplateArrayConstIterator iterator = *this;
		++(*this);
		return iterator;
	}

	TemplateArrayConstIterator& TemplateArrayConstIterator::operator--()
	{
		m_pointer--;
		return *this;
	}

	TemplateArrayConstIterator TemplateArrayConstIterator::operator--(int)
	{
		TemplateArrayConstIterator iterator = *this;
		--(*this);
		return iterator;
	}

	TemplateObject* TemplateArrayConstIterator::operator[](int index)
	{
		return *(m_pointer + index);
	}

	TemplateObject** TemplateArrayConstIterator::operator->()
	{
		return m_pointer;
	}

	TemplateObject* TemplateArrayConstIterator::operator*()
	{
		return *m_pointer;
	}

	bool TemplateArrayConstIterator::operator==(const TemplateArrayConstIterator& other) const
	{
		return m_pointer == other.m_pointer;
	}

	bool TemplateArrayConstIterator::operator!=(const TemplateArrayConstIterator& other) const
	{
		return !(m_pointer == other.m_pointer);
	}
}