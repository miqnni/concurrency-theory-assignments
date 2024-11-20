# Foata Normal Form
Given:

- a set of $N$ operations e.g. `x := 3x - y + 2z`;
- an alphabet $A$ consisting of $N$ symbols (every symbol corresponds to one operation);
- a word $w$ containing a sequence of operations;

find the following:

- dependence relation `D`;
- independence relation `I`;
- Foata Normal Form $\text{FNF}([w])$ for the trace $[w]$;
- Diekert's dependence graph for $w$.

## Sample input 1
```
x := x + y
y := y + 2z
x := 3x + z
z := y - z
A = {a, b, c, d}
w = baadcb
```

## Sample output 1
```
D = {(a, a), (a, b), (a, c), (b, a), (b, b), (b, d), (c, a), (c, c), (c, d), (d, b), (d, c), (d, d)

I = {(a, d), (b, c), (c, b), (d, a)

  b a a d c b
b 0 1 0 1 0 0
a 0 0 1 0 0 0
a 0 0 0 0 1 1
d 0 0 0 0 1 1
c 0 0 0 0 0 0
b 0 0 0 0 0 0

FNF([w]) = (b)(a d)(a)(c b)
```

## Sample input 2
```
y := 2y + w + v
v := x + v + 3w
x := x + w + v
m := x + y + m
x := q + p + w + y + z + v
y := w + y + x
x := v + z + w
z := z + y + w
m := w + z + v
A = {a, b, c, d, e, f, g, h, i}
w = aieaecdhgbfdbfi
```

## Sample output 2
```
D = {(a, a), (a, b), (a, d), (a, e), (a, f), (a, h), (b, a), (b, b), (b, c), (b, e), (b, g), (b, i), (c, b), (c, c), (c, d), (c, e), (c, f), (c, g), (d, a), (d, c), (d, d), (d, e), (d, f), (d, g), (d, i), (e, a), (e, b), (e, c), (e, d), (e, e), (e, f), (e, g), (e, h), (f, a), (f, c), (f, d), (f, e), (f, f), (f, g), (f, h), (g, b), (g, c), (g, d), (g, e), (g, f), (g, g), (g, h), (h, a), (h, e), (h, f), (h, g), (h, h), (h, i), (i, b), (i, d), (i, h), (i, i)

I = {(a, c), (a, g), (a, i), (b, d), (b, f), (b, h), (c, a), (c, h), (c, i), (d, b), (d, h), (e, i), (f, b), (f, i), (g, a), (g, i), (h, b), (h, c), (h, d), (i, a), (i, c), (i, e), (i, f), (i, g)

  a i e a e c d h g b f d b f i
a 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0
i 0 0 0 0 0 0 1 1 0 1 0 0 0 0 0
e 0 0 0 1 0 1 0 0 0 0 0 0 0 0 0
a 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0
e 0 0 0 0 0 1 0 1 0 0 0 0 0 0 0
c 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0
d 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1
h 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1
g 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0
b 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0
f 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0
d 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
b 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
f 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
i 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

FNF([w]) = (a i)(e)(a)(e)(c h)(d)(g)(b f)(d b)(f i)
```
