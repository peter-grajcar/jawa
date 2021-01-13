# Jawa

Jawa jest obiektowy język programowania.

## Code Examples

```jawa
publiczna klasa WitajŚwiecie {
    publiczny statyczny void głowny(Łańcuch[] args) {
        System.wyjście.wydrukovać("Witaj świecie");
    }
}
```

## Compiling Sources

```
mkdir build
cd build
cmake -DBISON_ROOT=<bison_install_folder> -DFLEX_ROOT=<flex_install_folder> ..
```