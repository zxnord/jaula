/*
 * jaula_value_null.h : JSON Analysis User Library Acronym
 * JSON null values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_null.h $
 * $Id: jaula_value_null.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_NULL_H_
#define _JAULA_VALUE_NULL_H_

#include <jaula/jaula_value.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief class for handling null values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is a container for JSON Null values
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Null : public Value
  {                              // class Value_Null
    public:

      /**
       * \brief Constructor
       */
      Value_Null(void);

      /**
       * \brief Destructor
       */
      virtual ~Value_Null();

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
       * \brief Copies the contents of one instance into another
       *
       * \param origin Reference to the value to be copied.
       *
       * \exception Bad_Data_Type
       * This exception is launched in case that origin and destination value
       * types are different.
       */
      virtual void set(Value const &origin) throw(Bad_Data_Type);

  };                             // class Value_Null

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_null.h 45 2009-01-11 16:17:03Z morongo $
