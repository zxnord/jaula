
/*
 * jaula_value_number_int.h : JSON Analysis User Library Acronym
 * JSON numeric values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_number_int.h $
 * $Id: jaula_value_number_int.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_VALUE_NUMBER_INT_H_
#define _JAULA_VALUE_NUMBER_INT_H_

#include <jaula/jaula_value.h>

/**
 * \addtogroup jaula_val JAULA: JSON Values containers
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Class for handling numeric (int) values
   *
   * \ingroup jaula_val
   *
   * \par
   * This class is a container for JSON numeric values adapted to hold only
   * integer values.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Value_Number_Int : public Value
  {                              // class Value_Number_Int
    public:

      /**
       * \brief Constructor
       *
       * \param data Initial value to be hold by the conatiner
       */
      Value_Number_Int(long data = 0);

      /**
       * \brief Destructor
       */
      virtual ~Value_Number_Int();

      /**
       * \brief Retrieves the value contained by the instance
       */
      long getData(void) const;

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
      void set(long data);

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

    private:

      /**
       * \brief Container to hold the value itself
       */
      long    data_;
  };                             // class Value_Number_Int

}                                // namespace JAULA
#endif

// EOF $Id: jaula_value_number_int.h 45 2009-01-11 16:17:03Z morongo $
