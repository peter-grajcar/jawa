# Jawa

Jawa jest obiektowy język programowania.

## Code Examples

<pre lang="jawa">
<span class="pl-k">publiczna klasa</span> <span class="pl-v">WitajŚwiecie</span> {
    
    <span class="pl-k">publiczny statyczny void</span> <span class="pl-v">głowny</span>(<span class="pl-v">Łańcuch</span>[] <span class="pl-v">args</span>) {
        <span class="pv-v">System</span>.<span class="pl-v">wyjście</span>.<span class="pl-v">wydrukovać</span>(<span class="pl-c1">"Witaj świecie"</span>);
    }
    
}
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
