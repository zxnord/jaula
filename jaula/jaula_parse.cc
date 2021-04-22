
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


//#ifdef HAVE_MEMORY
#include <memory>
//#endif

//#ifdef HAVE_SSTREAM
#include <sstream>
///#endif

#include <unordered_set>

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

  static const char DOUBLE_PRECISION_CHAR = '.';

  static const std::unordered_set<char> WHITESPACE_CHARS_ {
      ' ', '\t', '\n', '\r'
  };

  static const std::unordered_set<char> MAIN_OBJECT_CHARS_ {
      '{', '['
  };

  static const std::unordered_set<char> MAIN_NUMBER_CHARS_ {
      '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
  };

  static const std::unordered_set<char> EXTRA_NUMBER_CHARS_ {
      DOUBLE_PRECISION_CHAR, 'e', 'E', '+'
  };

  static const std::unordered_set<char> MAIN_BOOLEAN_CHARS_ {
      't', 'f'
  };

  inline void skipWhitespace(std::string::const_iterator &it) {
    if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
      skipWhitespace(++it);
    }
  }

  inline void GetUnicodeChar(unsigned int code, char chars[5]) {
    if (code <= 0x7F) {
        chars[0] = (code & 0x7F); chars[1] = '\0';
    } else if (code <= 0x7FF) {
        // one continuation byte
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xC0 | (code & 0x1F); chars[2] = '\0';
    } else if (code <= 0xFFFF) {
        // two continuation bytes
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xE0 | (code & 0xF); chars[3] = '\0';
    } else if (code <= 0x10FFFF) {
        // three continuation bytes
        chars[3] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xF0 | (code & 0x7); chars[4] = '\0';
    } else {
        // unicode replacement character
        chars[2] = 0xEF; chars[1] = 0xBF; chars[0] = 0xBD;
        chars[3] = '\0';
    }
  }

  inline void parseUnicodeCharacter(std::string::const_iterator &it, std::string &value) {
    static std::unordered_set<char> HEX_CHARS {
        'A', 'B', 'C', 'D', 'E', 'F'
    };

    std::string base = "0x";
    char uniVal[5];

    for(int i = 0; i < 4; ++i) {
        if( (*(it+i) == '\0') || ((MAIN_NUMBER_CHARS_.find(*(it+i)) == MAIN_NUMBER_CHARS_.end()) 
          && (HEX_CHARS.find(*(it+i)) == HEX_CHARS.end())) ) {
          throw Syntax_Error("Unexpected EOF found or invalid Unicode string", "error iterating string");
        }

        base += *(it+i);
    }

    GetUnicodeChar(std::stoi(base, 0, 16), uniVal);
    value += uniVal;
    it += 4;
  }

  inline void parseSpecialCharacters(std::string::const_iterator &it, std::string &value) {
    switch(*it) {
      case '\\':
        value.push_back('\\');
        break;

      case '"':
        value.push_back('"');
        break;

      case '/':
        value.push_back('/');
        break;

      case 'b':
        value.push_back('\b');
        break;

      case 'f':
        value.push_back('\f');
        break;

      case 'n':
        value.push_back('\n');
        break;

      case 'r':
        value.push_back('\r');
        break;

      case 't':
        value.push_back('\t');
        break;

      case 'u':
        parseUnicodeCharacter(++it, value);
        return;

      default:
        return;
    }
    it++;
  }

  inline Value *findNumericValue(std::string::const_iterator &it) {
    std::string rawValue;
    bool doubleNumber = false;

    while( (*it != ' ') && (*it != ',') ) {
      if( *it == 0 ) {
        throw Syntax_Error("Unexpected EOF found", "error iterating string");
      }
      
      if( (MAIN_NUMBER_CHARS_.find(*it) == MAIN_NUMBER_CHARS_.end()) && 
        (EXTRA_NUMBER_CHARS_.find(*it) == EXTRA_NUMBER_CHARS_.end()) ) {
        throw Syntax_Error("Invalid number", "error iterating string");
      }

      if( *it == '.' ) {
          doubleNumber = true;
      }

      rawValue.push_back(*(it++));
    }

    if( doubleNumber ) {
        std::unique_ptr<Value_Number> value(new Value_Number());

        value->set(std::stod(rawValue));

        return value.release();
    } else {
        std::unique_ptr<Value_Number_Int> value(new Value_Number_Int());

        value->set(std::stoi(rawValue));

        return value.release();
    }
  }

  inline Value *findBooleanValue(std::string::const_iterator &it) {
    std::unique_ptr<Value_Boolean> value(new Value_Boolean());
    std::string rawValue;

    while( (*it != ' ') && (*it != ',') ) {
      if( *it == 0 ) {
        throw Syntax_Error("Unexpected EOF found", "error iterating string");
      }

      rawValue.push_back(*(it++));
    }

    if( rawValue.compare("true") == 0 ) {
      value->set(true);
    } else if( rawValue.compare("false") == 0 ) {
      value->set(false);
    } else {
        throw Syntax_Error("Invalid Boolean string value", "error finding boolean value");
    }

    return value.release();
  }

  inline Value *findStringValue(std::string::const_iterator &it) {
    std::unique_ptr<Value_String> value(new Value_String());
    std::string rawValue;

    while( (*it != '"') && (*(it-1) != '\\') ) {
      if( *it == 0 ) {
        throw Syntax_Error("Unexpected EOF found", "error iterating string");
      }

      if( *it == '\\' ) {
        parseSpecialCharacters(++it, rawValue);
      } else {
        rawValue.push_back(*(it++));
      }
    }

    value->set(rawValue);

    return value.release();
  }

  inline Value *findNullValue(std::string::const_iterator &it) {
    std::unique_ptr<Value_Null> value;
    std::string rawValue;

    while( (*it != ' ') && (*it != ',') ) {
      if( *it == 0 ) {
        throw Syntax_Error("Unexpected EOF found", "error iterating string");
      }

      rawValue.push_back(*(it++));
    }

    if( rawValue.compare("null") == 0 ) {
      value.reset(new Value_Null());
    } else {
        throw Syntax_Error("Invalid Boolean string value", "error finding boolean value");
    }

    return value.release();
  }

  inline Value *findObjectValue(std::string::const_iterator &it, int &token) {
    if( MAIN_NUMBER_CHARS_.find(*it) != MAIN_NUMBER_CHARS_.end() ) {
      token = '1';
      return findNumericValue(it);
    } else if( MAIN_BOOLEAN_CHARS_.find(*it) != MAIN_BOOLEAN_CHARS_.end() ) {
      token = 'b';
      return findBooleanValue(it);
    } else if( *it == '[' ) {
      token = '[';
      return nullptr;
    } else if( *it == '{' ) {
      token = '{';
      return nullptr;
    } else if( *it == 'n' ) {
      token = 'n';
      return findNullValue(it);
    } else if( *it == '"' ) {
      token = 's';
      return findStringValue(++it);
    } else {
      throw Syntax_Error("json invalid property value", "error property_next state machine");
    }
  }

  Parser::Parser(void)
    {}

  Parser::~Parser(void)
    {}

/*  Value_Complex *Parser::parseStream(std::istream &inpStream
    , bool comments_allowed
    , bool full_read)
  {
    try
    {
      Lexan   lexer(inpStream, comments_allowed);

      unsigned int firstToken = lexer.yylex();
      std::unique_ptr<Value> pVal(Value_Parser::parseValue(lexer, firstToken));

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
*/

  Value_Complex *Parser::parseStream2(std::istream &inpStream)
  {
    std::string fullStr;

    inpStream.seekg(0, std::ios::end);
    fullStr.reserve(inpStream.tellg());
    inpStream.seekg(0, std::ios::beg);

    fullStr.assign((std::istreambuf_iterator<char>(inpStream)),
                    std::istreambuf_iterator<char>());

    return dynamic_cast<Value_Complex *>(Value_Parser::parseString(fullStr));
  }

  Value *Parser::Value_Parser::parseString(const std::string &input)
  {
    std::unique_ptr<Value_Array>  pArray;
    std::unique_ptr<Value_Object> pObject;
    std::string                   propName;

    auto it = input.cbegin();
    auto state = START;

    skipWhitespace(it);
    
    if((it == input.cend()) || (MAIN_OBJECT_CHARS_.find(*it) == MAIN_OBJECT_CHARS_.end()) ) {
      throw Syntax_Error("The first value taken from the input does not"
        " belong to a complex type (array or object)"
        , "analyzing input stream");
    }


    while(it != input.cend()) {
        if( *it == 0 ) {
          throw Syntax_Error("Unexpected EOF found", "error iterating string");
        }

       switch (state)
        {                        // main state switch

          case START :
          {
            int token = 0;
            std::unique_ptr<Value> pItemVal(findObjectValue(it, token));

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            switch(*it)
            {
              case '{':
                pObject.reset(new Value_Object());
                state = property_begin;
                it++;
                break;
              
              case '[':
                pArray.reset(new Value_Array());
                state = array_addItem;
                it++;
                break;

              case 's' :
                it++;
              case 'n' :
              case 'b' :
              case '1' :
                return pItemVal.release();

              default:
                /*std::ostringstream  errDet;
                errDet << "Unexpected symbol ";
                if (isprint(*it))
                  errDet << "'" << static_cast<char>(*it) << "'";
                else
                  errDet << '(' << *it << ')';
                errDet << " while waiting for a value or the beginning of an"
                  << " array or object";
                std::ostringstream  errAct;
                errAct << "analyzing line "
                  << lexan.lineno()
                  << " from input stream.";
                throw Syntax_Error(errDet.str(), errAct.str());*/
                throw Syntax_Error("json valid initializers { or [ not found", "error START state machine");
            }
            break;
          }

          case array_addItem :
          {
            int token = 0;

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            std::unique_ptr<Value> pItemVal(findObjectValue(it, token));

            switch (token)
            {                    // array_addItem state switch
              case 0 :
                continue;

              case ']' :
                return pArray.release();

              case '[' :
                {
                  int braceCount = 0;
                  std::unique_ptr<Value> pItemComplex(parseString(std::string(it, input.cend())));
                  pArray->addItem(*(pItemComplex.get()));

                  while( braceCount >= 0 ) {
                    if( *(++it) == '[' ) {
                      braceCount++;
                    } else if( *it == ']' ) {
                      braceCount--;
                    }
                  }
                  it++;
                }
                state = array_nextItem;
                break;

              case '{' :
                {
                  int braceCount = 0;
                  std::unique_ptr<Value> pItemComplex(parseString(std::string(it, input.cend())));
                  pArray->addItem(*(pItemComplex.get()));

                  while( braceCount >= 0 ) {
                    if( *(++it) == '{' ) {
                      braceCount++;
                    } else if( *it == '}' ) {
                      braceCount--;
                    }
                  }
                  it++;
                }
                state = array_nextItem;
                break;

              case 's' :
                it++;
              case 'n' :
              case 'b' :
              case '1' :
                pArray->addItem(*(pItemVal.get()));
                state = array_nextItem;
                break;

              default :
                throw Syntax_Error("json invalid array element value", "error array_addItem state machine");

            }                    // array_addItem state switch
            break;
          }

          case array_nextItem :

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            switch (*it)
            {                    // array_nextItem state switch
              case 0 :
                continue;

              case ']' :
                return pArray.release();

              case ',' :
                state = array_addItem;
                it++;
                break;

              default :
                throw Syntax_Error("json valid array value next , or end ] not found", "error array_nextItem state machine");


            }                    // array_nextItem state switch
            break;

          case property_begin :

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            switch (*it)
            {                    // property_begin state switch
              case 0:
                continue;

              case '}' :
                return pObject.release();

              case '"' :
                {
                  std::unique_ptr<Value> val(findStringValue(++it));
                  propName = static_cast<Value_String*>(val.get())->getData();
                  state = property_name;
                  it++;
                  break;
                }

              default :
                {
                  /*std::ostringstream  errDet;
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
                    << " from input stream."; */
                  throw Syntax_Error("json valid propName identifier \" or empty object } not found", "error property_begin state machine");
                }

            }                    // property_begin state switch
            break;

          case property_name :
            switch (*it)
            {                    // property_name state switch
              case 0 :
                continue;

              case ':' :
                state = property_value;
                it++;
                break;

              default :
                throw Syntax_Error("json valid property value separator : not found", "error property_name state machine");

            }                    // property_name state switch
            break;

          case property_value :
          {
            int token = 0;

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            std::unique_ptr<Value> pItemVal(findObjectValue(it, token));
            switch (token)
            {                    // property_value state switch
              case 0 :
                continue;

              case '[' :
                {
                  int braceCount = 0;
                  std::unique_ptr<Value> pItemComplex(parseString(std::string(it, input.cend())));
                  pObject->insertItem(propName, *(pItemComplex.get()));

                  while( braceCount >= 0 ) {
                    if( *(++it) == '[' ) {
                      braceCount++;
                    } else if( *it == ']' ) {
                      braceCount--;
                    }
                  }
                  it++;
                }
                state = property_next;
                break;

              case '{' :
                {
                  int braceCount = 0;
                  std::unique_ptr<Value> pItemComplex(parseString(std::string(it, input.cend())));
                  pObject->insertItem(propName, *(pItemComplex.get()));

                  while( braceCount >= 0 ) {
                    if( *(++it) == '{' ) {
                      braceCount++;
                    } else if( *it == '}' ) {
                      braceCount--;
                    }
                  }
                  it++;
                }
                state = property_next;
                break;

              case 's' :
                it++;
              case 'n' :
              case 'b' :
              case '1' :
                pObject->insertItem(propName, *(pItemVal.get()));
                state = property_next;
                break;

              default :
                throw Syntax_Error("json invalid property value", "error property_next state machine");

            }                    // property_value state switch
            break;
          }

          case property_next :

            if(WHITESPACE_CHARS_.find(*it) != WHITESPACE_CHARS_.end()) {
                skipWhitespace(it);
            }

            switch (*it)
            {                    // property_next state switch
              case 0 :
                continue;

              case '}' :
                return pObject.release();

              case ',' :
                state = property_begin;
                it++;
                break;

              default :
                throw Syntax_Error("json valid property value next , or end } not found", "error property_next state machine");

            }                    // property_next state switch
            break;

          default:
            throw Syntax_Error("state machine invalid status", "internal error");

          break;

        }                        // main state switch
    }

    return nullptr;
  }

  Parser::Value_Parser::Value_Parser(void)
    {}

  Parser::Value_Parser::~Value_Parser(void)
    {}

/*  Value *Parser::Value_Parser::parseValue(Lexan &lexan, unsigned int token)
  {
    try
    {
      std::unique_ptr<Value_Array>  pArray;
      std::unique_ptr<Value_Object> pObject;
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
                std::unique_ptr<Value>    pItem(parseValue(lexan, token));
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
                std::unique_ptr<Value> pVal(parseValue(lexan, token));
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
                std::unique_ptr<Value>    pItemVal(parseValue(lexan, token));
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
*/

/*  void Parser::Value_Parser::EOFError(Lexan &lexan, Syntax_Error const &ex)
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
*/

}                                // namespace JAULA


// EOF $Id: jaula_parse.cc 45 2009-01-11 16:17:03Z morongo $
