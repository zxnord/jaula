
/*
 * jaula_lexan.h : JSON Analysis User Library Acronym Lexical analysis
 * definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_lexan.h $
 * $Id: jaula_lexan.h 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

#ifndef _JAULA_LEXAN_H_
#define _JAULA_LEXAN_H_

/**
 * \addtogroup jaula_lex JAULA: JSON lexical analysis
 */

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/**
 * \brief Lexical Analysis Value Tokens.
 *
 * \par Description
 * Tokens are definde into the symbol table, so that GDB and other debuggers
 * know about them.
 */
enum yytokentype
{
  NULL_VALUE = 258
  , FALSE_VALUE
  , TRUE_VALUE
  , NUMBER_VALUE
  , NUMBER_INT_VALUE
  , STRING_VALUE
};
#endif
#define NULL_VALUE 258
#define FALSE_VALUE 259
#define TRUE_VALUE 260
#define NUMBER_VALUE 261
#define NUMBER_INT_VALUE 262
#define STRING_VALUE 263

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer jaulaFlexLexer
#include <FlexLexer.h>
#endif

#include <jaula/jaula_lexan_error.h>

namespace JAULA
{                                // namespace JAULA
  /**
   * \brief Lexical Analysis implementation
   *
   * \ingroup jaula_lex
   *
   * \par
   * This class implements the lexical analysis for JSON as specified by
   * RFC 4627.
   *
   * \author Kombo Morongo <morongo666@gmail.com>
   */
  class Lexan : public ::jaulaFlexLexer
  {                              // class Lexan
    public:

      /**
       * \brief Constructor
       *
       * \param in_stream stream for the input data to analyze
       *
       * \param comments_allowed flag to extend basic format and allow for
       * hash symbol '#' starting comments in input.
       *
       * \par Description
       * Creates a lexical analysis instance for the specified input stream.
       */
      Lexan(std::istream &in_stream, bool comments_allowed = false);

      /**
       * \brief Destructor
       */
      virtual ~Lexan();

      /**
       * \brief Retrieves tokens from the input
       *
       * \returns the code for a token read or 0 if the end of data has been
       * reached.
       */
      virtual int yylex();

      /**
       * \brief Error report
       *
       * \param detail Text for the error received
       *
       * \par Description
       * This method is a callback used by the analysis routines to indicate
       * an error condition.
       *
       * \par
       * Its current implementation consists of generating the exception
       * instance to be retrieved by getErrorReport().
       */
      virtual void LexerError(const char *detail);

      /**
       * \brief Retrieves last token associated data
       *
       * \returns the data associated for the last token returned from yylex()
       * in case there is such kind of data (token corresponds to a property
       * name or a single value) or undefined otherwise.
       */
      std::string const &getTokenData(void) const;

      /**
       * \brief Retrieves details for the last error detected
       *
       * \returns a pointer to an instance containing the details for the last
       * error detected during the lexical analysis process or a null pointer
       * in case no error have ocurred during the lexical analysis so far.
       */
      Lexan_Error const *getErrorReport(void) const;

    private:

      /**
       * \brief Flag for extending language to accept # comments
       */
      bool    commented;

      /**
       * \brief Container for the token associated data
       */
      std::string tokenData;

      /**
       * \brief Pointer to the last exception detected
       */
      Lexan_Error   *pErrorReport;

  };                             // class Lexan
}                                // namespace JAULA
#endif

// EOF $Id: jaula_lexan.h 45 2009-01-11 16:17:03Z morongo $
