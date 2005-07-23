/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Sun Jun 13 2004
    copyright   : (C) 2004 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "tag16_p.h"
#include <gwenhywfar/debug.h>
#include <gwenhywfar/inherit.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/text.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>


GWEN_LIST_FUNCTIONS(GWEN_TAG16, GWEN_TAG16)


GWEN_TAG16 *GWEN_TAG16_new() {
  GWEN_TAG16 *tlv;

  GWEN_NEW_OBJECT(GWEN_TAG16, tlv);
  GWEN_LIST_INIT(GWEN_TAG16, tlv);

  return tlv;
}



void GWEN_TAG16_free(GWEN_TAG16 *tlv) {
  if (tlv) {
    free(tlv->tagData);
    GWEN_LIST_FINI(GWEN_TAG16, tlv);
    GWEN_FREE_OBJECT(tlv);
  }
}



unsigned int GWEN_TAG16_GetTagType(const GWEN_TAG16 *tlv){
  assert(tlv);
  return tlv->tagType;
}



unsigned int GWEN_TAG16_GetTagLength(const GWEN_TAG16 *tlv){
  assert(tlv);
  return tlv->tagLength;
}



unsigned int GWEN_TAG16_GetTagSize(const GWEN_TAG16 *tlv){
  assert(tlv);
  return tlv->tagSize;
}



const void *GWEN_TAG16_GetTagData(const GWEN_TAG16 *tlv){
  assert(tlv);
  return tlv->tagData;
}



GWEN_TAG16 *GWEN_TAG16_fromBuffer(GWEN_BUFFER *mbuf, int isBerTlv) {
  const char *p;
  unsigned int tagMode;
  unsigned int tagType;
  unsigned int tagLength;
  const char *tagData;
  unsigned int size;
  unsigned int pos;
  unsigned int j;
  GWEN_TAG16 *tlv;
  GWEN_TYPE_UINT32 startPos;

  if (!GWEN_Buffer_GetBytesLeft(mbuf)) {
    DBG_ERROR(0, "Buffer empty");
    return 0;
  }

  startPos=GWEN_Buffer_GetPos(mbuf);

  tagMode=tagType=tagLength=0;

  p=GWEN_Buffer_GetPosPointer(mbuf);
  pos=0;
  size=GWEN_Buffer_GetBytesLeft(mbuf);

  /* get tag type */
  if (size<2) {
    DBG_ERROR(0, "Too few bytes for BER-TLV");
    return 0;
  }
  j=(unsigned char)(p[pos]);
  tagType=j;

  /* get length */
  pos++;
  if (pos+1>=size) {
    DBG_ERROR(0, "Too few bytes");
    return 0;
  }
  j=((unsigned char)(p[pos+1]))<<8;
  j|=(unsigned char)(p[pos]);
  pos+=2;
  tagLength=j;
  tagData=p+pos;
  GWEN_Buffer_IncrementPos(mbuf, pos);

  tlv=GWEN_TAG16_new();
  assert(tlv);
  tlv->tagType=tagType;
  tlv->tagLength=tagLength;
  if (tagLength) {
    tlv->tagData=(void*)malloc(tagLength);
    memmove(tlv->tagData, tagData, tagLength);
  }

  GWEN_Buffer_IncrementPos(mbuf, tagLength);
  tlv->tagSize=GWEN_Buffer_GetPos(mbuf)-startPos;
  return tlv;
}



void GWEN_TAG16_DirectlyToBuffer(unsigned int tagType,
                                 const char *p,
                                 int size,
                                 GWEN_BUFFER *buf){
  assert(buf);
  if (size==-1) {
    assert(p);
    size=strlen(p);
  }

  GWEN_Buffer_AppendByte(buf, tagType & 0xff);
  GWEN_Buffer_AppendByte(buf, size & 0xff);
  GWEN_Buffer_AppendByte(buf, (size>>8)&0xff);
  if (size) {
    assert(p);
    GWEN_Buffer_AppendBytes(buf, p, size);
  }

}








