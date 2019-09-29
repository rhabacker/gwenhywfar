/***************************************************************************
    begin       : Sun May 16 2010
    copyright   : (C) 2010 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/





static GWENHYWFAR_CB
int Gtk2Gui_WGroupBox_SetIntProperty(GWEN_WIDGET *w,
                                     GWEN_DIALOG_PROPERTY prop,
                                     int index,
                                     int value,
                                     int doSignal)
{
  GtkWidget *g;

  g=GTK_WIDGET(GWEN_Widget_GetImplData(w, GTK2_DIALOG_WIDGET_REAL));
  assert(g);

  switch (prop) {
  case GWEN_DialogProperty_Enabled:
    gtk_widget_set_sensitive(GTK_WIDGET(g), (value==0)?FALSE:TRUE);
    return 0;

  case GWEN_DialogProperty_Focus:
    gtk_widget_grab_focus(GTK_WIDGET(g));
    return 0;

  case GWEN_DialogProperty_Width:
  case GWEN_DialogProperty_Height:
    /* just ignore these for now */
    return 0;

  default:
    break;
  }

  DBG_WARN(GWEN_LOGDOMAIN,
           "Function is not appropriate for this type of widget (%s)",
           GWEN_Widget_Type_toString(GWEN_Widget_GetType(w)));
  return GWEN_ERROR_INVALID;
}




static GWENHYWFAR_CB
int Gtk2Gui_WGroupBox_GetIntProperty(GWEN_WIDGET *w,
                                     GWEN_DIALOG_PROPERTY prop,
                                     int index,
                                     int defaultValue)
{
  GtkWidget *g;

  g=GTK_WIDGET(GWEN_Widget_GetImplData(w, GTK2_DIALOG_WIDGET_REAL));
  assert(g);

  switch (prop) {
  case GWEN_DialogProperty_Enabled:
    return (gtk_widget_get_sensitive(GTK_WIDGET(g))==TRUE)?1:0;

  case GWEN_DialogProperty_Focus:
    return (gtk_widget_has_focus(GTK_WIDGET(g))==TRUE)?1:0;
    return 0;

  case GWEN_DialogProperty_Width:
  case GWEN_DialogProperty_Height:
    /* just ignore these for now */
    return 0;

  default:
    break;
  }

  DBG_WARN(GWEN_LOGDOMAIN,
           "Function is not appropriate for this type of widget (%s)",
           GWEN_Widget_Type_toString(GWEN_Widget_GetType(w)));
  return defaultValue;
}



static GWENHYWFAR_CB
int Gtk2Gui_WGroupBox_SetCharProperty(GWEN_WIDGET *w,
                                      GWEN_DIALOG_PROPERTY prop,
                                      int index,
                                      const char *value,
                                      int doSignal)
{
  GtkWidget *g;

  g=GTK_WIDGET(GWEN_Widget_GetImplData(w, GTK2_DIALOG_WIDGET_REAL));
  assert(g);

  switch (prop) {
  case GWEN_DialogProperty_Title:
    gtk_frame_set_label(GTK_FRAME(g), value);
    return 0;
  default:
    break;
  }

  DBG_WARN(GWEN_LOGDOMAIN,
           "Function is not appropriate for this type of widget (%s)",
           GWEN_Widget_Type_toString(GWEN_Widget_GetType(w)));
  return GWEN_ERROR_INVALID;
}



static GWENHYWFAR_CB
const char *Gtk2Gui_WGroupBox_GetCharProperty(GWEN_WIDGET *w,
                                              GWEN_DIALOG_PROPERTY prop,
                                              int index,
                                              const char *defaultValue)
{
  GtkWidget *g;

  g=GTK_WIDGET(GWEN_Widget_GetImplData(w, GTK2_DIALOG_WIDGET_REAL));
  assert(g);

  switch (prop) {
  case GWEN_DialogProperty_Title:
    return gtk_frame_get_label(GTK_FRAME(g));
  default:
    break;
  }

  DBG_WARN(GWEN_LOGDOMAIN,
           "Function is not appropriate for this type of widget (%s)",
           GWEN_Widget_Type_toString(GWEN_Widget_GetType(w)));
  return defaultValue;
}



static GWENHYWFAR_CB
int Gtk2Gui_WGroupBox_AddChildGuiWidget(GWEN_WIDGET *w, GWEN_WIDGET *wChild)
{
  GtkWidget *g;
  GtkWidget *gChild;
  uint32_t cflags;

  g=GTK_WIDGET(GWEN_Widget_GetImplData(w, GTK2_DIALOG_WIDGET_CONTENT));
  assert(g);

  gChild=GTK_WIDGET(GWEN_Widget_GetImplData(wChild, GTK2_DIALOG_WIDGET_REAL));
  assert(gChild);

  cflags=GWEN_Widget_GetFlags(wChild);

  gtk_box_pack_start(GTK_BOX(g), gChild,
                     (cflags & GWEN_WIDGET_FLAGS_FILLY)?TRUE:FALSE,
                     (cflags & GWEN_WIDGET_FLAGS_FILLY)?TRUE:FALSE,
                     0);

  return 0;
}



int Gtk2Gui_WGroupBox_Setup(GWEN_WIDGET *w)
{
  GtkWidget *g;
  GtkWidget *gContent;
  const char *s;
  /*uint32_t flags;*/
  GWEN_WIDGET *wParent;

  /*flags=GWEN_Widget_GetFlags(w);*/
  wParent=GWEN_Widget_Tree_GetParent(w);
  s=GWEN_Widget_GetText(w, 0);

  g=gtk_frame_new(s);

  gContent=gtk_vbox_new(TRUE, GTK2_GUI_DIALOG_DEFAULT_BOX_SPACING);
  gtk_container_add(GTK_CONTAINER(g), gContent);

  GWEN_Widget_SetImplData(w, GTK2_DIALOG_WIDGET_REAL, (void *) g);
  GWEN_Widget_SetImplData(w, GTK2_DIALOG_WIDGET_CONTENT, (void *) gContent);

  GWEN_Widget_SetSetIntPropertyFn(w, Gtk2Gui_WGroupBox_SetIntProperty);
  GWEN_Widget_SetGetIntPropertyFn(w, Gtk2Gui_WGroupBox_GetIntProperty);
  GWEN_Widget_SetSetCharPropertyFn(w, Gtk2Gui_WGroupBox_SetCharProperty);
  GWEN_Widget_SetGetCharPropertyFn(w, Gtk2Gui_WGroupBox_GetCharProperty);
  GWEN_Widget_SetAddChildGuiWidgetFn(w, Gtk2Gui_WGroupBox_AddChildGuiWidget);

  if (wParent)
    GWEN_Widget_AddChildGuiWidget(wParent, w);

  return 0;
}


