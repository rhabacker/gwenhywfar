/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Tue Sep 16 2003
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


#ifndef GWENHYFWAR_SERVICELAYER_H
#define GWENHYFWAR_SERVICELAYER_H

#include <gwenhyfwar/error.h>
#include <gwenhyfwar/connlayer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GWEN_SERVICELAYER GWEN_SERVICELAYER;


GWEN_SERVICELAYER *GWEN_ServiceLayer_new();
void GWEN_ServiceLayer_free(GWEN_SERVICELAYER *sl);


/**
 * Adds a connection to the service layer.
 */
GWEN_ERRORCODE
  GWEN_ServiceLayer_AddConnection(GWEN_SERVICELAYER *sl,
                                  GWEN_IPCCONNLAYER *conn);

/**
 * Lookup a connection by its id and a user mark.
 * @param id connection id (automatically assigned when creating the
 *        connection)
 * @param userMark mark assigned by the application (0 matches any)
 */
GWEN_IPCCONNLAYER*
  GWEN_ServiceLayer_FindConnection(GWEN_SERVICELAYER *sl,
                                   unsigned int id,
                                   unsigned int userMark);

/**
 * This function calls the work function of all connections, for which
 * data is available (either read or write) or whose socket state changed
 * (e.g. when there is an incoming connection, or when an outgoing connection
 * is established etc).
 * The given timeout value is used when waiting for socket changes (select).
 */
GWEN_ERRORCODE GWEN_ServiceLayer_Work(GWEN_SERVICELAYER *sl, int timeout);


/**
 * Removes all connections which belong to the given service layer and which
 * are closed. Connections with the PERSISTENT-flag are not removed.
 */
void GWEN_ServiceLayer_RemoveClosed(GWEN_SERVICELAYER *sl);


/**
 * Closes all matching connections belonging to the given service layer.
 * @param id connection id (assigned upon creating it. 0 matches any)
 * @param userMark a special mark assigned by the application (0 matches any)
 * @param force if !=0 then the connection will be closed physically
 *        immediately. Otherwise the ConnectionLayer is allowed to do some
 *        closing handshaking.
 */
void GWEN_ServiceLayer_Close(GWEN_SERVICELAYER *sl,
                             unsigned int id,
                             unsigned int userMark,
                             int force);

/**
 * Returns the next incoming message. This function uses a round-robin
 * mechanism to make sure every connection has the same chance of providing
 * a message.
 */
GWEN_IPCMSG *GWEN_ServiceLayer_NextIncomingMsg(GWEN_SERVICELAYER *sl,
                                               unsigned int userMark);


#ifdef __cplusplus
}
#endif


#endif /* GWENHYFWAR_SERVICELAYER_H */






