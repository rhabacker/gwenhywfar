/***************************************************************************
    begin       : Wed Feb 15 2022
    copyright   : (C) 2022 by Ralf Habacker
    email       : ralf.habacker@freenet.de

 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "funcs.h"

#include <stdio.h>

GWEN_Func::GWEN_Func() : _name(NULL), _func1(NULL), _func2(NULL), _func3(NULL), _help(NULL) {}
GWEN_Func::GWEN_Func(const char *name, GWEN_Func1 func, const char *help) : _name(name), _func1(func), _func2(NULL), _func3(NULL), _help(help) {}
GWEN_Func::GWEN_Func(const char *name, GWEN_Func2 func, const char *help) : _name(name), _func1(NULL), _func2(func), _func3(NULL), _help(help) {}
GWEN_Func::GWEN_Func(const char *name, GWEN_Func3 func, const char *help) : _name(name), _func1(NULL), _func2(NULL), _func3(func), _help(help) {}

int GWEN_Func::call() const
{
  if (!_func1)
    return -1;
  return _func1();
}

int GWEN_Func::call(int argc, char **argv) const
{
  if (!_func2)
    return -1;
  return _func2(argc, argv);
}

int GWEN_Func::call(GWEN_DB_NODE *node, int argc, char **argv) const
{
  if (!_func3)
    return -1;
  return _func3(node, argc, argv);
}

static const GWEN_Func *Funcs::get(const char *name)
{
    for(const GWEN_Func *f = _funcs; f->_name; f++) {
        if (strcmp(f->_name, name) == 0)
            return f;
    }
    return NULL;
}

/**
 * This function prints out a space separated list of all defined functions
 */
void GWEN_Func_Usage(const GWEN_Func *Func)
{
    const GWEN_Func *p;

    for (p=Func; p->name; p++) {
      fprintf(stderr, " %s", p->name);
      if (p->description)
        fprintf(stderr, " (%s)", p->description);
    }
    fprintf(stderr, "\n");
}

/**
 * This function prints out list of all defined functions including the description
 */
void GWEN_Func_Usage_With_Help(const GWEN_Func *Func)
{
    const GWEN_Func *p;

    for (p=Func; p->name; p++) {
      fprintf(stderr, "  %s:\n    %s\n\n", p->name, p->description ? p->description : "");
    }
}

/**
 * This function returns a pointer to the function identified by \p name
 * or NULL if function has not been found.
 */
const GWEN_Func* GWEN_Func_Find(const GWEN_Func *Func, const char *name)
{
  const GWEN_Func *p;

  for (p=Func; p->name; p++) {
    if (strcasecmp(name, p->name)==0)
      return p;
  }
  return NULL;
}



