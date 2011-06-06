/*
 * telepathy-example-list-managers - list installed connection managers
 *
 * Copyright (C) 2008 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2008 Nokia Corporation
 *
 * Copying and distribution of this file, with or without modification,
 * are permitted in any medium without royalty provided the copyright
 * notice and this notice are preserved.
 */

#include <telepathy-glib/telepathy-glib.h>
#include "setpresence.h"
typedef struct {
    GMainLoop *mainloop;
    int exit_code;
    TpDBusDaemon *bus_daemon;
    GError *error;
    GValue *str;
} ExampleData;


static void connection_is_ready_cb ( TpConnection * connection,
					const GError *error,
					gpointer user_data ){
	ExampleData * data = user_data;

	g_print ( "Yo connection is ready ");

	gchar * hum = g_value_get_string(data->str);
//	g_print("******************************** %s\n", hum);
	g_print ( "Yo connection is ready HEre my frind ");
	tp_cli_connection_interface_simple_presence_call_set_presence ( connection,	
									1000,
									"available",
									hum,
									NULL,
									NULL,
	       								NULL,
									NULL	);

	//quit(data);
}
	

static void
got_connections (const gchar * const *bus_names,
                 gsize n,
                 const gchar * const *cms,
                 const gchar * const *protocols,
                 const GError *error,
                 gpointer user_data,
                 GObject *unused)
{

	g_message("Dude start of got_connections");
  ExampleData *data = user_data;

  if (error != NULL)
    {
      g_warning ("%s", error->message);
      data->exit_code = 1;
    }
  else
    {
      gsize i;

      g_message ("Found %" G_GSIZE_FORMAT " connections:", n);

      for (i = 0; i < n; i++)
	{
	  g_message ("%s", bus_names[i]);
	 g_message ("- CM %s, protocol %s", cms[i], protocols[i]);
        }

       TpConnection * connection = NULL;

      g_print( "BeforeDude where is my car\n"); 
      connection = tp_connection_new ( data->bus_daemon, bus_names[0],NULL, &data->error );
      g_print( "Dude where is my car\n"); 
       if ( connection == NULL ){
	       g_print("Yo dawg\n");
//	       g_warning ("Phil:: %s", data->error->message );
	       g_print("Yo dawg2\n");
//	       g_error_free( data->error );
	       quit(data);
       }
       g_print("my fried this looks nice\n");
       tp_connection_call_when_ready ( connection, connection_is_ready_cb, data );

      
       g_print("my fried this looks nice\n");

      /* all the arrays are also NULL-terminated */
      g_assert (bus_names[n] == NULL && cms[n] == NULL &&
          protocols[n] == NULL);
    }

       g_print("my fried this looks nice the 3rd\n");
//  g_main_loop_quit (data->mainloop);
}

int quit ( gpointer user_data){

	g_debug("Am I being call???");

ExampleData * data = user_data;
 if (data->mainloop != NULL)
    g_main_loop_unref (data->mainloop);

  if (data->bus_daemon != NULL)
    g_object_unref (data->bus_daemon);
g_print ( "Quitting everything my dear\n");
   return data->exit_code;
}




int
set_presence( GValue *value, int loop )
{

  g_type_init ();
  ExampleData data = { g_main_loop_new (NULL, FALSE), 0, NULL, NULL, value };

  g_message("To all my loyal subject %s\n", g_value_get_string(value));

  tp_debug_set_flags (g_getenv ("EXAMPLE_DEBUG"));
 data.bus_daemon = tp_dbus_daemon_dup (&data.error);

  if (data.bus_daemon == NULL)
    {
      g_warning ("%s", data.error->message);
      g_error_free (data.error);
      data.exit_code = 1;
      /*goto out;*/
    }

  tp_list_connection_names (data.bus_daemon, got_connections, &data, NULL, NULL);
 if ( loop )
	 g_main_loop_run (data.mainloop);

  return 1;
}
