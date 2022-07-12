import time

if __name__ == "__main__":
    print('Python')
    N = 10000000
    start_time = time.time()
    l = []
    for i in range(N):
        l.append(i)
    print(f'List insertion :\t{time.time() - start_time}s')

    s = 0
    start_time = time.time()
    for i in range(N):
        s+= l[i]
    print(f'Summation :\t\t{time.time() - start_time}s')
