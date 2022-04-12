/**
 * Demo GTK+ Application that illustrates data entry.
 *
 * M. Horauer
 */
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stddef.h>
#include "../../wrapper/src/public/finclip_wrapper.h"

typedef struct {
  GtkWidget *dateDate;
  GtkWidget *idNbr;
  GtkWidget *emailAddr;
  GtkWidget *gnameEntry;
  GtkWidget *fnameEntry;
  GtkWidget *streetEntry;
  GtkWidget *cityEntry;
  GtkWidget *zipEntry;
  GtkWidget *phoneEntry;
  GtkWidget *byearSpin;
  GtkWidget *bmonthSpin;
  GtkWidget *bdaySpin;
} diaWidgets;

typedef struct {
  GtkApplication *app;
  GtkWidget *window;
  GtkWidget *img;
  diaWidgets *d;
} appWidgets;

/***************************************************************** PROTOTYPES */
static void activate(GtkApplication *app, gpointer user_data);
static void cancel_callback(GtkWidget *widget, gpointer user_data);
static void clear_callback(GtkWidget *widget, gpointer user_data);
static void add_callback(GtkWidget *widget, gpointer user_data);
static gint get_next_id(void);
static gint get_next_id(void) { return 12; }

/************************************************************ Cancel Callback */
static void cancel_callback(GtkWidget *widget, gpointer user_data) {}

/************************************************************* Clear Callback */
static void clear_callback(GtkWidget *widget, gpointer user_data) {
  finclip_close_all_applet();
}

/*************************************************************** Add Callback */
static void add_callback(GtkWidget *widget, gpointer user_data) {
  IPackerFactory *factory = finclip_get_packer_factory();
  IFinConfigPacker *packer = finclip_packer_factory_get_config_packer(factory);
  IFinConfig *config = finclip_config_packer_new_config(packer);
  finclip_config_set_app_store(config, 1);
  finclip_config_set_app_key(config,
                             "22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=");
  finclip_config_set_secret(config, "ae55433be2f62915");
  finclip_config_set_domain(config, "https://finchat-mop-b.finogeeks.club");
  finclip_config_packer_add_config(packer, config);
  finclip_initialize(packer);
  finclip_start_applet(NULL, 1, "60e3c059949a5300014d0c07", "", NULL, "", NULL);
}

/********************************************************* nameentry_callback */
static void nameentry_callback(GtkWidget *widget, gpointer user_data) {
  gint study_prog_nr = 54;
  gchar *org = "uni";
  gchar *cnt = "net";
  gchar *gname;
  gchar *fname;
  gchar email[256];
  gchar dateStamp[256];
  gchar *year;
  gchar id[256] = "";
  appWidgets *a = (appWidgets *)user_data;

  /* construct the eMail address */
  gname = (gchar *)gtk_entry_get_text(GTK_ENTRY(a->d->gnameEntry));
  fname = (gchar *)gtk_entry_get_text(GTK_ENTRY(a->d->fnameEntry));
  /* we update the fields on the top only when we got a family name */
  if ((g_strcmp0(gname, "") != 0) && (g_strcmp0(fname, "") != 0)) {
    g_sprintf(email, "%s.%s@%s.%s", gname, fname, org, cnt);
    gtk_label_set_label(GTK_LABEL(a->d->emailAddr), email);
    /* update date info when Family Name was entered */
    gtk_label_set_label(GTK_LABEL(a->d->dateDate), dateStamp);
    /* construct matrnum when Family Name was entered */
    year = g_strndup(dateStamp, 4);
    g_snprintf(id, 10, "%s%d%03d", year, study_prog_nr, get_next_id());
    gtk_label_set_label(GTK_LABEL(a->d->idNbr), id);
  }
}

/***************************************************************** ADD WINDOW */
static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *box;
  GtkWidget *ebox;
  GtkWidget *grid;
  GtkWidget *date_label;
  GtkWidget *id_label;
  GtkWidget *email_label;
  GtkWidget *gname_label;
  GtkWidget *fname_label;
  GtkWidget *street_label;
  GtkWidget *city_label;
  GtkWidget *zip_label;
  GtkWidget *phone_label;
  GtkWidget *birth_label;
  GtkWidget *c_button;
  GtkWidget *l_button;
  GtkWidget *a_button;
  appWidgets *a = (appWidgets *)user_data;

  /* create a window with title, default size,and icons */
  a->window = gtk_application_window_new(a->app);
  gtk_window_set_application(GTK_WINDOW(a->window), GTK_APPLICATION(a->app));
  gtk_window_set_title(GTK_WINDOW(a->window), "Student Management Toolbox");
  gtk_window_set_default_size(GTK_WINDOW(a->window), 400, 300);
  gtk_window_set_resizable(GTK_WINDOW(a->window), FALSE);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
  gtk_container_add(GTK_CONTAINER(a->window), GTK_WIDGET(box));
  gtk_container_set_border_width(GTK_CONTAINER(a->window), 10);

  /* grid: image and labels */
  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_widget_set_size_request(GTK_WIDGET(grid), 400, 90);
  gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
  gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(grid), TRUE, TRUE, 0);

  gname_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "Appid", "xalign", 1.0,
                               "yalign", 0.5, NULL);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(gname_label), 0, 3, 1, 1);
  a->d->gnameEntry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(a->d->gnameEntry), 1, 3, 1, 1);

  street_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "secret:", "xalign",
                                1.0, "yalign", 0.5, NULL);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(street_label), 0, 4, 1, 1);
  a->d->streetEntry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(a->d->streetEntry), 1, 4, 1, 1);

  zip_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "key:", "xalign", 1.0,
                             "yalign", 0.5, NULL);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(zip_label), 0, 5, 1, 1);
  a->d->zipEntry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(a->d->zipEntry), 1, 5, 1, 1);

  phone_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "domain:", "xalign",
                               1.0, "yalign", 0.5, NULL);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(phone_label), 2, 5, 1, 1);
  a->d->phoneEntry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(a->d->phoneEntry), 3, 5, 1, 1);

  /* lowerbox: buttons */
  c_button = gtk_button_new_with_mnemonic("_Cancel");
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(c_button), 1, 7, 1, 1);
  g_signal_connect(G_OBJECT(c_button), "clicked", G_CALLBACK(cancel_callback),
                   (gpointer)a);
  l_button = gtk_button_new_with_mnemonic("C_lear");
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_button), 2, 7, 1, 1);
  g_signal_connect(G_OBJECT(l_button), "clicked", G_CALLBACK(clear_callback),
                   (gpointer)a);
  a_button = gtk_button_new_with_mnemonic("_Add");
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(a_button), 3, 7, 1, 1);
  g_signal_connect(G_OBJECT(a_button), "clicked", G_CALLBACK(add_callback),
                   (gpointer)a);

  gtk_widget_show_all(GTK_WIDGET(a->window));
}

/*********************************************************************** main */
int main(int argc, char **argv) {
  int status;
  appWidgets *a = g_malloc(sizeof(appWidgets));
  a->d = g_malloc(sizeof(diaWidgets));

  a->app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(G_OBJECT(a->app), "activate", G_CALLBACK(activate),
                   (gpointer)a);
  status = g_application_run(G_APPLICATION(a->app), argc, argv);
  g_object_unref(a->app);

  g_free(a->d);
  g_free(a);
  return status;
}
/** EOF */