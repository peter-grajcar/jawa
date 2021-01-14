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

## Compiling the Sources

```
mkdir build
cd build
cmake -DBISON_ROOT=<bison_install_folder> -DFLEX_ROOT=<flex_install_folder> ..
```

## References

- [Jasmin - JVM assembler](http://jasmin.sourceforge.net/)
- [Java Grammar](https://docs.oracle.com/javase/specs/jls/se7/html/jls-2.html)
- [JVM Instruction Set](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html)
