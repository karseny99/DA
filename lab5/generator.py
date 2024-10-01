from random import choice
alph = "ABCDE"
s1 = s2 = ""

for i in range(5): 
    size1 = 10 ** i
    # print(choice(alph))
    for _ in range(size1):
        s1 += choice(alph)
        s2 += choice(alph)
    with open(f"tests/test_{i}.i", 'w') as file:
        file.write(s1)
        file.write('\n')
        file.write(s2)
    ans = []
    for k in range(size1):
        for j in range(k, size1):
            if s2.find(s1[k:j]) != -1:
                ans.append(s1[k:j])

    with open(f"tests/test_{i}.a", 'w') as file1:
        ans.sort(reverse=True)
        if(ans == []):
            file1.write(0)
        else:
            ln = len(ans[0])
            file1.write(str(ln))
            file1.write('\n')
            for stri in ans:
                if(ln != len(stri)):break
                file1.write(stri)
                file1.write('\n')

