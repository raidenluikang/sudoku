#define _CRT_SECURE_NO_WARNINGS 1

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <memory>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <utility>
#include <array>
#include <tuple>
#include <initializer_list>
#include <bitset>
#include <type_traits>
#include <climits>
#include <cinttypes>
#include <limits>
#include <cassert>
#include <deque>
#include <queue>
#include <stack>
#include <random>



int solve();

int main(int argc, char* argv[])
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

  solve();

	return 0;
}

static constexpr int ci[9][9] =
{
    {0, 0, 0,  1, 1, 1,  2, 2, 2},
    {0, 0, 0,  1, 1, 1,  2, 2, 2},
    {0, 0, 0,  1, 1, 1,  2, 2, 2},

    {3, 3, 3,  4, 4, 4,  5, 5, 5},
    {3, 3, 3,  4, 4, 4,  5, 5, 5},
    {3, 3, 3,  4, 4, 4,  5, 5, 5},

    {6, 6, 6,  7, 7, 7,  8, 8, 8},
    {6, 6, 6,  7, 7, 7,  8, 8, 8},
    {6, 6, 6,  7, 7, 7,  8, 8, 8},
};

static constexpr int ic[9][2] =
{
    {0, 0}, {0, 3}, {0, 6},

    {3, 0},  {3, 3}, {3, 6},

    {6, 0},  {6, 3}, {6, 6},
};

class Solution {

public:
    using bits = std::bitset<9>;

    char s[9][9];

    bits rows[9];
    bits cols[9];
    bits squares[9];

    int deep = 0;
    bool failure = false;

    static int least_bit(const bits& b)
    {
        for (int x = 0; x < 9; ++x)
            if (b.test(x))return x;
        return -1;
    }

    bool change_row(int r)
    {
        bits b = rows[r];

        if (b.count() != 1)
            return false;

        int digit = least_bit(b);
        
        assert(digit >= 0 && digit < 9);

        int pos = 0;
        while (pos < 9 && s[r][pos] != '.')
            ++pos;

        assert(pos < 9);

        set(r, pos, digit);

        return true;
    }

    bool change_col(int c)
    {
        bits b = cols[c];
        if (b.count() != 1)
            return false;

        int digit = least_bit(b);
     
        int pos = 0;
        while (pos < 9 && s[pos][c] != '.')
            ++pos;

        assert(pos < 9 );

        set(pos, c, digit);
        return true;
    }

    bool change_square(int k)
    {
        bits b = squares[k];
        if (b.count() != 1)
            return false;

        int digit = least_bit(b);

        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                int x = ic[k][0] + r;
                int y = ic[k][1] + c;
                if (s[x][y] == '.') {
                    set(x, y, digit);
                    return true;
                }
            }
        }

        assert(false);
        return false;
    }

    


    bool change_cell(int r, int c)
    {
        if (s[r][c] != '.')
            return false;

        bits b = rows[r] & cols[c] & squares[ci[r][c]];
        if (b.count() != 1)
            return false;

        int digit = least_bit(b);

        set(r, c, digit);
        
        return true;
    }

    bool change()
    {
        for (int i = 0; i < 9; ++i) {
            if (change_row(i)) return true;
            if (change_col(i)) return true;
            if (change_square(i))return true;

            for (int j = 0; j < 9; ++j) {
                if (change_cell(i, j)) return true;
            }
        }

        return false;


    }

    bool done() const
    {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (s[i][j] == '.')
                    return false;
            }
        }
        return true;
    }

    void set(int i, int j, int digit)
    {
        assert(digit >= 0 && digit < 9);
        assert(s[i][j] == '.');

        s[i][j] = '1' + digit;

        rows[i].reset(digit);
        cols[j].reset(digit);
        squares[ci[i][j]].reset(digit);
    }

    bool check_failure() const
    {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (s[i][j] == '.') {
                    auto u = rows[i] & cols[j] & squares[ci[i][j]];
                    if (u.count() == 0)
                        return true;
                }
            }
        }
        return false;
    }

    bool solveSudoku(std::vector<std::vector<char>>& board)
    {
        for (int i = 0; i < 9; ++i) {
            rows[i] = bits{};
            rows[i].flip(); // set all bits true

            cols[i] = bits{};
            cols[i].flip();

            squares[i] = bits{};
            squares[i].flip();
        }



        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                s[i][j] = '.';
                char c = board[i][j];
                if (c >= '1' && c <= '9') {
                    set(i, j, c - '1');
                }
                else 
                {
                    assert(c == '.');
                }
            }
        }

        while (change() && !check_failure())
            ;
        
        if (check_failure())
            return false;

        if (!done())
        {
            if (this->deep >= 10) {
                return false;
            }

            bits best{};
            
            bool f = false;
            
            int r = 0, c = 0;
            
            for (int i = 0; i < 9; ++i)
            {
                for (int j = 0; j < 9; ++j)
                {
                    if (s[i][j] == '.')
                    {
                        auto u = rows[i] & cols[j] & squares[ci[i][j]];
                        
                        if (u.count() == 0)
                            return false;

                        //assert(u.count() > 0);

                        if (!f || u.count() < best.count())
                        {
                            f = true;
                            best = u;
                            r = i;
                            c = j;
                        }
                    }
                }
            }

            for (int digit = 0; digit < 9; ++digit)
            {
                if (best.test(digit))
                {

                    Solution sol;

                    sol.deep = this->deep + 1;

                    auto v = board;
                    for (int i = 0; i < 9; ++i) {
                        for (int j = 0; j < 9; ++j) {
                            v[i][j] = s[i][j];
                        }
                    }

                    //let set this digit.
                    v[r][c] = '1' + digit;

                    if (sol.solveSudoku(v))
                    {
                        board = v;

                        return true;
                    }
                }
            }
            return false;
        }

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = s[i][j];
            }
        }

        return true;
    }
};

std::vector<std::string> parseLines(std::string line)
{
    // [..], [...], ....
    std::vector<std::string> lines(9);
    for (int i = 0, z = 0; i < 9; ++i) {
        while (z < line.size() && line[z] != '[')
            ++z;
        assert(line[z] == '[');
        int start = z;
        while (z < line.size() && line[z] != ']')
            ++z;
        assert(line[z] == ']');
        int finish = z;
        lines[i] = line.substr(start + 1, finish - start  - 1); // without [] symbols

    }
    return lines;
}
std::vector<char> parseDigits(std::string line)
{
    // line:  "1", ".", 
    std::vector<char> digits(9);
    for (int i = 0, z = 0; i < 9; ++i) {
        while (z < line.size() && line[z] != '\"')
            ++z;
        assert(line[z] == '\"');
        digits[i] = line[z + 1];
        z += 3;// skip "."  3 symbols
    }
    return digits;
}

std::vector<std::vector<char>> parseBoard(std::string line) {
    // line :  [[....],[....],.....]
    // each part   "1", "2", ".",....
    std::vector<std::vector<char>> board(9, std::vector<char>(9, '.'));
    line = line.substr(1, line.size() - 2);
    auto cells = parseLines(line);
    for (int i = 0; i < 9; ++i) {
        auto digits = parseDigits(cells[i]);
        for (int j = 0; j < 9; ++j) {
            board[i][j] = digits[j];
        }
    }
    return board;
}
int solve()
{
    std::string line;
    std::getline(std::cin, line);
    auto board = parseBoard(line);
    Solution sol;
    sol.solveSudoku(board);

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            putchar(board[i][j]);
        }
        putchar('\n');
    }
	return 0;
}
