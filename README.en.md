<div align="right">
  <a href="README.md">Türkçe</a> | <b>English</b>
</div>

# CM (C-Mat / C-Logic)

**CM** is a minimalist programming language built on C's raw performance, eliminating semicolons and verbose type declarations in favor of **mathematical notation** and **clear logical flow**. `.cm` source files are transpiled into pure C via the custom compiler engine (`cm_derle.c`) and compiled directly into native machine code.

---

## Installation & Usage

Compile the transpiler engine once using `gcc`:

```bash
gcc cm_derle.c -o cm
