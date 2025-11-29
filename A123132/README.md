# In search of $a(n) = n$

Consider $a(n)$ defined as [A123132](https://oeis.org/A123132). The equation $a(n) = n$ seems to be satisfied only with $n=25$.

Denote $d(x) := \lfloor \log_{10}(x) \rfloor$ + 1 as the number of digit base 10 of $x$. 
## Even $n$
If $n$ is even, the last digit of $n$ is an even digit. However, $a(n)$ ends with an even digit if and only if $2$ is the only prime factor of $n$. This means I can restrict the search to only $n = 2^k$. Notice that $a(2^k) = 10k + 2$, and $10k + 2 = 2^k$ has no integral solution. 
## If $n$ is divisible by 5
If $n$ is divisible by 5, I can assume the last digit of $n$ is 5 (since if it's 0, it is even, and thus cannot satisfies the equation). As the only prime ending in 5 is 5, it must be the case that $n$ has no prime factor greater than 5. In other words, $n = 3^i5^j$ . 
- With $i=0$, $5^j = 10j + 5$ gives only solution $j = 2$. 
- With $i > 0$, $3^i5^j = (10i+3) \cdot 10^{d(j)+1} + 10j + 5$
	- Notice that $$\align{i \log 3 + j \log 5 &> \log i + \log j + 4 \\ &>\log (i\cdot10^{d(j)+2} + 3\cdot 10^{d(j)+1} + j\cdot10 + 5)}$$ for $i > 11$ or $j > 11$. 
	- Checking small values shows no solution.
## For prime powers
If $p$ is a prime then $n = p^r$ is not a solution for all $r$ except $5^2$. I show this by claiming $p^r > a(p^r)$. First, notice that $a(p^r) = r \cdot 10^d(p) + p$. Thus it is enough to show that $$\frac{p^r - p}{r \cdot 10^{d(p)}} > 1$$ First, take $p \ge 23$ so that $$\frac{p^r - p}{r \cdot 10^{d(p)}} \ge \frac{p^r - p}{r \cdot 10 \cdot p} \ge \frac{p^{r-1} - 1}{10r} \ge \frac{23^{r-1} - 1}{10r} > 1$$ for all $r \ge 2$. 

When $r = 1$, $a(p) = 10^{d(p)} + p > p$. and when $p < 23$, I checked for the possible cases and concluded $5^2$ is the only answer.
