
/*
 * jaula_name_duplicated.h : JSON Analysis User Library Acronym
 * Object property name repeated exception
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_name_duplicated.h $
 * $Id: jaula_name_duplicated.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

///////////////////////////////////////////////////////////////////////
//                                                                   //
// jaula_name_duplicated.h : JSON analysis for application Readers   //
//                           Object property name repeated exception //
//                                                                   //
// (c) 2007 Kombo Morongo <morongo666@gmail.com>                     //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#ifndef _JAULA_NAME_DUPLICATED_H_
#define _JAULA_NAME_DUPLICATED_H_

#include <jaula/jaula_exception.h>

/**
 * \addtogroup jaula_exc JAULA: Error handling
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Class for bad object property name exceptions
   *
   * \ingroup jaula_exc
   *
   * \par
   * This class defines the exceptions to be thrown in case a property name
   * has been used more than once in the same object.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Name_Duplicated : public Exception
  {                              // class Name_Duplicated
    public:

      /**
       * \brief Constructor
       *
       * \param name Invalid object property name (duplicated).
       *
       * \param action action being performed when the exception arose.
       *
       * \param origin Name of the method (and others methods that have called
       * this) by the time the exception arose.
       *
       * \par Description
       * This method construct a new exception instance with
       * JAULA::Name_DUPLICATED code.
       */
      Name_Duplicated(std::string const &name = ""
        , std::string const &action = ""
        , std::string const &origin = "");

      /**
       * \brief Destructor
       */
      virtual ~Name_Duplicated();

      /**
       * \brief Retrieves the name of the duplicated property
       */
      std::string const &getName(void) const;

      /**
       * \brief Retrieves the detailed description for the exception
       *
       * \par Description
       * This method creates a standard detailed description for the exception
       * based on the duplicated object property name.
       */
      virtual std::string const &getDetail(void) const;

      /**
       * \brief Establishes the name for the duplicated property
       *
       * \param name Name of the object property that launched the exception.
       */
      void setName(std::string const &name);

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
      Name_Duplicated &operator=(Name_Duplicated const &orig);

    private:

      /**
       * Container for the repeated name
       */
      std::string name_;

      /**
       * workspace where to generate class specific detail
       */
      std::string detail_;
  };                             // class Name_Duplicated

}                                // namespace JAULA
#endif

// EOF $Id: jaula_name_duplicated.h 45 2009-01-11 16:17:03Z morongo $
