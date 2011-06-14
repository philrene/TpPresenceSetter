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
    int num;
} ExampleData;


static void connection_is_ready_cb ( TpConnection * connection,
					const GError *error,
					gpointer user_data ){
	ExampleData * data = user_data;

	gchar * hum = g_value_get_string(data->str);
	tp_cli_connection_interface_simple_presence_call_set_presence ( connection,	
									1000,
									"available",
									hum,
									NULL,
									NULL,
	       								NULL,
									NULL	);

	if(--data->num == 0){
		quit(data);
	}

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

  ExampleData *data = user_data;

  if (error != NULL)
    {
      g_warning ("%s", error->message);
      data->exit_code = 1;
    }
  else
    {
      gsize i;


	TpConnection * connection = NULL;

	int idx[10];
	int numIdx=0;

      for (i = 0; i < n; i++)
	{
	 if (strstr ( bus_names[i], "gabble") ){
		idx[numIdx++]=i;	
	 }
	}
	
      data->num = numIdx;
for( i=0; i < numIdx; i++){	 
      connection = tp_connection_new ( data->bus_daemon, bus_names[idx[i]],NULL, &data->error );

       if ( connection == NULL ){
	       data->num--;
       }
       tp_connection_call_when_ready ( connection, connection_is_ready_cb, data );
} 
       
      /* all the arrays are also NULL-terminated */
      g_assert (bus_names[n] == NULL && cms[n] == NULL &&
          protocols[n] == NULL);
    }
}

int quit ( gpointer user_data){

ExampleData * data = user_data;
  g_main_loop_quit (data->mainloop);
 if (data->mainloop != NULL)
    g_main_loop_unref (data->mainloop);

  if (data->bus_daemon != NULL)
    g_object_unref (data->bus_daemon);
   return data->exit_code;
}




int
set_presence( GValue *value, int loop )
{

  g_type_init ();
  ExampleData data = { g_main_loop_new (NULL, FALSE), 0, NULL, NULL, value, 0 };

  tp_debug_set_flags (g_getenv ("EXAMPLE_DEBUG"));
 data.bus_daemon = tp_dbus_daemon_dup (&data.error);

  if (data.bus_daemon == NULL)
    {
      g_warning ("%s", data.error->message);
      g_error_free (data.error);
      data.exit_code = 1;
    }

  tp_list_connection_names (data.bus_daemon, got_connections, &data, NULL, NULL);
 if ( loop )
	 g_main_loop_run (data.mainloop);

  return 1;
}
