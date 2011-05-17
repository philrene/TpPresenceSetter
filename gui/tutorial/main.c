#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <cairo.h>
#include <time.h>

gint count = 0;
char buf[5];
static char buffer[256];
void increase ( GtkWidget *widget, gpointer label )
{
	count ++;
	sprintf(buf, "%d", count);
	gtk_label_set_text(label, buf);
}

void decrease ( GtkWidget *widget, gpointer label) 
{
	count --;
	sprintf(buf, "%d", count );
	gtk_label_set_text( label, buf );
}

GdkPixbuf *create_pixbuf ( const gchar *filename ){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf =gdk_pixbuf_new_from_file( filename, &error );
	if ( !pixbuf ){
		g_warning ( "%s\n", error->message );
		g_error_free( error );
	}
	return pixbuf;
}

static gboolean
on_expose_event ( GtkWidget * widget,
		GdkEventExpose *event,
		gpointer data)
{
	cairo_t *cr;
	cr =gdk_cairo_create( widget->window);
	cairo_move_to(cr, 10, 10);
	cairo_show_text(cr, buffer);
	cairo_destroy(cr);
	return FALSE;
}

static gboolean
time_handler(GtkWidget *widget)
{
	if ( widget->window == NULL) return FALSE;
	time_t curtime;
	struct tm * loctime;
	curtime = time(NULL);
	loctime = localtime (&curtime);
	strftime( buffer, 256, "%T", loctime);

	gtk_widget_queue_draw(widget);
	return TRUE;
}


void show_about(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	
	gtk_about_dialog_set_name( GTK_ABOUT_DIALOG(dialog), "PresenceSetter");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.1a");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "(c) Phil René");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "This tool changes peridically your status on a telepathy-dbus system");
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}


int main( int argc, char * argv[] )
{
	GtkWidget * window;
	GtkWidget *label;
	GtkWidget *frame;
	GtkWidget *plus;
	GtkWidget *minus;

	GtkWidget * container;

	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *file;
	GtkWidget *preferences;
	GtkWidget *prefs_menu;
	GtkWidget *edit;
	GtkWidget *new;
	GtkWidget *open;
	GtkWidget *quit;
	GtkWidget *help_menu;
	GtkWidget *about;
	GtkWidget *help;

	GtkWidget *darea;


	GtkWidget *sep;
	GtkAccelGroup *accel_group = NULL;

	gtk_init ( &argc, &argv );

	window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title( GTK_WINDOW ( window ), "Phil's new gig");
	gtk_window_set_default_size ( GTK_WINDOW ( window ), 250, 200 );
	gtk_window_set_position ( GTK_WINDOW ( window ), GTK_WIN_POS_CENTER );
	gtk_window_set_icon ( GTK_WINDOW(window), create_pixbuf ( "icon.png" ) );


	darea = gtk_drawing_area_new();

	container = gtk_vbox_new(FALSE, 0);
	frame = gtk_fixed_new();
	vbox = gtk_vbox_new(FALSE, 0);
	menubar = gtk_menu_bar_new();
	filemenu= gtk_menu_new();
	prefs_menu= gtk_menu_new();
	help_menu = gtk_menu_new();


	gtk_box_pack_start(GTK_BOX(container), vbox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(container), darea, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(container), frame, TRUE,TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), container );
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW ( window ), accel_group );

	help = gtk_menu_item_new_with_label("Help");
	about = gtk_menu_item_new_with_label("About");
	file = gtk_menu_item_new_with_mnemonic("_File");
	new = gtk_image_menu_item_new_from_stock ( GTK_STOCK_NEW, NULL);
	open = gtk_image_menu_item_new_from_stock ( GTK_STOCK_OPEN, NULL);
	quit = gtk_image_menu_item_new_from_stock ( GTK_STOCK_QUIT, accel_group);
	sep = gtk_separator_menu_item_new();
	edit = gtk_menu_item_new_with_label("Edit");
	gtk_widget_set_size_request(edit, 90, 25 );
	preferences = gtk_menu_item_new_with_label("Preferences");

	gtk_widget_add_accelerator(quit, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(file), filemenu );
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(preferences), prefs_menu );
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(help), help_menu );
	gtk_menu_shell_append( GTK_MENU_SHELL(help_menu), about );
	gtk_menu_shell_append( GTK_MENU_SHELL(prefs_menu), edit );
	gtk_menu_shell_append( GTK_MENU_SHELL(filemenu), new );
	gtk_menu_shell_append( GTK_MENU_SHELL(filemenu), open );
	gtk_menu_shell_append( GTK_MENU_SHELL(filemenu), sep );
	gtk_menu_shell_append( GTK_MENU_SHELL(filemenu), quit );
	gtk_menu_shell_append( GTK_MENU_SHELL(menubar), file );
	gtk_menu_shell_append( GTK_MENU_SHELL(menubar), preferences );
	gtk_menu_shell_append( GTK_MENU_SHELL(menubar), help );
	gtk_box_pack_start ( GTK_BOX(vbox), menubar, FALSE, FALSE, 3);



	plus = gtk_button_new_with_label("+");
	gtk_widget_set_size_request ( plus, 80, 35 );
	gtk_fixed_put ( GTK_FIXED(frame), plus, 50, 20 );

	minus = gtk_button_new_with_label("-");
	gtk_widget_set_size_request ( minus, 80, 35 );
	gtk_fixed_put ( GTK_FIXED(frame), minus, 50, 80 );

	label = gtk_label_new("0");
	gtk_fixed_put ( GTK_FIXED(frame), label, 190,58);

	
	g_signal_connect ( G_OBJECT ( window), "destroy", G_CALLBACK(gtk_main_quit), NULL );
	g_signal_connect (plus, "clicked", G_CALLBACK(increase), label);
	g_signal_connect (minus, "clicked", G_CALLBACK(decrease), label);
	g_signal_connect (darea, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_signal_connect (G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT(about), "activate", G_CALLBACK(show_about), G_OBJECT(window));


	g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);

	gtk_widget_show_all( window );

	time_handler(window);
	gtk_main ();

	return 0;
}
