from string import ascii_lowercase
import random

def find_all(seq, subseq):
    n = len(seq)
    m = len(subseq)
    for i in range(n - m + 1):
        if all(seq[i + j] == subseq[j] for j in range(m)):
            yield i

def findall(p, s):
    i = s.find(p)
    while i != -1:
        yield i
        i = s.find(p, i+1)

p = ""
inline = []
for i in range(6):
    inline = []
    p = ""
    for el in [random.randint(1, 2) for _ in range(random.randint(2, 2))]:  
        p += str(el) + ' ' 
    p = p.rstrip()  
    s = ""
    for line in range(400):
        inline.append(random.randint(200, 1000))
        new_line = [random.randint(1, 2) for _ in range(inline[-1])] 
        for el in new_line:  
            s += str(el) + ' '  
        s = s[:len(s)-1] 
        s += '\n'
    p.replace(" \n", "\n")
    s.replace(" \n", "\n")
    with open(f"tests/test{i}.t", 'w') as f:
        f.write(f"{p}\n{s}")

    inline = [0] + inline
    for j in range(1, len(inline)):
        inline[j] += inline[j - 1]

    with open(f"tests/test{i}.t", 'r') as f:
        pattern = list(map(int, f.readline().split()))
        data = [int(x) for x in f.read().split()]
        entry = list(find_all(data, pattern))
        entries = list(findall(p, s))
        with open(f"tests/test{i}.a", 'w') as f1:
            for e in entry:
                res = next(x for x, val in enumerate(inline) if val > e)
                f1.write(str(res))
                f1.write(', ')
                f1.write(str(e - inline[res - 1] + 1))
                f1.write('\n')
        with open(f"tests/test{i}.t", 'r') as f2:
            pt = f2.readline().replace('\n', ' ').rstrip()
            st = f2.read().replace('\n', ' ')
            with open(f"tests/test{i}.s", 'w') as f3:
                f3.write(st)
            if(len(list(findall(pt, st))) != len(entry)):
                print("WARNING !!!")

            