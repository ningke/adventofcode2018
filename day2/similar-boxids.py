"""
--- Part Two ---

Confident that your list of box IDs is complete, you're ready to find the boxes full of prototype fabric.

The boxes will have IDs which differ by exactly one character at the same position in both strings. For
example, given the following box IDs:

abcde
fghij
klmno
pqrst
fguij
axcye
wvxyz

The IDs abcde and axcye are close, but they differ by two characters (the second and fourth). However, the
IDs fghij and fguij differ by exactly one character, the third (h and u). Those must be the correct boxes.

What letters are common between the two correct box IDs? (In the example above, this is found by removing
the differing character from either ID, producing fgij.)

"""

def find_common_partial_boxid(boxids):
    """ Return the partial boxid that appears twice or more """
    boxid_map = {}
    for bid in boxids:
        if bid not in boxid_map:
            boxid_map[bid] = 0
        boxid_map[bid] += 1

    # find the common ones:
    for bid, cnt in boxid_map.items():
        if cnt >= 2:
            return bid

    return None


# The O(n*m^2) algorithm
def find_common_boxid(boxids):
    """ Return common part of boxid """
    bidlen = len(boxids[0])
    for i in range(0, bidlen):
        partial_bids = [bid[:i] + bid[(i+1):] for bid in boxids]
        common_part = find_common_partial_boxid(partial_bids)
        if common_part is not None:
            return [common_part, i]

    return None


def check_input(boxids):
    """ We assume all boxids are of the same length so check it here """
    idlen = boxids[0]
    for bid in boxids:
        if len(boxid) != bid:
            return False
    return True


import sys
def main():
    """ main """
    boxids = [line.strip() for line in sys.stdin]
    assert check_input(boxids)

    print find_common_boxid(boxids)


main()
