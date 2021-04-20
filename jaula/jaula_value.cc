
/*
 * jaula_value.cc : JSON Analysis User Library Acronym
 * Generic values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value.cc $
 * $Id: jaula_value.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}


#ifdef HAVE_IOSTREAM
#include <iostream>
#endif

#include <jaula/jaula_value.h>
#include <jaula/jaula_value_array.h>
#include <jaula/jaula_value_boolean.h>
#include <jaula/jaula_value_null.h>
#include <jaula/jaula_value_number.h>
#include <jaula/jaula_value_number_int.h>
#include <jaula/jaula_value_object.h>
#include <jaula/jaula_value_string.h>

namespace JAULA
{                                // namespace JAULA

  Value::~Value()
    {}

  Value::ValueType Value::getType(void) const
  {
    return Type_;
  }

  void Value::set(Value const &origin) throw(Bad_Data_Type)
  {
    if (Type_ != origin.getType())
      throw Bad_Data_Type("Origin and destination value types do not match"
        , "checking value types"
        , "Value::set(Value const &)");
  }

  Value &Value::operator=(Value const &orig) throw(Bad_Data_Type)
  {
    try
    {
      if (this == &orig)
        return *this;
      set(orig);
      return *this;
    }
    catch(Bad_Data_Type &ex)
    {
      ex.addOrigin("Value::operator=(Value const &)");
      throw ex;
    }
  }

  Value *Value::duplicate(Value const &orig)
  {
    try
    {

      {                          // Null Value
        Value_Null const *pOrg = dynamic_cast<Value_Null const *>(&orig);
        if (pOrg)
          return new Value_Null();
      }                          // Null Value

      {                          // Boolean Value
        Value_Boolean const *pOrg =
          dynamic_cast<Value_Boolean const *>(&orig);
        if (pOrg)
          return new Value_Boolean(pOrg->getData());
      }                          // Boolean Value

      {                          // String Value
        Value_String const *pOrg = dynamic_cast<Value_String const *>(&orig);
        if (pOrg)
          return new Value_String(pOrg->getData());
      }                          // String Value

      {                          // Number (Int) Value
        Value_Number_Int const *pOrg =
          dynamic_cast<Value_Number_Int const *>(&orig);
        if (pOrg)
          return new Value_Number_Int(pOrg->getData());
      }                          // Number (Int) Value

      {                          // Number Value
        Value_Number const *pOrg = dynamic_cast<Value_Number const *>(&orig);
        if (pOrg)
          return new Value_Number(pOrg->getData());
      }                          // Number Value

      {                          // Array Value
        Value_Array const *pOrg = dynamic_cast<Value_Array const *>(&orig);
        if (pOrg)
          return new Value_Array(pOrg->getData());
      }                          // Array Value

      {                          // Object Value
        Value_Object const *pOrg = dynamic_cast<Value_Object const *>(&orig);
        if (pOrg)
          return new Value_Object(pOrg->getData());
      }                          // Object Value

      throw Bad_Data_Type("Unsupported data type detected"
        , "generating a deep copy of a value");
    }
    catch(Exception &ex)
    {
      // This should never happen unless there is a serious
      // bug in the code so exceptions will be only display
      // in extended debugging mode
#ifdef EXTENDED_DEBUG
      ex.addOrigin("Value::duplicate(Value const &)");
      std::cout << ex;
#endif
      return new Value_Null();
    }
  }

  Value::Value(ValueType Type)
    : Type_(Type)
    {}

}                                // namespace JAULA


std::ostream &operator<<(std::ostream &ostr, JAULA::Value const &val)
{
  val.repr(ostr);
  return ostr;
}


// EOF $Id: jaula_value.cc 45 2009-01-11 16:17:03Z morongo $
