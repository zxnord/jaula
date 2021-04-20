
/*
 * jaula_value_object.cc : JSON Analysis User Library Acronym
 *                         JSON object values definitions
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
 * Author: morongo $
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_value_object.cc $
 * $Id: jaula_value_object.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

}


#include <jaula/jaula_value_object.h>

namespace JAULA
{                                // namespace JAULA

  Value_Object::Value_Object(void)
    : Value_Complex(TYPE_OBJECT)
    , data_()
    {}

  Value_Object::Value_Object(Value_Object::dataType const &data)
    : Value_Complex(TYPE_OBJECT)
    , data_()
  {
    for (dataType::const_iterator it = data.begin(); (it != data.end()); it++)
      data_.insert(dataType::value_type(it->first, duplicate(*(it->second))));
  }

  Value_Object::~Value_Object()
  {
    clear();
  }

  Value_Object::dataType const &Value_Object::getData(void) const
  {
    return data_;
  }

  void Value_Object::repr(std::ostream &ostr) const
  {
    ostr << "{ ";
    for (dataType::const_iterator it = data_.begin(); (it != data_.end()); it++)
    {
      if (it != data_.begin())
        ostr << ", ";
      ostr << '"' << it->first << '"'
        << " : " << *(it->second);
    }
    ostr << " }";
  }

  bool Value_Object::empty(void) const
  {
    return data_.empty();
  }

  size_t Value_Object::size(void) const
  {
    return data_.size();
  }

  void Value_Object::set(dataType const &data)
  {
    clear();
    for (dataType::const_iterator it = data.begin(); (it != data.end()); it++)
      data_.insert(dataType::value_type(it->first, duplicate(*(it->second))));
  }

  void Value_Object::set(Value const &origin) throw(Bad_Data_Type)
  {
    try
    {
      Value::set(origin);
      Value_Object const *pOrg = dynamic_cast<Value_Object const *>(&origin);
      if (!pOrg)
        throw Bad_Data_Type("Origin and Destination types do not match", "narrowing origin to destination type");
      set(pOrg->getData());
    }
    catch(Bad_Data_Type &ex)
    {
      ex.addOrigin("Value_Object::set(Value const &)");
      throw ex;
    }
  }

  void Value_Object::insertItem(std::string const &name, Value const &item) throw(Name_Duplicated)
  {
    std::pair<dataType::iterator, bool> insSt = data_.insert(dataType::value_type(name, duplicate(item)));
    if (!insSt.second)
      throw Name_Duplicated(name, "inserting a new object property"
        , "Value_Object::insertItem(std::string const &, Value const &)");
  }

  void Value_Object::clear(void)
  {
    for (dataType::const_iterator it = data_.begin(); (it != data_.end()); it++)
      delete it->second;
    data_.clear();
  }

}                                // namespace JAULA


// EOF $Id: jaula_value_object.cc 45 2009-01-11 16:17:03Z morongo $
