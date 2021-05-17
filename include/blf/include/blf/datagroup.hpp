#pragma once

#include "blf/templateobject.hpp"

#include <vector>
#include <stdexcept>

namespace blf
{
    template<class DataGroup>
    class DataGroupIterator
    {
        public: 
            using IndexType = typename DataGroup::IndexType;
            using ValueType = typename DataGroup::ValueType;
            using PointerType = ValueType*;
            using ReferenceType = ValueType&;
            using IndexArrayReference = typename DataGroup::IndicesArrayType&;

        private:
            IndexType   m_index;
            IndexType   m_maxSize;
            IndexArrayReference m_indices;
            TemplateObject** m_dataArray;
        
        public:
            DataGroupIterator(IndexType index, IndexType maxSize, IndexArrayReference indices, TemplateObject** dataArray) : m_index(index), m_maxSize(maxSize), m_indices(indices), m_dataArray(dataArray) { };
            
            DataGroupIterator& operator++()
            {
                m_index++;
                return *this;
            }
            
            DataGroupIterator operator++(int)
            {
                DataGroupIterator iterator = *this;
                ++(*this);
                return iterator;
            }
            
            DataGroupIterator& operator--()
            {
                m_index--;
                return *this;
            }
            
            DataGroupIterator operator--(int)
            {
                DataGroupIterator iterator = *this;
                --(*this);
                return iterator;
            }
            
            ReferenceType operator[](int index)
            {
                return m_dataArray[m_indices[index]];
            }
            
            PointerType operator->()
            {
                return &m_dataArray[m_indices[m_index]];
            }
            
            ValueType operator*()
            {
                return (ValueType)(m_dataArray[m_indices[m_index]]);
            }
            
            bool operator==(const DataGroupIterator& other) const
            {
                return m_index == other.m_index;
            }
            
            bool operator!=(const DataGroupIterator& other) const
            {
                return !(*this == other);
            }
    };
    
    template<class T, class Index=int, class IndicesArray=std::vector<Index>>
    class DataGroup
    {
        public:
            using ValueType         = T*;
            using IndexType         = Index;
            using IndicesArrayType  = IndicesArray;
            using Iterator          = DataGroupIterator<DataGroup<T, Index, IndicesArray>>;
            
        private:
            IndicesArrayType  m_indices;
            TemplateObject**  m_dataArray;

            IndexType         m_size;
            
        public:
            DataGroup(TemplateObject** const dataArray)
            {
                m_dataArray = dataArray;
                m_size = 0;
            }
            
            void addIndex(IndexType index)
            {
                m_indices.push_back(index);
                m_size++;
            }
            
            IndexType getSize()
            {
                return m_size;
            }
            
            T* operator[](const IndexType index)
            {
                return (T*)(m_dataArray[m_indices[index]]);
            }
            
            Iterator begin() 
            { 
                return Iterator(0, m_size, m_indices, m_dataArray); 
            }
            
            Iterator end()
            {
                return Iterator(m_size, m_size, m_indices, m_dataArray);
            }
    };
}