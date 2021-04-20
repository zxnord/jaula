
/*
 * jaula_no_error.h : JSON Analysis User Library Acronym
 * No Error exception definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_no_error.h $
 * $Id: jaula_no_error.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_NO_ERROR_H_
#define _JAULA_NO_ERROR_H_

#include <jaula/jaula_exception.h>

/**
 * \addtogroup jaula_exc JAULA: Error handling
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief class for no error condition exceptions
   *
   * \ingroup jaula_exc
   *
   * \par
   * This class defines a null exception with no error.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class No_Error : public Exception
  {                              // class No_Error
    public:

      /**
       * \brief Constructor
       *
       * \par Description
       * This method construct a new exception instance with JAULA::NO_ERROR
       * code.
       */
      No_Error(void);

      /**
       * \brief Destructor
       */
      virtual ~No_Error();

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
      No_Error &operator=(No_Error const &orig);

  };                             // class No_Error

}                                // namespace JAULA
#endif

// EOF $Id: jaula_no_error.h 45 2009-01-11 16:17:03Z morongo $
