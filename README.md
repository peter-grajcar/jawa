# Jawa

Jawa jest obiektowy język programowania.

## Code Examples

<pre lang="jawa">
<code>
<span style="color: #ea4a5a; ">publiczna klasa</span> <span style="color: #fb8532; ">WitajŚwiecie</span> {
    
    <span style="color: #ea4a5a; ">publiczny statyczny void</span> <span style="color: #fb8532; ">głowny</span>(<span style="color: #fb8532; ">Łańcuch</span>[] <span style="color: #fb8532; ">args</span>) {
        <span style="color: #fb8532; ">System</span>.<span style="color: #fb8532; ">wyjście</span>.<span style="color: #fb8532; ">wydrukovać</span>(<span style="color: #79b8ff; ">"Witaj świecie"</span>);
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
