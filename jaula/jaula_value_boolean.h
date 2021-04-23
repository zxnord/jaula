
/*
 * jaula_value_boolean.h : JSON Analysis User Library Acronym
 * JSON boolean values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_boolean.h $
 * $Id: jaula_value_boolean.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_BOOLEAN_H_
#define _JAULA_VALUE_BOOLEAN_H_

#include <jaula/jaula_value.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Class for handling boolean values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is a container for JSON boolean values
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Boolean : public Value
  {                              // class Value_Boolean
    public:

      /**
       * \brief Constructor
       *
       * \param data Initial value to be hold by the conatiner
       */
      Value_Boolean(bool data = false);

      /**
       * \brief Destructor
       */
      virtual ~Value_Boolean();

      /**
       * \brief Retrieves the value contained by the instance
       */
      bool getData(void) const;

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
       * \brief Establishes the contents of the instance
       *
       * \param data Value to assign to the instance
       */
      void set(bool data);

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

    private:

      /**
       * \brief Container to hold the value itself
       */
      bool    data_;
  };                             // class Value_Boolean

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_boolean.h 45 2009-01-11 16:17:03Z morongo $
