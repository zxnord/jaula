
/*
 * jaula_value_string.cc : JSON Analysis User Library Acronym
 *                         JSON string values definitions
 *
 * Copyright (C) 2007 Kombo Morongo <morongo666@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * svn info:
 * $Author: morongo $
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_string.cc $
 * $Id: jaula_value_string.cc 45 2009-01-11 16:17:03Z morongo $
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

}


//#ifdef HAVE_SSTREAM
#include <sstream>
//#endif

#include <jaula/jaula_value_string.h>

namespace JAULA
{                                // namespace JAULA

  Value_String::Value_String(std::string const &data)
    : Value(TYPE_STRING)
    , data_(data)
    {}

  Value_String::~Value_String()
    {}

  std::string const &Value_String::getData(void) const
  {
    return data_;
  }

  void Value_String::repr(std::ostream &ostr) const
  {
    stringRepr(ostr, data_);
  }

  void Value_String::set(std::string const &data)
  {
    data_ = data;
  }

  void Value_String::set(Value const &origin)
  {
    try
    {
      Value::set(origin);
      Value_String const *pOrg = dynamic_cast<Value_String const *>(&origin);
      if (!pOrg)
        throw Bad_Data_Type("Origin and Destination types do not match", "narrowing origin to destination type");
      data_ = pOrg->getData();
    }
    catch(Bad_Data_Type &ex)
    {
      ex.addOrigin("Value_String::set(Value const &)");
      throw ex;
    }
  }

  void Value_String::stringRepr(std::ostream &ostr, std::string const &str)
  {
    ostr << '"';
    for (std::string::size_type i = 0; (i != str.size()); i++)
    {
      if (str[i] == '"')
        ostr << '\\' << '"';
      else if (str[i] == '\\')
        ostr << '\\' << '\\';
      else if (str[i] == '/')
        ostr << '\\' << '/';
      else if (str[i] == '\b')
        ostr << '\\' << 'b';
      else if (str[i] == '\f')
        ostr << '\\' << 'f';
      else if (str[i] == '\n')
        ostr << '\\' << 'n';
      else if (str[i] == '\r')
        ostr << '\\' << 'r';
      else if (str[i] == '\t')
        ostr << '\\' << 't';
      else if (str[i] == ' ')
        ostr << ' ';
      else //if (isprint(str[i]))
        ostr << str[i];
      /*else
      {
        std::ostringstream  aux;
        int val = static_cast<int>(str[i]) & 0x00ff;
        aux << std::hex << val;
        ostr << '\\' << 'u';
        for (std::string::size_type j = 0; ((j + aux.str().size()) < 4); j++)
          ostr << '0';
        ostr << aux.str();
      }*/
    }
    ostr << '"';
  }

}                                // namespace JAULA


// EOF $Id: jaula_value_string.cc 45 2009-01-11 16:17:03Z morongo $
