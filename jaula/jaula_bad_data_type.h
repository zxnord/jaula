
/*
 * jaula_bad_data_type.h : JSON Analysis User Library Acronym
 * Bad data type exception definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_bad_data_type.h $
 * $Id: jaula_bad_data_type.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_BAD_DATA_TYPE_H_
#define _JAULA_BAD_DATA_TYPE_H_

#include <jaula/jaula_exception.h>

/**
 * \addtogroup jaula_exc JAULA: Error handling
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief class for incompatible data type exceptions
   *
   * \ingroup jaula_exc
   *
   * \par
   * This class defines the exceptions to be thrown in case of incompatible
   * data types usage.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Bad_Data_Type : public Exception
  {                              // class Bad_Data_Type
    public:

      /**
       * \brief Constructor
       *
       * \param detail detailed description about the exception which made
       * execution fail.
       *
       * \param action action being performed when the exception arose.
       *
       * \param origin Name of the method (and others methods that have
       * called this) by the time the exception arose.
       *
       * \par Description
       * This method construct a new exception instance with
       * JAULA::BAD_DATA_TYPE code.
       */
      Bad_Data_Type(std::string const &detail = ""
        , std::string const &action = ""
        , std::string const &origin = "");

      /**
       * \brief Destructor
       */
      virtual ~Bad_Data_Type();

      /**
       * \brief Assignment operator
       *
       * \param orig Original instance to copy
       *
       * \returns a reference to the destination instance
       *
       * \par Description
       * Copies the contents of the original instance to destination.
       *
       * \note
       * This method has been redefined from the base class preventing
       * instance type modification.
       */
      Bad_Data_Type &operator=(Bad_Data_Type const &orig);

  };                             // class Bad_Data_Type

}                                // namespace JAULA
#endif

// EOF $Id: jaula_bad_data_type.h 45 2009-01-11 16:17:03Z morongo $
