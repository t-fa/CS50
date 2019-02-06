from nltk.tokenize import sent_tokenize
from cs50 import get_int

def lines(a, b):
    """Return lines in both a and b"""

    a_list = a.split('\n')
    b_list = b.split('\n')

    same_line = []

    for item in a_list:
        if item in b_list:
            if item not in same_line:
                same_line.append(item)
    return same_line


def sentences(a, b):
    """Return sentences in both a and b"""

    a_sent = sent_tokenize(a)
    b_sent = sent_tokenize(b)

    same_sent = []

    for sent in a_sent:
        if sent in b_sent:
            if sent not in same_sent:
                same_sent.append(sent)
    return same_sent


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    def split(string, n):
        substring_list = []

        temp = len(string) - n + 1

        for i in range(temp):
            if string[i:n+i] not in substring_list:
                substring_list.append(string[i:n+i])
        return substring_list

    a_list = split(a, n)
    b_list = split(b, n)

    same_subs = []

    for sub in a_list:
        if sub in b_list and sub not in same_subs:
            same_subs.append(sub)
    return same_subs
