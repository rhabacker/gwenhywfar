/***************************************************************************
  $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Fri Nov 22 2002
    copyright   : (C) 2002 by Martin Preuss
    email       : martin@libchipcard.de


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

#include "libloader_p.h"
#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>
#include <gwenhywfar/buffer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <windows.h>


const char *GWEN_LibLoader_ErrorString(int c);


static int gwen_libloader_is_initialized=0;
static GWEN_ERRORTYPEREGISTRATIONFORM *gwen_libloader_errorform=0;


GWEN_ERRORCODE GWEN_LibLoader_ModuleInit(){
  if (!gwen_libloader_is_initialized) {
    GWEN_ERRORCODE err;

    gwen_libloader_errorform=GWEN_ErrorType_new();
    GWEN_ErrorType_SetName(gwen_libloader_errorform,
                           GWEN_LIBLOADER_ERROR_TYPE);
    GWEN_ErrorType_SetMsgPtr(gwen_libloader_errorform,
                             GWEN_LibLoader_ErrorString);
    err=GWEN_Error_RegisterType(gwen_libloader_errorform);
    if (!GWEN_Error_IsOk(err))
      return err;
    gwen_libloader_is_initialized=1;
  }
  return 0;
}



GWEN_ERRORCODE GWEN_LibLoader_ModuleFini(){
  if (gwen_libloader_is_initialized) {
    GWEN_ERRORCODE err;

    err=GWEN_Error_UnregisterType(gwen_libloader_errorform);
    if (!GWEN_Error_IsOk(err))
      return err;
    gwen_libloader_is_initialized=0;
  }
  return 0;
}



GWEN_LIBLOADER *GWEN_LibLoader_new(){
  GWEN_LIBLOADER *h;

  GWEN_NEW_OBJECT(GWEN_LIBLOADER, h);
  return h;
}



void GWEN_LibLoader_free(GWEN_LIBLOADER *h){
  if (h) {
    free(h);
  }
}



GWEN_ERRORCODE GWEN_LibLoader_LoadLibrary(GWEN_LIBLOADER *h,
                                          const char *name){
  assert(h);

  h->handle=(void*)LoadLibrary(name);
  if (!h->handle) {
    int werr;
    char *lpMsgBuf; /* from: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/formatmessage.asp */

    werr=GetLastError();

    /* DEBUG */
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		  FORMAT_MESSAGE_FROM_SYSTEM |
		  FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL,
		  werr,
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		  (LPTSTR) &lpMsgBuf,
		  0,
		  NULL);
    fprintf(stderr, "Error loading DLL \"%s\": %d\n%d = %s\n", 
	    name, werr, werr, lpMsgBuf);
    LocalFree(lpMsgBuf);

    if ( (werr == ERROR_DLL_NOT_FOUND) ||
	 (werr == ERROR_FILE_NOT_FOUND) ||
	 (werr == ERROR_MOD_NOT_FOUND) ) {
      DBG_INFO(0, "File \"%s\" not found", name);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
                            GWEN_LIBLOADER_ERROR_NOT_FOUND);
    }
    /* TODO: Find the code for resolve errors */
    else {
      DBG_INFO(0, "Error loading library \"%s\" (%d)", name, werr);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
                            GWEN_LIBLOADER_ERROR_COULD_NOT_LOAD);
    }
  }
  return 0;
}



GWEN_ERRORCODE GWEN_LibLoader_CloseLibrary(GWEN_LIBLOADER *h){
  assert(h);

  if (!h->handle)
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
                          GWEN_LIBLOADER_ERROR_NOT_OPEN);
  if (!FreeLibrary((HINSTANCE)h->handle))
    return GWEN_Error_new(0,
			  GWEN_ERROR_SEVERITY_ERR,
			  GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
			  GWEN_LIBLOADER_ERROR_COULD_NOT_CLOSE);
  h->handle=0;
  return 0;
}



GWEN_ERRORCODE GWEN_LibLoader_Resolve(GWEN_LIBLOADER *h,
                                      const char *name, void **p){
  assert(h);
  assert(name);
  assert(p);

  if (!h->handle)
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
                          GWEN_LIBLOADER_ERROR_NOT_OPEN);
  *p=(void*)GetProcAddress((HINSTANCE)h->handle,name);
  if (!*p) {
    DBG_ERROR(0, "Error resolving symbol \"%s\"\n",
              name);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_LIBLOADER_ERROR_TYPE),
                          GWEN_LIBLOADER_ERROR_COULD_NOT_RESOLVE);
  }
  DBG_VERBOUS(0, "Resolved symbol \"%s\": %08x\n",
	      name, (int)*p);
  return 0;
}



GWEN_ERRORCODE GWEN_LibLoader_OpenLibraryWithPath(GWEN_LIBLOADER *h,
                                                  const char *path,
						  const char *name){
  GWEN_BUFFER *buffer;
  unsigned int pos;
  unsigned int i;
  GWEN_ERRORCODE err;

  assert(h);
  assert(name);
  buffer=GWEN_Buffer_new(0, 256, 0, 1);

  if (path) {
    GWEN_Buffer_AppendString(buffer, path);
    GWEN_Buffer_AppendByte(buffer, '\\'); /* Use backslash: This is windows! */
  }
  /* remember current position */
  pos=GWEN_Buffer_GetPos(buffer);
  /* append name of the library to load */
  GWEN_Buffer_AppendString(buffer, name);
  i=strlen(name);
  if ((i<=4) || (strcmp(name+i-4, ".dll")!=0)) {
    /* no DLL-extension, add it myself */
    GWEN_Buffer_AppendString(buffer, ".dll");
  }

  /* append trailing null byte */
  GWEN_Buffer_AppendByte(buffer, (char)0);

  /* try to load the library */
  err=GWEN_LibLoader_LoadLibrary(h, GWEN_Buffer_GetStart(buffer));
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO(0, "Could not load library \"%s\"",
             GWEN_Buffer_GetStart(buffer));
    DBG_INFO_ERR(0, err);
    GWEN_Buffer_free(buffer);
    return err;
  }

  DBG_INFO(0, "Library \"%s\" loaded",
	   GWEN_Buffer_GetStart(buffer));
  GWEN_Buffer_free(buffer);
  return 0;
}



GWEN_ERRORCODE GWEN_LibLoader_OpenLibrary(GWEN_LIBLOADER *h,
					  const char *name){
  return GWEN_LibLoader_OpenLibraryWithPath(h, 0, name);
}



const char *GWEN_LibLoader_ErrorString(int c){
  const char *s;

  switch(c) {
  case 0:
    s="Success";
    break;

  case GWEN_LIBLOADER_ERROR_COULD_NOT_LOAD:
    s="Could not load library";
    break;
  case GWEN_LIBLOADER_ERROR_NOT_OPEN:
    s="Library not open";
    break;
  case GWEN_LIBLOADER_ERROR_COULD_NOT_CLOSE:
    s="Could not close library";
    break;
  case GWEN_LIBLOADER_ERROR_COULD_NOT_RESOLVE:
    s="Could not resolve symbol";
    break;
  case GWEN_LIBLOADER_ERROR_NOT_FOUND:
    s="Library not found";
    break;
  default:
    s=(const char*)0;
  } /* switch */
  return s;
}






