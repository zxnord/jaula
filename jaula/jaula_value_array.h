
/*
 * jaula_value_array.h : JSON Analysis User Library Acronym
 * JSON array values definitions
 *
 * Copyright (C) 2007, 2008, 2009 Kombo Morongo <morongo666@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * svn info:
 * $Author: morongo $
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_array.h $
 * $Id: jaula_value_array.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_ARRAY_H_
#define _JAULA_VALUE_ARRAY_H_

#include <list>

#include <jaula/jaula_value_complex.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Class for handling array values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is a container for JSON arrays
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Array : public Value_Complex
  {                              // class Value_Array
    public:

      /**
       * \brief Data type for value contents
       *
       * \par Description
       * Definition for the data container internal structure.
       *
       * This data type defines a list of pointers to any kind of values
       * (including arrays).
       */
      typedef std::list<Value *>  dataType;

      /**
       * \brief Default Constructor
       *
       * \par Description
       * The default constructor initializes an empty array.
       */
      Value_Array(void);

      /**
       * \brief Data Constructor
       *
       * \param data Reference to the data to be copied
       *
       * \par Description
       * This constructor generates a new instance by making a deep copy of
       * the original data.
       */
      Value_Array(dataType const &data);

      /**
       * \brief Destructor
       *
       * \note
       * The destruction process releases all the memory associated to the
       * data structure so any reference to the array or any element in it
       * will be void.
       */
      virtual ~Value_Array();

      /**
       * \brief Retrieves the array of values contained by the instance
       */
      dataType const &getData(void) const;

      /**
       * \brief Represents the instance in a stream
       *
       * \param ostr Stream where the instance is to be represented.
       *
       * \par Description
       * writes the instance content in JSON notation in a stream.
       */
      virtual void repr(std::ostream &ostr) const;

      /**
       * \brief True if the instance is empty.
       *
       * \returns true if there are no elements contained in the array and
       * false otherwise.
       */
      virtual bool empty(void) const;

      /**
       * \brief Number of elements contained
       *
       * \returns the number of elements contained in the array.
       */
      virtual size_t size(void) const;

      /**
       * \brief Establishes the contents of the instance
       *
       * \param data Array to assign to the instance
       *
       * \note
       * The destination array is destroyed during the asignment process so,
       * any references to it or to its former elements will be void.
       */
      void set(dataType const &data);

      /**
       * \brief Copies the contents of one instance into another
       *
       * \param origin Reference to the value to be copied.
       *
       * \exception Bad_Data_Type
       * This exception is launched in case that origin and destination value
       * types are different.
       *
       * \note
       * The destination array is destroyed during the asignment process so,
       * any references to it or to its former elements will be void.
       */
      virtual void set(Value const &origin);

      /**
       * \brief Appends one item to the array
       *
       * \param item Item to be appended
       *
       * \par Description
       * Appends a deep copy of the item value at the end of the array.
       */
      void addItem(Value const &item);

      /**
       * \brief Empties the contents of an instance
       *
       * \par Description
       * Erases the contained array.
       *
       * \note
       * As the array is destroyed by the process, any references to it or to
       * any of its elements will be void.
       */
      virtual void clear(void);

    private:

      /**
       * \brief Container to hold the value itself
       */
      dataType    data_;
  };                             // class Value_Array

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_array.h 45 2009-01-11 16:17:03Z morongo $
