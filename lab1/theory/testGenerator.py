import sys, random, string

TESTS_DIR = "tests"
TESTS_CNT = 5

CNT_TEST_PER_FILE = 1000

def generate_kv():
    key = random.randint(0, 2 ** 32 - 1)
    value = random.choice(string.ascii_letters)
    return key, value


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test path>")
        sys.exit(1)
    for num in range(1, TESTS_CNT + 1):
        filename = f"{TESTS_DIR}/{num:02}"
        with open(f"{filename}.t", 'w') as tfd, \
             open(f"{filename}.a", "w") as afd:
            arr = []

            for _ in range(CNT_TEST_PER_FILE):
                key, value = generate_kv()
                arr.append((key, value))
                tfd.write(f"{key} {value}\n")

            arr.sort(key=lambda x: x[0])
            for elem in arr:
                afd.write(f"{elem[0]} {elem[1]}\n")



if __name__ == "__main__":
    main()