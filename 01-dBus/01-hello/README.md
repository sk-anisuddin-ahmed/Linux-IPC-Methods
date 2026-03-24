## Calling the Service

First, check that the service is registered on the bus:

```bash
busctl --user list | grep org.example.HelloService
```

You should see the service name in the output.

Then, in another terminal, call the service using busctl:

```bash
busctl --user call org.example.HelloService / org.example.HelloService SayHello
```

The service will respond with "Hello".