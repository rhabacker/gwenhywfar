/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Fri Sep 19 2003
    copyright   : (C) 2003 by Martin Preuss
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

#ifndef GWENHYFWAR_CMDLAYER_H
#define GWENHYFWAR_CMDLAYER_H

#include <gwenhyfwar/connlayer.h>
#include <gwenhyfwar/servicelayer.h>
#include <gwenhyfwar/db.h>

#define GWEN_CMDLAYER_MAXMSGSIZE 8192

typedef struct GWEN_IPCSERVICECMD GWEN_IPCSERVICECMD;


GWEN_IPCSERVICECMD *GWEN_IPCServiceCmd_new();
void GWEN_IPCServiceCmd_free(GWEN_IPCSERVICECMD *s);

GWEN_ERRORCODE GWEN_IPCServiceCmd_Init(GWEN_IPCSERVICECMD *s,
                                       const char *xmlfile);
GWEN_ERRORCODE GWEN_IPCServiceCmd_Fini(GWEN_IPCSERVICECMD *s);

unsigned int GWEN_IPCServiceCmd_AddListener(GWEN_IPCSERVICECMD *s,
                                            const char *addr,
                                            int port,
                                            unsigned int mark);

unsigned int GWEN_IPCServiceCmd_AddPeer(GWEN_IPCSERVICECMD *s,
                                        const char *addr,
                                        int port,
                                        unsigned int mark);

GWEN_ERRORCODE GWEN_IPCServiceCmd_Work(GWEN_IPCSERVICECMD *s,
                                       int timeout);


GWEN_IPCMSG *GWEN_ConnectionLayerCmd_CreateMsg(GWEN_IPCCONNLAYER *cl,
                                               unsigned int refId,
                                               const char *name,
                                               unsigned int version,
                                               GWEN_DB_NODE *da);

GWEN_DB_NODE *GWEN_IPCServiceCmd_ParseMsg(GWEN_IPCCONNLAYER *cl,
                                          GWEN_IPCMSG *msg);

void GWEN_IPCServiceLayerCmd_SetNameAndVersion(GWEN_IPCSERVICECMD *s,
                                               const char *name,
                                               const char *version);

GWEN_IPCCONNLAYER*
  GWEN_IPCServiceLayerCmd_FindConnection(GWEN_IPCSERVICECMD *s,
                                         unsigned int id,
                                         unsigned int userMark);


const char *GWEN_ConnectionLayerCmd_GetPeerName(GWEN_IPCCONNLAYER *cl);
const char *GWEN_ConnectionLayerCmd_GetPeerVersion(GWEN_IPCCONNLAYER *cl);


#endif /* GWENHYFWAR_CMDLAYER_H */



