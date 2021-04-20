
/*
 * jaula_exception.cc : JSON Analysis User Library Acronym
 * Error handling definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_exception.cc $
 * $Id: jaula_exception.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}


#include <jaula/jaula_exception.h>

namespace JAULA
{                                // namespace JAULA

  Exception::Exception(ExCode code
    , std::string const &detail
    , std::string const &action
    , std::string const &origin)
    : code_(code)
    , detail_(detail)
    , action_(action)
    , origin_(origin)
    {}

  Exception::Exception(Exception const &orig)
    : code_(orig.getCode())
    , detail_(orig.getDetail())
    , action_(orig.getAction())
    , origin_(orig.getOrigin())
    {}

  Exception::~Exception()
    {}

  Exception::ExCode Exception::getCode(void) const
  {
    return code_;
  }

  std::string const &Exception::getDetail(void) const
  {
    return detail_;
  }

  std::string const &Exception::getAction(void) const
  {
    return action_;
  }

  std::string const &Exception::getOrigin(void) const
  {
    return origin_;
  }

  void Exception::display(std::ostream &ostr) const
  {
    ostr << "JAULA Exception(";
    switch(code_)
    {
      case NO_ERROR :
        ostr << "No Error";
        break;

      case BAD_DATA_TYPE :
        ostr << "Bad Data Type";
        break;

      case NAME_DUPLICATED :
        ostr << "Property name already exists";
        break;

      case LEXAN_ERROR :
        ostr << "Error detected during lexical analysis";
        break;

      case SYNTAX_ERROR :
        ostr << "Error detected during syntax analysis";
        break;

      default :
        ostr << "Unknown Error";
        break;

    }
    ostr << ")";

    if (!detail_.empty())
      ostr << " : " << detail_ << ".";

    if (!action_.empty())
      ostr << " Exception arose while " << action_ << ".";

    if (!origin_.empty())
      ostr << " Exception detected running " << origin_ << ".";
  }

  void Exception::setCode(ExCode code)
  {
    code_ = code;
  }

  void Exception::setDetail(std::string const &detail)
  {
    detail_ = detail;
  }

  void Exception::setAction(std::string const &action)
  {
    action_ = action;
  }

  void Exception::setOrigin(std::string const &origin)
  {
    origin_ = origin;
  }

  void Exception::addOrigin(std::string const &origin)
  {
    if (origin_.empty())
      origin_ = origin;
    else
    {
      origin_ += " invoked by ";
      origin_ += origin;
    }
  }

  Exception &Exception::operator=(Exception const &orig)
  {
    if (this == &orig)
      return *this;

    code_ = orig.getCode();
    detail_ = orig.getDetail();
    action_ = orig.getAction();
    origin_ = orig.getOrigin();

    return *this;
  }

}                                // namespace JAULA


std::ostream &operator<<(std::ostream &ostr, JAULA::Exception const &ex)
{
  ex.display(ostr);
  return ostr;
}


// EOF $Id: jaula_exception.cc 45 2009-01-11 16:17:03Z morongo $
