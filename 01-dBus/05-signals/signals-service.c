#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int counter = 0;

void emit_counter_changed(DBusConnection* conn, int new_value) {
    DBusMessage* signal = dbus_message_new_signal(
        "/",                    // object path
        "org.example.HelloService", // interface
        "CounterChanged"        // signal name
    );

    if (!signal) {
        fprintf(stderr, "Failed to create signal\n");
        return;
    }

    dbus_message_append_args(signal,
                             DBUS_TYPE_INT32, &new_value,
                             DBUS_TYPE_INVALID);

    dbus_connection_send(conn, signal, NULL);
    dbus_connection_flush(conn);
    dbus_message_unref(signal);

    printf("Emitted CounterChanged signal with value %d\n", new_value);
}

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
                strcmp(interface, "org.example.HelloService") == 0) {

                if (strcmp(member, "IncrementCounter") == 0) {
                    counter++;
                    printf("Incremented counter to %d\n", counter);

                    emit_counter_changed(conn, counter);

                    DBusMessage* reply = dbus_message_new_method_return(msg);
                    dbus_connection_send(conn, reply, NULL);
                    dbus_connection_flush(conn);
                    dbus_message_unref(reply);
                }
            }
        }
        dbus_message_unref(msg);
    }
    return 0;
}