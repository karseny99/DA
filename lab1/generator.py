import sys
import random
import string

TEST_COUNT = 6

def get_random_MD5():
    length = 32
    random_list = [ random.choice("0123456789abcdef") for _ in range(length) ]
    return "".join(random_list)

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test directory>")
        sys.exit(1)

    test_dir = sys.argv[1]

    lines = [1]
    lines.extend([ 10 ** i for i in range(1, TEST_COUNT) ])

    for enum, test_count in enumerate(range(1, TEST_COUNT+1)):
        test = []
        answer = []

        line_count = lines[enum]
        print(line_count)
        for _ in range(line_count):
            key = get_random_MD5()
            value = random.randint(0, 2 ** 64 - 1)
            test.append((key, value))

        test_name = "{}/{:02d}".format(test_dir, test_count)
        with open(f'{test_name}.t', 'w') as ftest:
            for key, value in test:
                ftest.write(f'{key} {value}\n')

        answer = sorted(test, key=lambda x: x[0])
        with open(f'{test_name}.a', 'w') as ftest:
            for key, value in answer:
                ftest.write(f'{key} {value}\n')
main()