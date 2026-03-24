## Signals Service

Call method to trigger signal:

```bash
busctl --user call org.example.HelloService / org.example.HelloService IncrementCounter
```

Monitor signals:

```bash
busctl --user monitor org.example.HelloService
```