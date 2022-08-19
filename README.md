
USAGE
=====
```bash
USAGE: ./a.out [GUESS]...
  lowercase letter         GREY
  captialized letter       YELLOW
  preceding asterisk (*)   GREEN
```

EXAMPLE
=======
```bash
$ ./a.out Adieu sTork pANTy
-----
chant   1       2       3       3       1       10*
thana   2       2       3       3       1       0
twang   2       1       3       3       1       10*
----- 3
$ ./a.out Adieu sTork pANTy ch*a*nT
g++ -std=c++11 wordle.cpp -o a.out
-----
twang   1       1       1       1       1       5*
----- 1
```
