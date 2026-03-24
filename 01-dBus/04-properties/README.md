## Properties Service

Get a property:

```bash
busctl --user get-property org.example.HelloService / org.example.HelloService Counter
```

Set a property:

```bash
busctl --user set-property org.example.HelloService / org.example.HelloService Counter i 42
```

Get all properties:

```bash
busctl --user get-property org.example.HelloService / org.example.HelloService
```