0. Clone the repository.

1. Fetch the submodule `ev3dev`.
```
git submodule init && git submodule update` 
```

2. Make a libraries directory and add the necessary libs.
```
mkdir libraries && cp ev3dev-c/lib/libev3dev-c.a ./libraries
```

3. Compile
```
make
```