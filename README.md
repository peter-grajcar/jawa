# Jawa

Jawa jest obiektowy język programowania.

## Code Examples

<pre>
<code>
<font color="#ea4a5a">publiczna klasa</font> <font color="#fb8532">WitajŚwiecie</font> {
    
    <font color="#ea4a5a">publiczny statyczny void</font> <font color="#fb8532">głowny</font>(<font color="#fb8532">Łańcuch</font>[] <font color="#fb8532">args</font>) {
        <font color="#fb8532">System</font>.<font color="#fb8532">wyjście</font>.<font color="#fb8532">wydrukovać</font>(<font color="#79b8ff">"Witaj świecie"</font>);
    }
    
}
</code>
</pre>

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
