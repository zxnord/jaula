
/*
 * jaula_value_object.h : JSON Analysis User Library Acronym
 * JSON object values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_object.h $
 * $Id: jaula_value_object.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_OBJECT_H_
#define _JAULA_VALUE_OBJECT_H_

#include <map>

#include <jaula/jaula_name_duplicated.h>
#include <jaula/jaula_value_complex.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Class for handling object values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is a container for JSON objects
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Object : public Value_Complex
  {                              // class Value_Object
    public:

      /**
       * \brief Data type for value contents
       *
       * \par Description
       * Definition for the data container internal structure.
       *
       * \par
       * This data type defines a map of pointers to any kind of values
       * (including objects) indexed by a property name that is always a
       * string.
       */
      typedef std::map<std::string, Value *>  dataType;

      /**
       * \brief Default Constructor
       *
       * \par Description
       * The default constructor initializes an empty object.
       */
      Value_Object(void);

      /**
       * \brief Data Constructor
       *
       * \param data Reference to the data to be copied
       *
       * \par Description
       * This constructor generates a new instance by making a deep copy of
       * the original data.
       */
      Value_Object(dataType const &data);

      /**
       * \brief Destructor
       *
       * \note
       * The destruction process releases all the memory associated to the
       * data structure so any reference to the object or any element in it
       * will be void.
       */
      virtual ~Value_Object();

      /**
       * \brief Retrieves the map of values contained by the instance
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
       * \returns true if there are no single elements contained in the object
       * and false otherwise.
       */
      virtual bool empty(void) const;

      /**
       * \brief Number of elements contained
       *
       * \returns the number of single elements contained by the object.
       */
      virtual size_t size(void) const;

      /**
       * \brief Establishes the contents of the instance
       *
       * \param data map of values to assign to the instance
       *
       * \note
       * The destination object is destroyed during the asignment process so,
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
       * The destination object is destroyed during the asignment process so,
       * any references to it or to its former elements will be void.
       */
      virtual void set(Value const &origin);

      /**
       * \brief Inserts one item to the object
       *
       * \param name Name for the property to insert
       *
       * \param item Item value to be inserted
       *
       * \exception Name_Duplicated
       * This exception is thrown in case the object already has a property
       * with the same name as the one to insert.
       *
       * \par Description
       * Inserts a deep copy of the item value at the specified name fot the
       * object.
       */
      void insertItem(std::string const &name, Value const &item);

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
  };                             // class Value_Object

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_object.h 45 2009-01-11 16:17:03Z morongo $
