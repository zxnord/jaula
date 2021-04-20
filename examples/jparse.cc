
/*
 * jparse.cc : JSON analysis user library unit test
 *             Test for JSON files syntactic analysis
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/examples/jparse.cc $
 * $Id: jparse.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}



#ifdef HAVE_MEMORY
#include <memory>
#endif

#ifdef HAVE_FSTREAM
#include <fstream>
#endif

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif

#ifdef HAVE_STRING
#include <string>
#endif

#include <jaula.h>

#define APPNAME "jparse"

int main(int argc, char *argv[])
{

	std::cout << APPNAME << " : syntax analysis test for " << PACKAGE_NAME << " version " << PACKAGE_VERSION << std::endl
		<< std::endl
		<< "usage : " << APPNAME << "file_specs" << std::endl
		<< std::endl;

	unsigned int good = 0;
	for(int i = 1; (i < argc); i++)
	{

		std::ifstream    arch(argv[i], std::ios::in);
		if (!arch)
		{
			std::cerr << "Could not open file " << argv[i] << std::endl
				<< ". Execution aborted." << std::endl;
			return 1;
		}

		try
		{
			std::cout << "Analyzing " << argv[i] << " ...";
			std::auto_ptr<JAULA::Value_Complex> pParsed(JAULA::Parser::parseStream(arch));
			std::cout << "Ok." << std::endl
				<< std::endl
				<< "Reduced expression of " << argv[i] << ':' << std::endl
				<< *(pParsed.get()) << std::endl;
			good++;
		}
		catch(JAULA::Exception ex)
		{
			std::cout << "Error." << std::endl;
			std::cerr << std::endl << ex << std::endl;
		}

		arch.close();

		std::cout << std::endl;
	}

	std::cerr << "Execution finished. " << good << " file(s) successfully parsed." << std::endl;
	return 0;
}												  /* main */



// EOF $Id: jparse.cc 45 2009-01-11 16:17:03Z morongo $
