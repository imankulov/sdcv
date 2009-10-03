#ifndef _READLINE_HPP_
#define _READLINE_HPP_

#include <string>
#include "lib/lib.h"
using std::string;

/**
 * Autocompletion function.
 * Uses autocompleter global object in order to perform autocompletion
 */
char *complete (const char * line, int item);

/**
 * Autocompletion class.
 * Object of the class performs the completions using @Libs object.
 */
class Autocompleter 
{
private: 
	Libs * libs;
	gchar ** reslist;

public:
	Autocompleter(Libs *libraries);
	~Autocompleter();
	char *complete(const gchar * line, gint item);
};

/** 
 * Global autocompleter object.
 */
extern Autocompleter *completer;


class read_line {
public:
	virtual ~read_line() {}
	virtual bool read(const string &banner, string& line)=0;
	virtual void add_to_history(const std::string& phrase) {}
};

extern read_line *create_readline_object(Libs *lib, bool has_history=true, bool has_completion=true);

#endif//!_READLINE_HPP_
