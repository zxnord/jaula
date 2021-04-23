 
/*
 * jaula_lexan.ll : JSON Analysis User Library Acronym
 * Lexical analysis implementation based on flex
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
 * $HeadURL: file:///svn/p/jaula/code/trunk/jaula/jaula_lexan.ll $
 * $Id: jaula_lexan.ll 49 2009-05-10 09:24:23Z morongo $
 * $Revision: 49 $
 */

%x	LINECOMM
%x	STR
%x	LITERAL

%option c++
%option prefix="jaula"
%option yyclass="JAULA::Lexan"
%option yylineno
%option noyywrap

%{

extern "C"
{
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
}

#include <cstdio>
#include <sstream>

#include <jaula/jaula_lexan_error.h>
#include <jaula/jaula_no_error.h>

#include <jaula_lexan.h>

%}

SIGN		[+-]
EXP		[eE]
DIGIT		[0-9]
HEXAU		[A-F]
HEXAL		[a-f]
HEXA		{HEXAU}|{HEXAL}
XDIGIT	{DIGIT}|{HEXA}
INT		{SIGN}?{DIGIT}+
FLOAT1	{SIGN}?{DIGIT}+\.{DIGIT}+({EXP}{SIGN}?{DIGIT}+)?
FLOAT2	{SIGN}?{DIGIT}+{EXP}{SIGN}?{DIGIT}+
FLOAT		{FLOAT1}|{FLOAT2}

%%

[ \f\n\r\t]+	/* Me como los espacios */

"["		|
"]"		|
"{"		|
"}"		|
":"		|
","		{ /* Meaningful symbols */
				return *yytext;
			} /* Meaningful symbols */

null		{ /* null constant value */
				return NULL_VALUE;
			} /* null constant value */

false		{ /* false constant value */
				return FALSE_VALUE;
			} /* false constant value */

true		{ /* true constant value */
				return TRUE_VALUE;
			} /* true constant value */

{FLOAT}	{ /* float value */
				tokenData = yytext;
				return NUMBER_VALUE;
			} /* float value */

{INT}		{ /* integer value */
				tokenData = yytext;
				return NUMBER_INT_VALUE;
			} /* integer value */

"#"		{ /* Begin comment */
				if (commented)
					BEGIN(LINECOMM);
				else
				{
					LexerError("invalid character (#) detected");
					yyterminate();
				}
			} /* Begin comment */

\"			{ /* Begin string */
				BEGIN(STR);
				tokenData.erase();
			} /* Begin string */

.			{ /* invalid character */
				std::ostringstream	ostr;
				ostr << "unexpected character '"
             << yytext
             << "' detected (quotes missing?)";
				LexerError(ostr.str().c_str());
				yyterminate();
			} /* invalid character */

<LINECOMM><<EOF>>	{	/* unterminated comment */
							LexerError("unterminated comment detected at end of input");
							yyterminate();
						} /* unterminated string */

<LINECOMM>\n		{	/* end comment */
							BEGIN(0);
						}	/* end comment */

<LINECOMM>.			/* Don't analyze commented data */

<STR>\"		{ /* end string */
						BEGIN(0);
						return STRING_VALUE;
					} /* end string */

<STR>\\		{ /* begin literal mode */
						yy_push_state(LITERAL);
					} /* begin literal mode */

<STR>\n		{ /* unterminated string */
						LexerError("unterminated string detected at end of line");
						yyterminate();
					} /* unterminated string */

<STR><<EOF>>	{ /* unterminated string */
						LexerError("unterminated string detected at end of input");
						yyterminate();
					} /* unterminated string */

<STR>.			{ /* append string content to token data */
						tokenData += yytext;
					} /* append string content to token data */

<LITERAL>\"		{ /* Literal quote */
						tokenData += '"';
						yy_pop_state();
					} /* Literal quote */

<LITERAL>\\		{ /* Literal backslash */
						tokenData += '\\';
						yy_pop_state();
					} /* Literal backslash */

<LITERAL>"/"		{ /* Literal slash */
						std::cerr << "jaula_lexan : Entre a procesar barra escapada." << std::endl;
						tokenData += '/';
						std::cerr << "jaula_lexan : Estado del token = " << tokenData << std::endl;
						yy_pop_state();
						std::cerr << "jaula_lexan : Vuelvo al estado anterior." << std::endl;
					} /* Literal slash */

<LITERAL>"b"		{ /* Literal backspace */
						tokenData += '\b';
						yy_pop_state();
					} /* Literal backspace */

<LITERAL>"f"		{ /* Literal form feed */
						tokenData += '\f';
						yy_pop_state();
					} /* Literal form feed */

<LITERAL>"n"		{ /* Literal new line */
						tokenData += '\n';
						yy_pop_state();
					} /* Literal new line */

<LITERAL>"r"		{ /* Literal carriage return */
						tokenData += '\r';
						yy_pop_state();
					} /* Literal carriage return */

<LITERAL>"t"		{ /* Literal tab */
						tokenData += '\t';
						yy_pop_state();
					} /* Literal tab */

<LITERAL>u{XDIGIT}{4}		{ /* unicode character */
						char	*reminder = 0;
						unsigned int accum = strtol(&(yytext[1]), &reminder, 16);
						if ((reminder) && (*reminder))
						{
							std::ostringstream	ostr;
							ostr << "Expresion \\" << yytext << "could not be converted to a unicode character";
							LexerError(ostr.str().c_str());
							yyterminate();
						}
						tokenData += static_cast<char>(accum & 0x00ff);
						yy_pop_state();
					} /* unicode character */

<LITERAL><<EOF>>	{ /* unterminated literal */
						LexerError("unterminated escape sequence detected at end of input");
						yyterminate();
					} /* unterminated literal */

<LITERAL>.		{ /* invalid literal */
						std::cerr << "jaula_lexan : Entre a procesar secuencia de escape invalida." << std::endl;
						std::ostringstream	ostr;
						ostr << "Invalid literal sequence \\" << yytext;
						LexerError(ostr.str().c_str());
						yyterminate();
					} /* invalid literal */

%%

/* End of automatically generated code */

namespace JAULA
{ // namespace JAULA

	Lexan::Lexan(std::istream &in_stream, bool comments_allowed)
		: ::jaulaFlexLexer(&in_stream, 0)
		, commented(comments_allowed)
		, tokenData()
		, pErrorReport()
	{}

	void Lexan::LexerError(const char *msg)
	{
 		std::ostringstream	errAct;
 		errAct << "analyzing line " << lineno() << " from input stream.";

		if (pErrorReport)
			delete pErrorReport;
		pErrorReport = new Lexan_Error(msg
      , errAct.str()
      , "JAULA::Lexan::LexerError(char const *)");
	}

	std::string const &Lexan::getTokenData(void) const
	{
 		return tokenData;
	}

	Lexan_Error const *Lexan::getErrorReport(void) const
	{
 		return pErrorReport;
	}

	Lexan::~Lexan()
	{
		if (pErrorReport)
			delete pErrorReport;
	}

} // namespace JAULA

/* EOF $Id: jaula_lexan.ll 49 2009-05-10 09:24:23Z morongo $ */
