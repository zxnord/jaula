
/*
 * jaula_value_array.h : jaula.h : JSON Analysis User Library Acronym
 * JSON array values definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_array.cc $
 * $Id: jaula_value_array.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

}


#include <jaula/jaula_value_array.h>

namespace JAULA
{                                // namespace JAULA

  Value_Array::Value_Array(void)
    : Value_Complex(TYPE_ARRAY)
    , data_()
    {}

  Value_Array::Value_Array(Value_Array::dataType const &data)
    : Value_Complex(TYPE_ARRAY)
    , data_()
  {
    for (dataType::const_iterator it = data.begin(); (it != data.end()); it++)
      data_.push_back(duplicate(*(*it)));
  }

  Value_Array::~Value_Array()
  {
    clear();
  }

  Value_Array::dataType const &Value_Array::getData(void) const
  {
    return data_;
  }

  void Value_Array::repr(std::ostream &ostr) const
  {
    ostr << "[ ";
    for (dataType::const_iterator it = data_.begin()
      ; (it != data_.end())
      ; it++)
    {
      if (it != data_.begin())
        ostr << ", ";
      ostr << **it;
    }
    ostr << " ]";
  }

  bool Value_Array::empty(void) const
  {
    return data_.empty();
  }

  size_t Value_Array::size(void) const
  {
    return data_.size();
  }

  void Value_Array::set(dataType const &data)
  {
    clear();
    for (dataType::const_iterator it = data.begin(); (it != data.end()); it++)
      data_.push_back(duplicate(*(*it)));
  }

  void Value_Array::set(Value const &origin)
  {
    try
    {
      Value::set(origin);
      Value_Array const *pOrg = dynamic_cast<Value_Array const *>(&origin);
      if (!pOrg)
        throw Bad_Data_Type("Origin and Destination types do not match"
          , "narrowing origin to destination type");
      set(pOrg->getData());
    }
    catch(Bad_Data_Type &ex)
    {
      ex.addOrigin("Value_Array::set(Value const &)");
      throw ex;
    }
  }

  void Value_Array::addItem(Value const &item)
  {
    data_.push_back(duplicate(item));
  }

  void Value_Array::clear(void)
  {
    for (dataType::const_iterator it = data_.begin()
      ; (it != data_.end())
      ; it++)
      delete *it;
    data_.clear();
  }

}                                // namespace JAULA


// EOF $Id: jaula_value_array.cc 45 2009-01-11 16:17:03Z morongo $
