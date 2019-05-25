
def first_with_given_key(iterable, key=lambda k: k):
    d = dict()
    for i in iterable:
        if key(i) not in d:
            d[key(i)] = i
            yield i

if __name__ == "__main__":
    print(tuple(first_with_given_key([[1], [2, 3], [4], [5, 6, 7], [8, 9]], key=len)))
