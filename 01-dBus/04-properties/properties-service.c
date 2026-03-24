#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int counter = 0;

int main(void)
{
    DBusError err;
    DBusConnection* conn;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (!conn) {
        fprintf(stderr, "Failed to connect: %s\n", err.message);
        exit(1);
    }

    int ret = dbus_bus_request_name(conn,
                                    "org.example.HelloService",
                                    DBUS_NAME_FLAG_REPLACE_EXISTING,
                                    &err);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Failed to acquire name\n");
        exit(1);
    }

    printf("org.example.HelloService running...\n");

    while (1) {
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if (!msg) {
            usleep(100000);
            continue;
        }

        if (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_CALL) {
            const char* interface = dbus_message_get_interface(msg);
            const char* member = dbus_message_get_member(msg);

            if (interface && member) {
                if (strcmp(interface, "org.example.HelloService") == 0 &&
                    strcmp(member, "SayHello") == 0) {

                    printf("Received SayHello call\n");

                    const char* reply_str = "Hello";
                    DBusMessage* reply = dbus_message_new_method_return(msg);
                    dbus_message_append_args(reply,
                                             DBUS_TYPE_STRING, &reply_str,
                                             DBUS_TYPE_INVALID);
                    dbus_connection_send(conn, reply, NULL);
                    dbus_connection_flush(conn);
                    dbus_message_unref(reply);
                } else if (strcmp(interface, "org.freedesktop.DBus.Properties") == 0) {
                    if (strcmp(member, "Get") == 0) {
                        const char* prop_interface;
                        const char* prop_name;

                        DBusMessageIter args;
                        dbus_message_iter_init(msg, &args);
                        dbus_message_iter_get_basic(&args, &prop_interface);
                        dbus_message_iter_next(&args);
                        dbus_message_iter_get_basic(&args, &prop_name);

                        if (strcmp(prop_interface, "org.example.HelloService") == 0 &&
                            strcmp(prop_name, "Counter") == 0) {

                            printf("Received Get Counter\n");

                            DBusMessage* reply = dbus_message_new_method_return(msg);
                            DBusMessageIter reply_iter;
                            dbus_message_iter_init_append(reply, &reply_iter);
                            dbus_message_iter_append_basic(&reply_iter, DBUS_TYPE_VARIANT, &counter);
                            dbus_connection_send(conn, reply, NULL);
                            dbus_connection_flush(conn);
                            dbus_message_unref(reply);
                        }
                    } else if (strcmp(member, "GetAll") == 0) {
                        const char* prop_interface;

                        DBusMessageIter args;
                        dbus_message_iter_init(msg, &args);
                        dbus_message_iter_get_basic(&args, &prop_interface);

                        if (strcmp(prop_interface, "org.example.HelloService") == 0) {

                            printf("Received GetAll\n");

                            DBusMessage* reply = dbus_message_new_method_return(msg);
                            DBusMessageIter reply_iter, array_iter;
                            dbus_message_iter_init_append(reply, &reply_iter);
                            dbus_message_iter_open_container(&reply_iter, DBUS_TYPE_ARRAY, "{sv}", &array_iter);

                            // Add Counter property
                            DBusMessageIter dict_iter, variant_iter;
                            dbus_message_iter_open_container(&array_iter, DBUS_TYPE_DICT_ENTRY, NULL, &dict_iter);
                            const char* key = "Counter";
                            dbus_message_iter_append_basic(&dict_iter, DBUS_TYPE_STRING, &key);
                            dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_VARIANT, "i", &variant_iter);
                            dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_INT32, &counter);
                            dbus_message_iter_close_container(&dict_iter, &variant_iter);
                            dbus_message_iter_close_container(&array_iter, &dict_iter);

                            dbus_message_iter_close_container(&reply_iter, &array_iter);
                            dbus_connection_send(conn, reply, NULL);
                            dbus_connection_flush(conn);
                            dbus_message_unref(reply);
                        }
                    } else if (strcmp(member, "Set") == 0) {
                        const char* prop_interface;
                        const char* prop_name;
                        int new_value;

                        DBusMessageIter args;
                        dbus_message_iter_init(msg, &args);
                        dbus_message_iter_get_basic(&args, &prop_interface);
                        dbus_message_iter_next(&args);
                        dbus_message_iter_get_basic(&args, &prop_name);
                        dbus_message_iter_next(&args);

                        DBusMessageIter variant_iter;
                        dbus_message_iter_recurse(&args, &variant_iter);
                        dbus_message_iter_get_basic(&variant_iter, &new_value);

                        if (strcmp(prop_interface, "org.example.HelloService") == 0 &&
                            strcmp(prop_name, "Counter") == 0) {

                            printf("Received Set Counter to %d\n", new_value);
                            counter = new_value;

                            DBusMessage* reply = dbus_message_new_method_return(msg);
                            dbus_connection_send(conn, reply, NULL);
                            dbus_connection_flush(conn);
                            dbus_message_unref(reply);
                        }
                    }
                }
            }
        }
        dbus_message_unref(msg);
    }
    return 0;
}