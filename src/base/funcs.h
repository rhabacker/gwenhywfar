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

#ifndef GWENHYWFAR_Funcs_H
#define GWENHYWFAR_Funcs_H

#include <gwenhywfar/gwenhywfarapi.h>
#include <gwenhywfar/db.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef int (*GWEN_Func1)(void);
typedef int (*GWEN_Func2)(int, char**);
typedef int (*GWEN_Func3)(GWEN_DB_NODE*, int, char**);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * This is one of the very few structs inside Gwenhywfar whose
 * contents are available for direct access to the the program.
 * Developer's note: Please note that any change within this struct will
 * make it necessary to increment the SO_VERSION of the library !
 */
class GWENHYWFAR_API GWEN_Func {
public:
  GWEN_Func(const char *name, GWEN_Func1 func, const char *help = NULL);
  GWEN_Func(const char *name, GWEN_Func2 func, const char *help = NULL);
  GWEN_Func(const char *name, GWEN_Func3 func, const char *help = NULL);
  GWEN_Func();

  int call() const;
  int call(int argc, char **argv) const;
  int call(GWEN_DB_Node *node, int argc, char **argv) const;

protected:
  const char *_name;
  GWEN_Func1 _func1;
  GWEN_Func2 _func2;
  GWEN_Func3 _func3;
  const char *_help;
};

class GWENHYWFAR_API GWEN_Funcs {
public:
  GWEN_Funcs(const GWEN_Func *funcs);

  const GWEN_Func *get(const char *name);

protected:
  GWEN_Func *_funcs;
};

#endif

/**
 * This function prints out a space separated list of all defined functions
 * without description.
 */
GWENHYWFAR_API
void GWEN_Funcs_Usage(const GWEN_Func* funcs);

/**
 * This function outputs a list of all defined functions including the description
 */
GWENHYWFAR_API
void GWEN_Funcs_Usage_With_Help(const GWEN_Func* funcs);

/**
 * This function returns a pointer to the function described by \p name
 * or NULL if the function was not found.
 */
GWENHYWFAR_API
const GWEN_Func* GWEN_Funcs_Find(const GWEN_Func* funcs, const char *name);


#endif /* GWENHYWFAR_Funcs_H */



