#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

            if (interface && member &&
                strcmp(interface, "org.example.HelloService") == 0 &&
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
            } else if (interface && member &&
                       strcmp(interface, "org.example.HelloService") == 0 &&
                       strcmp(member, "Ping") == 0) {

                printf("Received Ping call\n");

                const char* reply_str = "Pong";
                DBusMessage* reply = dbus_message_new_method_return(msg);
                dbus_message_append_args(reply,
                                         DBUS_TYPE_STRING, &reply_str,
                                         DBUS_TYPE_INVALID);
                dbus_connection_send(conn, reply, NULL);
                dbus_connection_flush(conn);
                dbus_message_unref(reply);
            }
        }
        dbus_message_unref(msg);
    }
    return 0;
}