
/*
 * jaula_syntax_error.cc : JSON Analysis User Library Acronym
 * syntax analysis exception definitions
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_syntax_error.cc $
 * $Id: jaula_syntax_error.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}


#include <jaula_syntax_error.h>

namespace JAULA
{                                // namespace JAULA

  Syntax_Error::Syntax_Error(std::string const &detail
    , std::string const &action
    , std::string const &origin)
    : Exception(Exception::SYNTAX_ERROR, detail, action, origin)
    {}

  Syntax_Error::~Syntax_Error()
    {}

  Syntax_Error &Syntax_Error::operator=(Syntax_Error const &orig)
  {
    Exception::operator=(orig);
    return *this;
  }

}                                // namespace JAULA


// EOF $Id: jaula_syntax_error.cc 45 2009-01-11 16:17:03Z morongo $
