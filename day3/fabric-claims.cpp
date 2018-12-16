/*
 *--- Day 3: No Matter How You Slice It ---
 *
 * The Elves managed to locate the chimney-squeeze prototype fabric for Santa's
 * suit (thanks to someone who helpfully wrote its box IDs on the wall of the
 * warehouse in the middle of the night). Unfortunately, anomalies are still
 * affecting them - nobody can even agree on how to cut the fabric.
 *
 * The whole piece of fabric they're working on is a very large square - at
 * least 1000 inches on each side.
 *
 * Each Elf has made a claim about which area of fabric would be ideal for
 * Santa's suit. All claims have an ID and consist of a single rectangle with
 * edges parallel to the edges of the fabric. Each claim's rectangle is defined
 * as follows:
 *
 *     The number of inches between the left edge of the fabric and the left edge of the rectangle.
 *     The number of inches between the top edge of the fabric and the top edge of the rectangle.
 *     The width of the rectangle in inches.
 *     The height of the rectangle in inches.
 *
 * A claim like #123 @ 3,2: 5x4 means that claim ID 123 specifies a rectangle 3
 * inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4
 * inches tall. Visually, it claims the square inches of fabric represented by #
 * (and ignores the square inches of fabric represented by .) in the diagram
 * below:
 *
 * ...........
 * ...........
 * ...#####...
 * ...#####...
 * ...#####...
 * ...#####...
 * ...........
 * ...........
 * ...........
 *
 * The problem is that many of the claims overlap, causing two or more claims to
 * cover part of the same areas. For example, consider the following claims:
 *
 * #1 @ 1,3: 4x4
 * #2 @ 3,1: 4x4
 * #3 @ 5,5: 2x2
 *
 * Visually, these claim the following areas:
 *
 * ........
 * ...2222.
 * ...2222.
 * .11XX22.
 * .11XX22.
 * .111133.
 * .111133.
 * ........
 *
 * The four square inches marked with X are claimed by both 1 and 2. (Claim 3,
 * while adjacent to the others, does not overlap either of them.)
 *
 * If the Elves all proceed with their own plans, none of them will have enough
 * fabric. How many square inches of fabric are within two or more claims?
 *
 *  --- Part Two ---
 *
 * Amidst the chaos, you notice that exactly one claim doesn't overlap by even a
 * single square inch of fabric with any other claim. If you can somehow draw
 * attention to it, maybe the Elves will be able to make Santa's suit after all!
 *
 * For example, in the claims above, only claim 3 is intact after all claims are
 * made.
 *
 * What is the ID of the only claim that doesn't overlap?
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

#include <stdio.h>
#include <assert.h>

using namespace std;


class Patch {
public:
    Patch(unsigned id, int x, int y, unsigned width, unsigned height) :
        id(id), x(x), y(y), width(width), height(height) {
    }

    unsigned id;
    int x = 0;
    int y = 0;
    unsigned width = 0;
    unsigned height = 0;
};

typedef vector<vector<vector<unsigned>>> FabricMap;


/*
 * Get all the claims and also calculate the enclosing fabric.
 * Example line:
 *       #1 @ 287,428: 27x20
 */
pair<Patch, vector<Patch>>
getClaimsFromInput()
{
    vector<Patch> claims;

    string line;
    int fabWidth = 0, fabHeight = 0;
    while (getline(cin, line)) {
        stringstream ss{line};

        string token;
        getline(ss, token, ' '); // #1
        unsigned id;
        assert(1 == sscanf(token.c_str(), "#%d", &id));

        getline(ss, token, ' '); // @

        getline(ss, token, ' '); // x,y
        int x, y;
        assert(2 == sscanf(token.c_str(), "%d,%d:", &x, &y));
        assert(x >= 0);
        assert(y >= 0);

        getline(ss, token, ' '); // w,h
        int w, h;
        assert(2 == sscanf(token.c_str(), "%dx%d", &w, &h));
        assert(w >= 0);
        assert(h >= 0);

        claims.emplace_back(id, x, y, w, h);

        auto right = x + w;
        auto bottom = y + h;
        if (fabWidth < right) {
            fabWidth = right;
        }
        if (fabHeight < bottom) {
            fabHeight = bottom;
        }
    }

    return make_pair(Patch(0, 0, 0, fabWidth, fabHeight), claims);
}


/*
 * Mark a claim on fabric.
 */
void markClaim(Patch const &claim, FabricMap &fabMap)
{
    for (unsigned i = 0; i < claim.height; i++) {
        auto y = claim.y + i;
        auto &row = fabMap[y];
        for (unsigned j = 0; j < claim.width; j++) {
            auto x = claim.x + j;
            row[x].push_back(claim.id);
        }
    }
}


/*
 * Get overlapped area.
 */
size_t
getOverlappedArea(vector<Patch> const &claims, Patch &fab)
{
    // initialize all "pixels" to 0
    FabricMap fabricMap(fab.height, vector<vector<unsigned>>(fab.width));
    unordered_set<unsigned> nonOverlappedSet;

    for (auto &clm : claims) {
        nonOverlappedSet.insert(clm.id);
    }

    size_t totalClaimArea = 0;
    for (auto &clm : claims) {
        markClaim(clm, fabricMap);
        totalClaimArea += clm.width * clm.height;
        printf("claim: %u x %u = %u, total %zu\n",
               clm.width, clm.height, clm.width * clm.height,
               totalClaimArea);
    }

    // Go through each pixel in the fabric and tabulate overlapped pixels
    unsigned overlapped = 0;
    for (auto const &row : fabricMap) {
        for (auto const &claimedVec : row) {
            auto claimedNr = claimedVec.size();
            if (claimedNr > 1) {
                overlapped += 1;
                for (auto const &clmId : claimedVec) {
                    nonOverlappedSet.erase(clmId);
                }
                //printf("%x", claimedNr);
            } else if (claimedNr == 1) {
                //printf("#");
            } else {
                //printf(".");
            }
        }
        //printf("\n");
    }

    printf("Non-overlapped claims:\n");
    for (auto &clmId : nonOverlappedSet) {
        printf("%u, ", clmId);
    }
    printf("\n");

    return overlapped;
}


int main () {
    auto [fab, claims] = getClaimsFromInput();

    auto result = getOverlappedArea(claims, fab);
    cout << "Overlapped area = " << result << endl;

    return 0;
}
