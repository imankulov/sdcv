/* 
 * This file part of sdcv - console version of Stardict program
 * http://sdcv.sourceforge.net
 * Copyright (C) 2005 Evgeniy <dushistov@mail.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#ifdef WITH_READLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#endif
#include <glib.h>

#include "utils.hpp"

#include "readline.hpp"

#ifndef WITH_READLINE

class dummy_readline : public read_line {
public:
	bool read(const string& banner, string& line) {
		printf(banner.c_str());
		return stdio_getline(stdin, line);
	}
};

#else

extern const char *rl_readline_name;
Autocompleter *completer;


char *complete (const char * line, int item)
{
	return completer->complete(line, item);
}


Autocompleter::Autocompleter(Libs *libraries)
{
	libs = libraries;
	reslist = new gchar*[ MAX_MATCH_ITEM_PER_LIB*libs->ndicts() ];
}

Autocompleter::~Autocompleter()
{
	delete []reslist;
}

char * Autocompleter::complete(const gchar * line, gint item)
{
	if (item == 0) {
		string rule(line);
		rule += '*';
		for (int i=0; i<MAX_MATCH_ITEM_PER_LIB*libs->ndicts(); i++)
			reslist[i] = NULL;
		libs->LookupWithRule(rule.c_str(), reslist);
	}
	return reslist[item];
}

class real_readline : public read_line {

private:
	bool has_history;
	bool has_completion;


public:
	real_readline(Libs *lib, bool has_history=true, bool has_completion=true)
	{
		rl_readline_name = (char *)"sdcv";
		this->has_history = has_history;
		this->has_completion = has_completion;
		if (has_history){
			string histname=(string(g_get_home_dir()) + G_DIR_SEPARATOR + ".sdcv_history");
			using_history();
			read_history(histname.c_str());
		}
		if (has_completion) {
			completer = new Autocompleter(lib);
			rl_completion_entry_function = &complete;
			rl_completer_word_break_characters = (char *)"";
			rl_completion_append_character = '\0';
		}
	}
	~real_readline() 
	{
		if (has_history){
			string histname=(string(g_get_home_dir())+G_DIR_SEPARATOR+".sdcv_history");
			write_history(histname.c_str());
			const gchar *hist_size_str=g_getenv("SDCV_HISTSIZE");
			int hist_size;
			if (!hist_size_str || sscanf(hist_size_str, "%d", &hist_size)<1)
				hist_size=2000;
			history_truncate_file(histname.c_str(), hist_size);
		}
		if (has_completion) {
			delete completer;
		}
	}
	bool read(const string &banner, string& line)
	{
		char *phrase=NULL;
		phrase=readline(banner.c_str());
		if (phrase) {
			line=phrase;
			free(phrase);
			return true;
		}
		return false;
	}
	void add_to_history(const std::string& phrase) 
	{ 
		if (has_history)
			add_history(phrase.c_str()); 
	}
};

#endif//WITH_READLINE

read_line *create_readline_object(Libs *lib, bool has_history, bool has_completion)
{
#ifdef WITH_READLINE
	return new real_readline(lib, has_history, has_completion);
#else
	return new dummy_readline();
#endif
}
