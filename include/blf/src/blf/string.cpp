#include "blf/string.hpp"

#include <iostream>
#include <cstring>

namespace blf
{
	void String::setupCString(const char* string, size_t bufferSize)
	{	
		// Assigning the char buffer, hopefully doesn't blow up.
		m_stringBuffer = (char*)string;

		// Sizes.
		m_stringSize = bufferSize - 1;
		m_bufferSize = bufferSize;
	}

	void String::copyCString(const String& otherString)
	{
		// Copying size data, since we aren't changing the buffer size.
		m_stringSize = otherString.m_stringSize;
		m_bufferSize = otherString.m_bufferSize;

		// Copying the buffer.
		m_stringBuffer = new char[m_bufferSize];

		memcpy(m_stringBuffer, otherString.m_stringBuffer, m_bufferSize);
		/*for( int i = 0; i < m_stringSize; i++ )
		{
			m_stringBuffer[i] = 'a';
		}*/
		m_stringBuffer[m_stringSize] = '\0';

		// Since we duplicated the string buffer, we have to delete.
		m_shouldDelete = true;
	}

	String String::operator+(const String& otherString)
	{
		return String();
	}

	String String::operator=(const String& otherString)
	{
		copyCString(otherString);

		return *this;
	}

	bool String::operator==(const String& otherString)
	{
		//bool isNullptr = otherString == nullptr;
		if (otherString.m_stringSize != this->m_stringSize)
		{
			return false;
		}
		if (otherString.m_stringBuffer == this->m_stringBuffer)
		{
			return true;
		}
		if (strcmp(this->m_stringBuffer, otherString.m_stringBuffer) == 0)
		{
			return true;
		}
		return false;
	}

	std::ostream& operator<<(std::ostream& ostream, String& string)
	{
		ostream << (const char*)string.m_stringBuffer;
		return ostream;
	}

	// CONSTRUCTOR METHODS.

	String::String()
	{
		m_stringBuffer = nullptr;
		m_shouldDelete = false;
		m_bufferSize = 0;
		m_stringSize = 0;
	}

	String::String(const String& string)
	{
		copyCString(string);
	}

	String::String(const std::string& string)
	{
		// Setting sizes for fast lookup times.
		m_bufferSize = string.length() + 1;
		m_stringSize = m_bufferSize - 1;

		// Allocating a new buffer to store the string's text.
		// Cannot trust the string's internal buffer, as then we are at mercy
		// of the string's lifetime.
		m_stringBuffer = new char[m_bufferSize];

		// Copying text to the buffer.
		memcpy((char*)m_stringBuffer, string.c_str(), m_stringSize);
		m_stringBuffer[m_stringSize] = '\0';

		// Adding a null terminator.
		//m_stringBuffer[m_stringSize] = '\0';

		// Marking it for deletion when this object's lifetime runs out.
		// Because we allocated a new char buffer.
		m_shouldDelete = true;
	}

	String::String(const char* stringBuffer)
	{
		setupCString(stringBuffer, strlen(stringBuffer) + 1);

		// Not our buffer.
		m_shouldDelete = false;
	}

	template<size_t size>
	String::String(const char(&stringBuffer)[size])
	{
		setupCString(stringBuffer, size);

		// Not our buffer, not our responsiblity.
		m_shouldDelete = false;
	}

	String::String(const char* stringBuffer, size_t bufferLength)
	{
		setupCString(stringBuffer, bufferLength);

		// Not our buffer.
		m_shouldDelete = false;
	}

	// The simple yet very crucial destructor.
	String::~String()
	{
		// Checking if the string marked itself for deletion.
		if (m_shouldDelete)
		{
			// Cleaning up the string's own mess.
			delete[] m_stringBuffer;
			m_deleted = true;
		}
	}
}
