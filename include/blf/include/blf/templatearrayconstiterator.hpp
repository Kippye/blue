#pragma once

#include "templateobject.hpp"

namespace blf
{
    class TemplateArrayConstIterator
    {
        private:
            TemplateObject** m_pointer;

        public:
            TemplateArrayConstIterator(TemplateObject** pointer) : m_pointer(pointer) {};

            TemplateArrayConstIterator& 			operator++();
            TemplateArrayConstIterator 			    operator++(int);

            TemplateArrayConstIterator& 			operator--();
            TemplateArrayConstIterator 			    operator--(int);

            TemplateObject*	                        operator[](int index);
            TemplateObject**	                    operator->();
            TemplateObject*	                        operator*();

            bool 					                operator==(const TemplateArrayConstIterator& other) const;
            bool 					                operator!=(const TemplateArrayConstIterator& other) const;
    };
}