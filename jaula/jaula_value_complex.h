
/*
 * jaula_value_complex.h : JSON Analysis User Library Acronym
 * JSON complex values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_complex.h $
 * $Id: jaula_value_complex.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_COMPLEX_H_
#define _JAULA_VALUE_COMPLEX_H_

#include <jaula/jaula_value.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Base class for handling complex values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class defines an abstract base class for JSON complex values
   * (values which are an aggregation of other values)
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Complex : public Value
  {                              // class Value_Complex
    public:

      /**
       * \brief Destructor
       */
      virtual ~Value_Complex();

      /**
       * \brief True if the instance is empty.
       *
       * \returns true if there are no single elements contained by the
       * instance and false otherwise.
       */
      virtual bool empty(void) const = 0;

      /**
       * \brief Number of elements contained
       *
       * \returns the number of single elements contained by the instance.
       */
      virtual size_t size(void) const = 0;

      /**
       * \brief Empties the contents of an instance
       *
       * \par Description
       * Erases all the instance content.
       */
      virtual void clear(void) = 0;

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
      Value_Complex(ValueType Type);

  };                             // class Value_Complex

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_complex.h 45 2009-01-11 16:17:03Z morongo $
