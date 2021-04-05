/***************************************************************************
    begin       : Mon Feb 08 2021
    copyright   : (C) 2021 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "gwenbuild/buildctx/buildsubcmd_p.h"

#include <gwenhywfar/debug.h>
#include <gwenhywfar/misc.h>


GWEN_LIST_FUNCTIONS(GWB_BUILD_SUBCMD, GWB_BuildSubCmd);




GWB_BUILD_SUBCMD *GWB_BuildSubCmd_new(void)
{
  GWB_BUILD_SUBCMD *cmd;

  GWEN_NEW_OBJECT(GWB_BUILD_SUBCMD, cmd);

  return cmd;
}



void GWB_BuildSubCmd_free(GWB_BUILD_SUBCMD *cmd)
{
  if (cmd) {
    free(cmd->command);
    free(cmd->arguments);
    free(cmd->buildMessage);

    GWEN_FREE_OBJECT(cmd);
  }
}



uint32_t GWB_BuildSubCmd_GetFlags(const GWB_BUILD_SUBCMD *cmd)
{
  return cmd->flags;
}



void GWB_BuildSubCmd_SetFlags(GWB_BUILD_SUBCMD *cmd, uint32_t f)
{
  cmd->flags=f;
}



void GWB_BuildSubCmd_AddFlags(GWB_BUILD_SUBCMD *cmd, uint32_t f)
{
  cmd->flags|=f;
}



void GWB_BuildSubCmd_DelFlags(GWB_BUILD_SUBCMD *cmd, uint32_t f)
{
  cmd->flags&=~f;
}



const char *GWB_BuildSubCmd_GetCommand(const GWB_BUILD_SUBCMD *cmd)
{
  return cmd->command;
}



void GWB_BuildSubCmd_SetCommand(GWB_BUILD_SUBCMD *cmd, const char *s)
{
  free(cmd->command);
  cmd->command=s?strdup(s):NULL;
}



const char *GWB_BuildSubCmd_GetArguments(const GWB_BUILD_SUBCMD *cmd)
{
  return cmd->arguments;
}



void GWB_BuildSubCmd_SetArguments(GWB_BUILD_SUBCMD *cmd, const char *s)
{
  free(cmd->arguments);
  cmd->arguments=s?strdup(s):NULL;
}



const char *GWB_BuildSubCmd_GetBuildMessage(const GWB_BUILD_SUBCMD *cmd)
{
  return cmd->buildMessage;
}



void GWB_BuildSubCmd_SetBuildMessage(GWB_BUILD_SUBCMD *cmd, const char *s)
{
  free(cmd->buildMessage);
  cmd->buildMessage=s?strdup(s):NULL;
}



void GWB_BuildSubCmd_toXml(const GWB_BUILD_SUBCMD *cmd, GWEN_XMLNODE *xmlNode)
{
  GWEN_XMLNode_SetIntValue(xmlNode, "flags", cmd->flags);

  if (cmd->command)
    GWEN_XMLNode_SetCharValue(xmlNode, "command", cmd->command);

  if (cmd->arguments)
    GWEN_XMLNode_SetCharValue(xmlNode, "arguments", cmd->arguments);

  if (cmd->buildMessage)
    GWEN_XMLNode_SetCharValue(xmlNode, "buildMessage", cmd->buildMessage);
}



GWB_BUILD_SUBCMD *GWB_BuildSubCmd_fromXml(GWEN_XMLNODE *xmlNode)
{
  GWB_BUILD_SUBCMD *cmd;
  const char *s;

  cmd=GWB_BuildSubCmd_new();

  cmd->flags=GWEN_XMLNode_GetIntValue(xmlNode, "flags", 0);

  s=GWEN_XMLNode_GetCharValue(xmlNode, "command", NULL);
  GWB_BuildSubCmd_SetCommand(cmd, s);

  s=GWEN_XMLNode_GetCharValue(xmlNode, "arguments", NULL);
  GWB_BuildSubCmd_SetArguments(cmd, s);

  s=GWEN_XMLNode_GetCharValue(xmlNode, "buildMessage", NULL);
  GWB_BuildSubCmd_SetBuildMessage(cmd, s);

  return cmd;
}




