def Fib(n):
    if n == 0: return 0
    if n == 1: return 1
    return Fib(n - 1) + Fib(n - 2)


if __name__ == "__main__":
    with open("../data-samples/fib.txt", "w") as f:
        val = int(input())

        for i in range(1, val + 1):
            for _ in range(Fib(i)):
                f.write(chr(ord('a') + i - 1))