# Style

---

For all multi-line declarations (structs, functions, etc.) do not do the Linus thing, instead do:

```c
void func(void) {
    ...
}

typedef struct {
    ...
} Foo;

etc.
```

Always typdef `enum` and `struct`.

`PascalCase` for types, `snake_case` for functions. Try to avoid `xxx_t`

Space out your code; spaces after casts and inside and between `(Vector2) { items, in, vectors }`

No space after `if()` declarations