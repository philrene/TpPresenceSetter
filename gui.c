#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <time.h>

#define PERIOD 5000
gint count = 0;
char buf[5];
static char buffer[256];
enum
{
	LIST_ITEM=0,
	IDX_ITEM,
	N_COLUMNS
};

GtkWidget *list;
GtkWidget *add_wnd;

GtkTreeIter current;
int incIdx=0;
void add_to_list_cb ( GtkWidget *widget, gpointer view );
void destroy_add_wnd ( );

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

void destroy_add_wnd (  ){
	gtk_widget_destroy ( GTK_WINDOW( add_wnd ));
}
/*
 * Creating the window to add a status to the list
 */

void show_add_to_list ( GtkWidget *widget, gpointer list)
{
	GtkWidget *container;
	GtkWidget *view_window;
        GtkWidget *view;
	GtkWidget *add_wnd_add_button;
	GtkWidget *add_wnd_cancel_button;

	view = gtk_text_view_new();	
	gtk_text_view_set_wrap_mode( view, GTK_WRAP_WORD );

	add_wnd = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	view_window = gtk_scrolled_window_new ( NULL, NULL );
	gtk_scrolled_window_set_policy ( view_window, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC ); 
	gtk_window_set_title( GTK_WINDOW ( add_wnd ), "Add a status");
	gtk_window_set_modal( add_wnd, TRUE);
	gtk_window_set_default_size ( GTK_WINDOW ( add_wnd ), 600, 300 );
	gtk_window_set_position ( GTK_WINDOW ( add_wnd ), GTK_WIN_POS_CENTER );
	gtk_window_set_icon ( GTK_WINDOW( add_wnd ), create_pixbuf ( "icon.png" ) );



	container = gtk_vbox_new ( FALSE, 0 );
	gtk_box_pack_start(GTK_BOX(container), view_window, TRUE, TRUE, 0);

	add_wnd_add_button = gtk_button_new_with_label("Add Status");
	gtk_widget_set_size_request ( add_wnd_add_button, 80, 35 );
	gtk_box_pack_start ( GTK_BOX(container), add_wnd_add_button, FALSE, FALSE, 3 );
	add_wnd_cancel_button = gtk_button_new_with_label("Cancel");
	gtk_widget_set_size_request ( add_wnd_cancel_button, 80, 35 );
	gtk_box_pack_start ( GTK_BOX(container), add_wnd_cancel_button, FALSE, FALSE, 3 );


	g_signal_connect (add_wnd_add_button, "clicked", G_CALLBACK(add_to_list_cb), view);
	g_signal_connect (add_wnd_cancel_button, "clicked", destroy_add_wnd, NULL);

	gtk_container_add(GTK_CONTAINER(view_window), view );
	gtk_container_add(GTK_CONTAINER(add_wnd), container );
	
	gtk_widget_show_all( add_wnd );
}


static gboolean
time_handler(GtkWidget *selection)
{

	fprintf(stderr,"Being called my friend\n");

	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter iter;

	store = GTK_LIST_STORE ( gtk_tree_view_get_model( GTK_TREE_VIEW(list)));
	model = gtk_tree_view_get_model(GTK_TREE_VIEW ( list ));

	if ( gtk_tree_model_get_iter_first ( model, &iter ) == FALSE)
		return TRUE; // nothing to do there is nothing in the queue

	if ( ! gtk_tree_model_iter_next  ( model, &current ) ){
		gtk_tree_model_get_iter_first ( model, &iter );
		current = iter;
	}


	GValue *itemvalue;

	g_type_init();
	itemvalue=g_new0(GValue, 1);

	gtk_tree_model_get_value( model, &current, LIST_ITEM, itemvalue );

	set_presence ( itemvalue, 0 );

	g_print("yeah man &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

	/*g_printf("YYYYYYYYYYAAAAAAAAAAAAA  %s\n", g_value_get_string(itemvalue));*/
	/*itemvalue=g_new0(GValue, 1);*/
	/*gtk_tree_model_get_value( model, &current, IDX_ITEM, itemvalue );*/

	/*[>g_printf("BOooooooAAAA  %u\n", g_value_get_int(itemvalue));<]*/




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

static void
init_list( GtkWidget * list)
{
	GtkCellRendererText *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_fixed_size ( renderer, 200, -1 );
	column = gtk_tree_view_column_new_with_attributes("List of statuses", renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_INT);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
	gtk_tree_view_set_enable_search( GTK_TREE_VIEW(list), FALSE );
	gtk_tree_view_set_reorderable( GTK_TREE_VIEW(list), TRUE );

	g_object_unref(store);
}

static void
add_to_list(GtkWidget *list, const gchar *str)
{
	  GtkListStore *store;
	  GtkTreeIter iter;
	  store = GTK_LIST_STORE(gtk_tree_view_get_model
			            (GTK_TREE_VIEW(list)));
	  gtk_list_store_append(store, &iter);
	  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
	  gtk_list_store_set(store, &iter, IDX_ITEM, incIdx++ , -1);
}


void add_to_list_cb ( GtkWidget *widget, gpointer view )
{
 	GtkTextBuffer * buffer;
	buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW (view) );

	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);


	const gchar *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	add_to_list ( list, str ); 


	destroy_add_wnd();
}

void remove_from_list ( GtkWidget *widget, gpointer selection )
{
	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter iter;

	store = GTK_LIST_STORE ( gtk_tree_view_get_model( GTK_TREE_VIEW(list)));
	model = gtk_tree_view_get_model(GTK_TREE_VIEW ( list ));

	if ( gtk_tree_model_get_iter_first ( model, &iter ) == FALSE)
		return;

	if ( gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)){
		gtk_list_store_remove(store, &iter );
	}
}

int start_gui(  )
{
	GtkWidget *window;
	GtkWidget *list_window;
	GtkWidget *bottom;
	GtkWidget *add;
	GtkWidget *remove;

	GtkWidget * container;

	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *file;
	GtkWidget *quit;
	GtkWidget *help_menu;
	GtkWidget *about;
	GtkWidget *help;


	GtkTreeSelection *list_selection;
	GtkWidget *bottom_separator;

	GtkWidget *darea;


	GtkAccelGroup *accel_group = NULL;

	gtk_init ( NULL, NULL );
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible( GTK_TREE_VIEW(list), FALSE );
	list_selection  = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	init_list( list );

	window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	list_window = gtk_scrolled_window_new ( NULL, NULL );
	gtk_scrolled_window_set_policy ( list_window, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC );
	gtk_window_set_title( GTK_WINDOW ( window ), "Phil's new gig");
	gtk_window_set_default_size ( GTK_WINDOW ( window ), 600, 500 );
	gtk_window_set_position ( GTK_WINDOW ( window ), GTK_WIN_POS_CENTER );
	gtk_window_set_icon ( GTK_WINDOW(window), create_pixbuf ( "icon.png" ) );


	darea = gtk_drawing_area_new();


	// initialization menu items
	container = gtk_vbox_new(FALSE, 0);
	bottom = gtk_hbox_new(FALSE, 0);
	vbox = gtk_vbox_new(FALSE, 0);
	menubar = gtk_menu_bar_new();
	filemenu= gtk_menu_new();
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW ( window ), accel_group );
	help_menu = gtk_menu_new();

	file = gtk_menu_item_new_with_mnemonic("_File");
	help = gtk_menu_item_new_with_label("Help");
	about = gtk_menu_item_new_with_label("About");
	quit = gtk_image_menu_item_new_from_stock ( GTK_STOCK_QUIT, accel_group);

	gtk_widget_add_accelerator(quit, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(help), help_menu );
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(file), filemenu );
	gtk_menu_shell_append( GTK_MENU_SHELL(help_menu), about );
	gtk_menu_shell_append( GTK_MENU_SHELL(filemenu), quit );
	gtk_menu_shell_append( GTK_MENU_SHELL(menubar), file );
	gtk_menu_shell_append( GTK_MENU_SHELL(menubar), help );
	gtk_box_pack_start ( GTK_BOX(vbox), menubar, FALSE, FALSE, 3);

	gtk_container_add(GTK_CONTAINER(window), container );
	gtk_container_add(GTK_CONTAINER(list_window), list );
	
	// END init menu 


	bottom_separator = gtk_hseparator_new();

	add = gtk_button_new_with_label("Add");
	gtk_widget_set_size_request ( add, 80, 35 );
	gtk_box_pack_start ( GTK_BOX(bottom), add, FALSE, FALSE, 3 );

	remove = gtk_button_new_with_label("Remove");
	gtk_widget_set_size_request ( remove, 80, 35 );
	gtk_box_pack_start ( GTK_BOX(bottom), remove, FALSE, FALSE, 3 );


	gtk_widget_set_size_request( bottom, 50, 50 );

	
	g_signal_connect ( G_OBJECT ( window), "destroy", G_CALLBACK(gtk_main_quit), NULL );
	g_signal_connect (add, "clicked", G_CALLBACK(show_add_to_list), window);
	g_signal_connect (remove, "clicked", G_CALLBACK(remove_from_list),list_selection);
	g_signal_connect (G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT(about), "activate", G_CALLBACK(show_about), G_OBJECT(window));

	gtk_box_pack_start(GTK_BOX(container), vbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(container), list_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(container), bottom_separator, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(container), bottom, FALSE,FALSE, 0);

	gtk_container_set_border_width (GTK_CONTAINER (bottom), 10);

	g_timeout_add(PERIOD, (GSourceFunc) time_handler, (gpointer) list_selection);

	gtk_widget_show_all( window );

	// just for chilling

	time_handler(list);
	gtk_main ();

	return 0;
}
