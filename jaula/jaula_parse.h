
/*
 * jaula_parse.h : JSON Analysis User Library Acronym
 * JSON data parser
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_parse.h $
 * $Id: jaula_parse.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_PARSE_H_
#define _JAULA_PARSE_H_

/**
 * \addtogroup jaula_parse JAULA: JSON data parser
 */

//#include <jaula/jaula_lexan.h>
#include <jaula/jaula_syntax_error.h>
#include <jaula/jaula_value_complex.h>

namespace JAULA
{                                // namespace JAULA
  /**
   * \brief JSON Data Parser
   *
   * \ingroup jaula_parse
   *
   * \par
   * This class implements the JSON parser itself as specified by RFC 4627.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Parser
  {                              // class Parser
    public:

      /**
       * \brief Constructor
       */
      Parser(void);

      /**
       * \brief Destructor
       */
      ~Parser(void);

      /**
       * \brief Parses JSON data from a stream
       *
       * \param inpStream stream from where to read the data to parse.
       *
       * \param comments_allowed flag that if it is true means that the input
       * can contain comments that begin with the hash '#' symbol and ends
       * with eoln (as in bash).
       * If it is false, it means that no comments are allowed in the input
       * and if present will be considered as a syntax error.
       *
       * \param full_read flag that if it is true means that the parser must
       * analyze the input stream until the end of file is although it already
       * had got a full array or object from it.
       * In this situation, as JSON specification expects just only one array
       * or object per input, any further data that is not a space for the
       * syntax (or a comment if comments are allowed) will launch a syntax
       * error exception.
       *
       * \par
       * If this flag is false, the parser will stop once a full array or
       * object is taken from the input and, on exit, the stream will point to
       * the start of the remaining data.
       *
       * \returns a pointer to memory taken from the heap containing a complex
       * value (array or object) with all the data from the stream parsed on
       * individual / nested items.
       *
       * \exception Exception
       * An exception will be thrown as soon as a lexical or syntax error is
       * found analyzing the stream.
       * The result of printing the exception through a stream is a human
       * readable text explaining the error found and an approximation of the
       * error line where occurred.
       *
       * \par Description
       * This method is the entry point for the JSON parser.
       *
       * \warning
       * As this method returns a pointer to memory from the heap, it is up to
       * the user to free it when it is no longer needed in order to avoid
       * leaks.
       */
      //static Value_Complex *parseStream(std::istream &inpStream
      //  , bool comments_allowed = false
      //  , bool full_read = true
      //  );

      static Value_Complex *parseStream2(std::istream &inpStream);



    private:

      /**
       * \brief JSON Value Parser
       *
       * \ingroup jaula_parse
       *
       * \par
       * Parses a value from the input stream.
       *
       * \author Kombo Morongo <morongo666@gmail.com>
       */
      class Value_Parser
      {                          // class Value_Parser
        public:

          /**
           * \brief Constructor
           */
          Value_Parser(void);

          /**
           * \brief Destructor
           */
          ~Value_Parser(void);

          /**
           * \brief reads a single JSON value
           *
           * \param lexan Reference to the lexical analysis instance that
           * reads from the input.
           *
           * \param token Token read from the upper level.
           * * If this token does not belong to an starting value token, an
           * error condition will arise.
           *
           * \returns a pointer to memory taken from the heap containing the
           * value read.
           * If this value belongs to a complex type, all the items that it
           * contains have been recursively parsed.
           *
           * \exception Exception
           * An exception will be thrown as soon as a lexical or syntax error
           * is found analyzing the stream. The result of printing the
           * exception retrurned through a stream is a human readable text
           * explaining the error found and an approximation of the error line
           * where it occurred.
           *
           * \par Description
           * This method fully reads a whole value from the input or until a
           * syntax or lexical error is found. Upon execution input stream is
           * positioned so that a new token or EOF can be read from the input.
           */
          //static Value *parseValue(Lexan &lexan, unsigned int token);

          static Value *parseString(const std::string &input);

        private:

          /**
           * \brief Enumeration for the parser state machine
           *
           * \par Description
           * This type defines a constant for each state that the parser may
           * assume during the process.
           */
          enum parser_states
          {
            /** initial state */
            START,
            /** a new item for an array has been read */
            array_addItem,
            /** an inter-item delimiter for an array has been read */
            array_nextItem,
            /** error condition detected
             * (pseudostate that launches an exception terminating the
             * process)
             */
            error,
            /** a boolean false value constant has been read (pseudostate) */
            false_value,
            /** a null value constant has been read (pseudostate) */
            null_value,
            /** a numeric (integer) value has been read (pseudostate) */
            number_int_value,
            /** a numeric (float) value has been read (pseudostate) */
            number_value,
            /** an object initial delimiter has been read */
            property_begin,
            /** an object property name has been read */
            property_name,
            /** an object property delimiter has been read */
            property_value,
            /** an inter-property delimiter for an object has been read */
            property_next,
            /** a string value has been read (pseudostate) */
            string_value,
            /** a boolean true value constant has been read (pseudostate) */
            true_value,
            /** final state (pseudostate) */
            END
          };                     //enum parser_states

          /**
           * \brief Analyzes the reason for an EOF condition
           *
           * \param lexan Reference to the lexical analysis instance that
           * reads from the input.
           *
           * \param ex Syntax error to be thrown detailing why an EOF at this
           * point is an error.
           *
           * \exception Exception
           * As a result of the execution of this method an exception is
           * thrown with the data conatined in the input parameter or with a
           * JAULA::LEXAN_ERROR type if the EOF is due to an error in the
           * lexical analysis phase.
           *
           * \par Description
           * This method is to be launched whenever an unexpected end of file
           * is encountered.
           * Its implementation includes analyzing if the EOF condition is
           * real or a side effect from a lexical analysis error and chooses
           * to send the Lexan::LexerError Exception or the one received in
           * the input parameter based on this.
           */
//          static void EOFError(Lexan &lexan, Syntax_Error const &ex);
      };                         // class Value_Parser

  };                             // class Parser

}                                // namespace JAULA
#endif

// EOF $Id: jaula_parse.h 45 2009-01-11 16:17:03Z morongo $
