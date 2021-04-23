
/*
 * jaula_value.h : JSON Analysis User Library Acronym
 * Generic values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value.h $
 * $Id: jaula_value.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_H_
#define _JAULA_VALUE_H_

#include <jaula/jaula_bad_data_type.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Base class for handling values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is the abstract base for all the containers for values
   * according to the JSON specification.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value
  {                              // class Value
    public:

      /**
       * \brief Enumeration of available value types
       */
      enum    ValueType
      {                          // enum ValueType
        /** JSON Null value */
        TYPE_NULL
        /** JSON Boolean value */
        ,   TYPE_BOOLEAN
        /** JSON String value */
        ,   TYPE_STRING
        /** JSON Number value */
        ,   TYPE_NUMBER
        /** JSON Number value adapted to hold integer quantities */
        ,   TYPE_NUMBER_INT
        /** JSON Array of values */
        ,   TYPE_ARRAY
        /** JSON Object */
        ,   TYPE_OBJECT
      };                         // enum ValueType

      /**
       * \brief Destructor
       */
      virtual ~Value();

      /**
       * \brief Retrieves the value type for the instance
       *
       * \note
       * Value types are immutable during instance's life cycle and can only
       * be specified at construction time.
       */
      ValueType getType(void) const;

      /**
       * \brief Represents the instance in a stream
       *
       * \param ostr Stream where the instance is to be represented.
       *
       * \par Description
       * writes the instance content in JSON notation in a stream.
       */
      virtual void repr(std::ostream &ostr) const = 0;

      /**
       * \brief Copies the contents of one instance into another
       *
       * \param origin Reference to the value to be copied.
       *
       * \exception Bad_Data_Type
       * This exception is launched in case that origin and destination value
       * types are different.
       */
      virtual void set(Value const &origin);

      /**
       * \brief Assignment operator
       *
       * \param orig Original instance to copy
       *
       * \returns a reference to the destination instance
       *
       * \exception Bad_Data_Type
       * This exception is launched in case that origin and destination value
       * types are different.
       *
       * \par Description
       * Copies the contents of the original instance in the destination.
       *
       * \note
       * This method controls if destination and origin instances are the same
       * so there is no trouble in a = a asignments.
       */
      Value &operator=(Value const &orig);

      /**
       * \brief Creates a duplicate of a value
       *
       * \param orig Original instance to duplicate.
       *
       * \returns a pointer to memory taken from the heap (by means of the new
       * operator) and containing a deep copy of the original value.
       *
       * \warning
       * As this method returns a pointer to memory allocated from the heap,
       * it is up to the caller to release once it is no longer needed in
       * order to avoid leaks.
       */
      static Value *duplicate(Value const &orig);

    protected:

      /**
       * \brief Constructor
       *
       * \param Type Type of value to be contained by the instance
       *
       * \par Description
       * This method construct a new instance by specifying its type.
       *
       * \note
       * A ValueType for the instance is immutable during all the life cycle,
       * this is the only method that permits specifying the value type.
       */
      Value(ValueType Type);

    private:

      /**
       * \brief Container for error code
       */
      ValueType  Type_;
  };                             // class Value

}                                // namespace JAULA


/**
 * \brief Insertion operator extension for values
 *
 * \ingroup jaula_val
 *
 * \param ostr Stream where the instance is to be represented.
 *
 * \param val Instance to represent
 *
 * \returns a reference to the stream
 *
 * \par Description
 * This method extends the standard insertion operation for streams to invoke
 * JAULA::repr() through this alternative sintax writing the contents in JSON
 * notation.
 *
 * \author Kombo Morongo <morongo666@gmail.com>
 */
std::ostream &operator<<(std::ostream &ostr, JAULA::Value const &val);
#endif

// EOF $Id: jaula_value.h 45 2009-01-11 16:17:03Z morongo $
