/*
 * lextst.cc : JSON analysis user library unit test
 *             Test for lexical analysis routines
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/examples/lextst.cc $
 * $Id: lextst.cc 45 2009-01-11 16:17:03Z morongo $
 * $Revision: 45 $
 */

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
}


#include <fstream>
#include <iostream>
#include <string>

#include <jaula.h>

#define APPNAME "lextst"

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "jaula"
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "1.4.1"
#endif

int main(int argc, char *argv[])
{

	std::cout << APPNAME << " : lexical analysis test for " << PACKAGE_NAME << " version " << PACKAGE_VERSION << std::endl
		<< std::endl
		<< "usage : " << APPNAME << "file_specs" << std::endl
		<< std::endl;

	try
	{
		for(int i = 1; (i < argc); i++)
		{

			std::ifstream    arch(argv[i], std::ios::in);
			if (!arch)
			{
				std::cerr << "Could not open file " << argv[i] << std::endl
					<< ". Execution aborted." << std::endl;
				return 1;
			}

			std::cout << "Analyzing " << argv[i] << " ..." << std::endl;
			JAULA::Lexan lexan(arch, true);
			for (int resp = lexan.yylex(); (resp); resp = lexan.yylex())
			{
				switch (resp)
				{
					case '[' :
						std::cout << " [ " << std::endl;
						break;

					case ']' :
						std::cout << " ] " << std::endl;
						break;

					case '{' :
						std::cout << " { " << std::endl;
						break;

					case '}' :
						std::cout << " } " << std::endl;
						break;

					case ':' :
						std::cout << " : " << std::endl;
						break;

					case ',' :
						std::cout << ", " << std::endl;
						break;

					case NULL_VALUE  :
						std::cout << "null" << std::endl;
						break;

					case FALSE_VALUE  :
						std::cout << "false" << std::endl;
						break;

					case TRUE_VALUE  :
						std::cout << "true" << std::endl;
						break;

					case NUMBER_VALUE  :
						std::cout << lexan.getTokenData() << std::endl;
						break;

					case NUMBER_INT_VALUE  :
						std::cout << lexan.getTokenData() << std::endl;
						break;

					case STRING_VALUE  :
						std::cout << '"' << lexan.getTokenData() << '"' << std::endl;
						break;

					default :
						std::cerr << std::endl
							<< "Unsupported Symbol read (" << resp
							<< ") analyzing line "
							<< lexan.lineno()
							<< "from " << argv[i] << "." << std::endl;
						break;
				}

			}

			arch.close();

			if (lexan.getErrorReport())
				throw *lexan.getErrorReport();

			std::cout << argv[i] << " analyzed successfully." << std::endl;
		}

		std::cerr << "Execution successfully finished" << std::endl;
		return 0;
	}
	catch(JAULA::Exception ex)
	{
		std::cerr << std::endl << ex << std::endl;
		return 1;
	}
}												  /* main */



// EOF $Id: lextst.cc 45 2009-01-11 16:17:03Z morongo $
