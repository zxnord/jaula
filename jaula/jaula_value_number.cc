
/*
 * jaula_value_number.cc : JSON Analysis User Library Acronym
 *                         JSON numeric values definitions
 *
 * Copyright (C) 2007, 2008, 2009 Kombo Morongo <morongo666@gmail.com>
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_number.cc $
 * $Id: jaula_value_number.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}


#include <jaula/jaula_value_number.h>

namespace JAULA
{                                // namespace JAULA

  Value_Number::Value_Number(double data)
    : Value(TYPE_NUMBER)
    , data_(data)
    {}

  Value_Number::~Value_Number()
    {}

  double Value_Number::getData(void) const
  {
    return data_;
  }

  void Value_Number::repr(std::ostream &ostr) const
  {
    ostr << data_;
  }

  void Value_Number::set(double data)
  {
    data_ = data;
  }

  void Value_Number::set(Value const &origin)
  {
    try
    {
      Value::set(origin);
      Value_Number const *pOrg = dynamic_cast<Value_Number const *>(&origin);
      if (!pOrg)
        throw Bad_Data_Type("Origin and Destination types do not match", "narrowing origin to destination type");
      data_ = pOrg->getData();
    }
    catch(Bad_Data_Type &ex)
    {
      ex.addOrigin("Value_Number::set(Value const &)");
      throw ex;
    }
  }

}                                // namespace JAULA


// EOF $Id: jaula_value_number.cc 45 2009-01-11 16:17:03Z morongo $
