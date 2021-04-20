
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_parse.cc $
 * $Id: jaula_parse.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

}


#ifdef HAVE_MEMORY
#include <memory>
#endif

#ifdef HAVE_SSTREAM
#include <sstream>
#endif

#include <jaula/jaula_parse.h>
#include <jaula/jaula_syntax_error.h>
#include <jaula/jaula_value_array.h>
#include <jaula/jaula_value_boolean.h>
#include <jaula/jaula_value_null.h>
#include <jaula/jaula_value_number.h>
#include <jaula/jaula_value_number_int.h>
#include <jaula/jaula_value_object.h>
#include <jaula/jaula_value_string.h>

namespace JAULA
{                                // namespace JAULA
  Parser::Parser(void)
    {}

  Parser::~Parser(void)
    {}

  Value_Complex *Parser::parseStream(std::istream &inpStream
    , bool comments_allowed
    , bool full_read) throw(Exception)
  {
    try
    {
      Lexan   lexer(inpStream, comments_allowed);

      unsigned int firstToken = lexer.yylex();
      std::auto_ptr<Value> pVal(Value_Parser::parseValue(lexer, firstToken));

      if (!dynamic_cast<Value_Complex const *>(pVal.get()))
        throw Syntax_Error("The first value taken from the input does not"
          " belong to a complex type (array or object)"
          , "analyzing input stream");

      if (full_read)
      {
        if (lexer.yylex())
        {
          std::ostringstream  errAct;
          errAct << "analyzing line "
            << lexer.lineno()
            << " from input stream.";
          throw Syntax_Error("The input stream contains more than one first"
            " level complex (array or object) value"
            , errAct.str());
        }
      }

      return dynamic_cast<Value_Complex *>(pVal.release());
    }
    catch(Exception &ex)
    {
      ex.addOrigin("Parser::parseStream(std::istream *, bool, bool)");
      throw ex;
    }
  }

  Parser::Value_Parser::Value_Parser(void)
    {}

  Parser::Value_Parser::~Value_Parser(void)
    {}

  Value *Parser::Value_Parser::parseValue(Lexan &lexan, unsigned int token)
    throw(Exception)
  {
    try
    {
      std::auto_ptr<Value_Array>  pArray;
      std::auto_ptr<Value_Object> pObject;
      std::string                 propName;
      for (parser_states  state = START
        ; (state != END)
        ; token = lexan.yylex())
      {                          // main state for

        switch (state)
        {                        // main state switch

          case START :
            switch (token)
            {                    // START state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for a value"
                  , errAct.str()));
              }
              break;

              case '[' :
                pArray.reset(new Value_Array());
                state = array_addItem;
                break;

              case NULL_VALUE :
                return new Value_Null();

              case FALSE_VALUE :
                return new Value_Boolean(false);

              case TRUE_VALUE :
                return new Value_Boolean(true);

              case NUMBER_VALUE :
              {
                char *resto;
                double  val = strtod(lexan.getTokenData().c_str(), &resto);
                if ((resto) && (*resto))
                {
                  std::ostringstream  errDet;
                  errDet << "Value " << lexan.getTokenData()
                    << " contains a non-numerical value and is not enclosed"
                    << " between double quotes";
                  std::ostringstream  errAct;
                  errAct << "analyzing line "
                    << lexan.lineno()
                    << " from input stream.";
                  throw Syntax_Error(errDet.str(), errAct.str());
                }
                return new Value_Number(val);
              }

              case NUMBER_INT_VALUE :
              {
                char *resto;
                long  val = strtol(lexan.getTokenData().c_str(), &resto, 0);
                if ((resto) && (*resto))
                {
                  std::ostringstream  errDet;
                  errDet << "Value "
                    << lexan.getTokenData()
                    << " contains a non-numerical value and is not enclosed"
                    << " between double quotes";
                  std::ostringstream  errAct;
                  errAct << "analyzing line "
                    << lexan.lineno()
                    << " from input stream.";
                  throw Syntax_Error(errDet.str(), errAct.str());
                }
                return new Value_Number_Int(val);
              }

              case STRING_VALUE :
                return new Value_String(lexan.getTokenData());

              case '{' :
                pObject.reset(new Value_Object());
                state = property_begin;
                break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for a value or the beginning of an"
                  << " array or object";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // START state switch
            break;

          case array_addItem :
            switch (token)
            {                    // array_addItem state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for a value"
                  " or a closing array symbol"
                  , errAct.str()));
              }
              break;

              case ']' :
                return pArray.release();

              case '[' :
              case '{' :
              case NULL_VALUE :
              case FALSE_VALUE :
              case TRUE_VALUE :
              case NUMBER_VALUE :
              case NUMBER_INT_VALUE :
              case STRING_VALUE :
              {
                std::auto_ptr<Value>    pItem(parseValue(lexan, token));
                pArray->addItem(*(pItem.get()));
              }
              state = array_nextItem;
              break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for a value or the end of an array";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // array_addItem state switch
            break;

          case array_nextItem :
            switch (token)
            {                    // array_nextItem state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for an item"
                  " delimiter or a closing array symbol"
                  , errAct.str()));
              }
              break;

              case ']' :
                return pArray.release();

              case ',' :
                state = array_addItem;
                break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for an item delimiter or the end of"
                  << " an array";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // array_nextItem state switch
            break;

          case property_begin :
            switch (token)
            {                    // property_begin state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for a"
                  " property name or a closing object symbol"
                  , errAct.str()));
              }
              break;

              case '}' :
                return pObject.release();

              case STRING_VALUE :
              {
                std::auto_ptr<Value> pVal(parseValue(lexan, token));
                Value_String *pStrVal =
                  dynamic_cast<Value_String *>(pVal.get());
                if (pStrVal)
                  propName = pStrVal->getData();
                else
                {
                  std::ostringstream  errDet;
                  errDet << "Unexpected value type ";
                  if (isprint(token))
                    errDet << "'" << static_cast<char>(token) << "'";
                  else
                    errDet << '(' << token << ')';
                  errDet << " while waiting for a property name";
                  std::ostringstream  errAct;
                  errAct << "analyzing line "
                    << lexan.lineno()
                    << " from input stream.";
                  throw Syntax_Error(errDet.str(), errAct.str());
                }
              }
              state = property_name;
              break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for a property name or the end of"
                  << " an object";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // property_begin state switch
            break;

          case property_name :
            switch (token)
            {                    // property_name state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for a"
                  " property name delimiter symbol"
                  , errAct.str()));
              }
              break;

              case ':' :
                state = property_value;
                break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for property name delimiter";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // property_name state switch
            break;

          case property_value :
            switch (token)
            {                    // property_value state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for a value"
                  " for an object property"
                  , errAct.str()));
              }
              break;

              case '[' :
              case '{' :
              case NULL_VALUE :
              case FALSE_VALUE :
              case TRUE_VALUE :
              case NUMBER_VALUE :
              case NUMBER_INT_VALUE :
              case STRING_VALUE :
              {
                std::auto_ptr<Value>    pItemVal(parseValue(lexan, token));
                pObject->insertItem(propName, *(pItemVal.get()));
              }
              state = property_next;
              break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for a value for an object property";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // property_value state switch
            break;

          case property_next :
            switch (token)
            {                    // property_next state switch
              case 0 :
              {
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                EOFError(lexan
                  , Syntax_Error("Unexpected EOF while waiting for an item"
                  " delimiter or a closing object symbol"
                  , errAct.str()));
              }
              break;

              case '}' :
                return pObject.release();

              case ',' :
                state = property_begin;
                break;

              default :
              {
                std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(token))
                  errDet << "'" << static_cast<char>(token) << "'";
                else
                  errDet << '(' << token << ')';
                errDet << " while waiting for an item delimiter or the end of"
                  << " an object";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());
              }
              break;

            }                    // property_next state switch
            break;

          default:
          {
            std::ostringstream  errAct;
            errAct << "analyzing line "
              << lexan.lineno()
              << " from input stream.";
            throw Syntax_Error("The parser state machine reached an unknown"
              " value. Internal error"
              , errAct.str());
          }
          break;

        }                        // main state switch

      }                          // main state for
      {
        std::ostringstream  errAct;
        errAct << "analyzing line "
          << lexan.lineno()
          << " from input stream.";
        throw Syntax_Error("The parser state machine flow reached to an"
          " unexpected point. Internal error"
          , errAct.str());
      }
      return 0;                  // Flow control will never reach this point
    }
    catch(Exception &ex)
    {
      ex.addOrigin("Parser::Value_Parser::parseValue(Lexan &, unsigned int)");
      throw ex;
    }
  }

  void Parser::Value_Parser::EOFError(Lexan &lexan, Syntax_Error const &ex)
    throw(Exception)
  {
    try
    {
      if (lexan.getErrorReport())
        throw Lexan_Error(*(lexan.getErrorReport()));
      else
        throw Syntax_Error(ex);
    }
    catch(Exception &ex)
    {
      ex.addOrigin("Parser::Value_Parser::EOFError(Lexan &"
        ", Exception const &)");
      throw ex;
    }
  }

}                                // namespace JAULA


// EOF $Id: jaula_parse.cc 45 2009-01-11 16:17:03Z morongo $
