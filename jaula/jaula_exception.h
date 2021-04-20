
/*
 *  jaula_exception.h : JSON Analysis User Library Acronym
 *  Error handling definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_exception.h $
 * $Id: jaula_exception.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_EXCEPTION_H_
#define _JAULA_EXCEPTION_H_

#include <ostream>
#include <string>

/**
 * \addtogroup jaula_exc JAULA: Error handling
 */

namespace JAULA
{                                // namespace JAULA

  /**
   * \brief Base class for error handling exceptions
   *
   * \ingroup jaula_exc
   *
   * \par
   * This class is the base for all the exceptions thrown by the methods
   * defined in this library.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Exception
  {                              // class Exception
    public:

      /**
       * \brief Enumeration of available error codes
       */
      enum    ExCode
      {                          // enum ExCode
        /** No Error detected */
        NO_ERROR
        /** The data provided for a method is from a wrong type */
        ,   BAD_DATA_TYPE
        /** Name repeated for an object property */
        ,   NAME_DUPLICATED
        /** Error detected in the lexical analysis phase */
        ,   LEXAN_ERROR
        /** Error detected in the syntactic analysis phase */
        ,   SYNTAX_ERROR
      };                         // enum ExCode

      /**
       * \brief Constructor
       *
       * \param code Code for the exception as defined in ExCode
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
       * This method construct a new exception instance detailing each
       * available property.
       */
      Exception(ExCode code = NO_ERROR
        , std::string const &detail = ""
        , std::string const &action = ""
        , std::string const &origin = "");

      /**
       * \brief Copy constructor
       *
       * \param orig Original instance to copy
       *
       * \par Description
       * Creates a new instance copying the contents from another
       */
      Exception(Exception const &orig);

      /**
       * \brief Destructor
       */
      virtual ~Exception();

      /**
       * \brief Retrieves the error code for the exception
       */
      ExCode getCode(void) const;

      /**
       * \brief Retrieves the detailed description for the exception
       *
       * \note
       * This method has been declared as virtual so it can be overrwritten
       * to generate automatic detailed descriptions for specific (derived)
       * exception types.
       */
      virtual std::string const &getDetail(void) const;

      /**
       * \brief Retrieves the action that caused the exception
       */
      std::string const &getAction(void) const;

      /**
       * \brief Retrieves the method being run when the exception arose
       *
       * \par Description
       * Retrieves the method being run when the exception arose and the
       * methods that called it if available.
       */
      std::string const &getOrigin(void) const;

      /**
       * \brief Represents the instance in a stream
       *
       * \param ostr Stream where the instance is to be displayed.
       *
       * \par Description
       * Puts a text representation of the instance contents in a stream.
       */
      void display(std::ostream &ostr) const;

      /**
       * \brief Establishes the description for the exception
       *
       * \param detail detailed description about the exception which made
       * execution fail.
       */
      void setDetail(std::string const &detail);

      /**
       * \brief Establishes the action that caused the exception
       *
       * \param action action being performed when the exception arose.
       */
      void setAction(std::string const &action);

      /**
       * \brief Establishes the method being run when the exception arose
       *
       * \param origin Name of the method (and others methods that have called
       * this) by the time the exception arose.
       */
      void setOrigin(std::string const &origin);

      /**
       * \brief Attachs a new origin after the existing ones
       *
       * \param origin Name of the method to add
       *
       * \par Description
       * Adds the name of a method who is supposed to be the next method in
       * the calling stack from where the exception occured so a basic trace
       * can be performed.
       */
      void addOrigin(std::string const &origin);

      /**
       * \brief Assignment operator
       *
       * \param orig Original instance to copy
       *
       * \returns a reference to the destination instance
       *
       * \par Description
       * Copies the contents of the original instance in the destination.
       *
       * \note
       * This method controls if destination and origin instances are the
       * same so there is no trouble in a = a asignments.
       */
      Exception &operator=(Exception const &orig);

    protected:

      /**
       * \brief Establishes the error code for the exception
       *
       * \param code Code for the exception as defined in ExCode
       */
      void setCode(ExCode code);

    private:

      /**
       * \brief Container for error code
       */
      ExCode  code_;

      /**
       * \brief Container for exception textual detail
       */
      std::string detail_;

      /**
       * \brief Container action causing the exception
       */
      std::string action_;

      /**
       * \brief Container for where the exception was detected
       */
      std::string origin_;
  };                             // class Exception

}                                // namespace JAULA


/**
 * \brief Insertion operator extension for Exceptions
 *
 * \ingroup jaula_exc
 *
 * \param ostr Stream where the instance is to be displayed.
 *
 * \param ex Instance to display
 *
 * \returns a reference to the stream
 *
 * \par Description
 * This method extends the standard insertion operation for streams to invoke
 * JAULA::display() through this alternative sintax.
 *
 * \author Kombo Morongo <morongo666@gmail.com>
 */
std::ostream &operator<<(std::ostream &ostr, JAULA::Exception const &ex);
#endif

// EOF $Id: jaula_exception.h 45 2009-01-11 16:17:03Z morongo $
