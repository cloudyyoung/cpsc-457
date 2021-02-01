import sys

def main():
    max_pali = ""
    while True:
        line = sys.stdin.readline()
        if len(line) == 0: break
        words = line.split()
        for word in words:
            if len(word) <= len(max_pali):
                continue
            if word.lower() == word.lower()[::-1]:
                max_pali = word
    print(f"Longest palindrome: {max_pali}")

main()
