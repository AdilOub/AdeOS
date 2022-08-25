

def main():
    a = str(input("\n>"))
    c = '{'
    for i in range(0, len(a)):
        c += a[i]
        c += ","
    c = c[:len(c)-1]
    c+="}"
    print(c)
    print("\n")
    main()

if __name__ == "__main__":
    main()