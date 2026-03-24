# Generic dBus Terminal Commands

## List Services

List all services on the user bus:

```bash
busctl --user list
```

List all services on the system bus:

```bash
busctl --system list
```

## Introspect a Service

Introspect a service to see interfaces and methods:

```bash
busctl --user introspect <service_name> <object_path>
```

## Call a Method

Call a method on a service:

```bash
busctl --user call <service_name> <object_path> <interface_name> <method_name> [arguments...]
```

## Get a Property

Get a property value:

```bash
busctl --user get-property <service_name> <object_path> <interface_name> <property_name>
```

## Set a Property

Set a property value:

```bash
busctl --user set-property <service_name> <object_path> <interface_name> <property_name> <type> <value>
```

## Monitor Signals

Monitor signals from a specific service:

```bash
busctl --user monitor <service_name>
```

Monitor all signals on the bus:

```bash
busctl --user monitor
```

## Show Object Tree

Show the object tree of a service:

```bash
busctl --user tree <service_name>
```

## Get Bus Status

Get status information about the bus:

```bash
busctl --user status
```

## Examples with org.freedesktop.DBus

List all bus names:

```bash
busctl --user call org.freedesktop.DBus / org.freedesktop.DBus ListNames
```

Introspect the DBus service:

```bash
busctl --user introspect org.freedesktop.DBus /
```

Get the machine ID property:

```bash
busctl --user get-property org.freedesktop.DBus / org.freedesktop.DBus MachineId
```

Get the list of activatable services:

```bash
busctl --user call org.freedesktop.DBus / org.freedesktop.DBus ListActivatableNames
```

## Emit a Signal

Emit a signal from a service:

```bash
busctl --user emit <service_name> <object_path> <interface_name> <signal_name> [arguments...]
```

## Get Service Info

Get information about a service:

```bash
busctl --user info <service_name>
```

## Capture Bus Traffic

Capture and display bus traffic:

```bash
busctl --user capture
```

## Argument Types

Common argument types for method calls, signals, and properties:

- s: string
- i: signed 32-bit integer
- u: unsigned 32-bit integer
- b: boolean
- d: double
- y: byte
- n: signed 16-bit integer
- q: unsigned 16-bit integer
- x: signed 64-bit integer
- t: unsigned 64-bit integer
- etc. (see `busctl --help` for the full list)