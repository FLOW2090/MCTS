#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <assert.h>
#include <cstring>
#include <random>
#include <time.h>

using namespace std;

// structs
struct board {
    // 'O' for player, 'X' for robot, ' ' for empty, 'D' for draw
    // suppose the robot plays first
    // b[i][j] is the i-th row j-th column entry
    char b[3][3];
    bool isFull = false;
    board() { memset(b, ' ', sizeof(b)); }
    board(const board & e) : isFull(e.isFull) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                b[i][j] = e.b[i][j];
            }
        }
    }
    ~board() {}
    int countEmpty() { // count how many places are empty
        int cnt = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (b[i][j] == ' ') { cnt++; }
            }
        }
        return cnt;
    }
    void updateIsFull() { if (countEmpty() == 0) isFull = true; }
    void operator = (const board & e) {
        isFull = e.isFull;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                b[i][j] = e.b[i][j];
            }
        }
        return;
    }
};

struct game {
    char lastPiece = 'O'; // suppose the robot(or X/*  */) places first
    int lastX = -1, lastY = -1; // the position of last time in the smallBoard 
    int lastBX = -1, lastBY = -1; // the position of last time in the bigBoard (to mark which block to be updated)
    board bigBoard;
    board smallBoard[3][3];
    game() {}
    game(const game & g) : bigBoard(g.bigBoard), lastX(g.lastX), lastY(g.lastY), lastBX(g.lastBX), lastBY(g.lastBY), lastPiece(g.lastPiece) 
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                smallBoard[i][j] = g.smallBoard[i][j];
            }
        }
    }
    ~game() {}
    void updateSmall2Big() { // update the bigBoard from the smallBoard
        // check row
        for (int i = 0; i < 3; i++) {
            if (smallBoard[lastBX][lastBY].b[i][0] != ' ' && smallBoard[lastBX][lastBY].b[i][0] != 'D' && smallBoard[lastBX][lastBY].b[i][0] == smallBoard[lastBX][lastBY].b[i][1] && smallBoard[lastBX][lastBY].b[i][0] == smallBoard[lastBX][lastBY].b[i][2]) {
                bigBoard.b[lastBX][lastBY] = smallBoard[lastBX][lastBY].b[i][0];
                smallBoard[lastBX][lastBY].isFull = true;
                return;
            }
        }
        // check column
        for (int i = 0; i < 3; i++) {
            if (smallBoard[lastBX][lastBY].b[0][i] != ' ' && smallBoard[lastBX][lastBY].b[0][i] != 'D' && smallBoard[lastBX][lastBY].b[0][i] == smallBoard[lastBX][lastBY].b[1][i] && smallBoard[lastBX][lastBY].b[0][i] == smallBoard[lastBX][lastBY].b[2][i]) {
                bigBoard.b[lastBX][lastBY] = smallBoard[lastBX][lastBY].b[0][i];
                smallBoard[lastBX][lastBY].isFull = true;
                return;
            }
        }
        // check diagonal
        if (smallBoard[lastBX][lastBY].b[0][0] != ' ' && smallBoard[lastBX][lastBY].b[0][0] != 'D' && smallBoard[lastBX][lastBY].b[0][0] == smallBoard[lastBX][lastBY].b[1][1] && smallBoard[lastBX][lastBY].b[0][0] == smallBoard[lastBX][lastBY].b[2][2]) {
            bigBoard.b[lastBX][lastBY] = smallBoard[lastBX][lastBY].b[0][0];
            smallBoard[lastBX][lastBY].isFull = true;
            return;
        }
        if (smallBoard[lastBX][lastBY].b[0][2] != ' ' && smallBoard[lastBX][lastBY].b[0][2] != 'D' && smallBoard[lastBX][lastBY].b[0][2] == smallBoard[lastBX][lastBY].b[1][1] && smallBoard[lastBX][lastBY].b[0][2] == smallBoard[lastBX][lastBY].b[2][0]) {
            bigBoard.b[lastBX][lastBY] = smallBoard[lastBX][lastBY].b[0][2];
            smallBoard[lastBX][lastBY].isFull = true;
            return;
        }
        // board is full
        if (smallBoard[lastBX][lastBY].isFull) {
            int X = 0, O = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    switch (smallBoard[lastBX][lastBY].b[i][j]) {
                        case 'X' :
                            X++;
                            break;
                        case 'O':
                            O++;
                            break;
                        default:
                            break;
                    }
                }
            }
            if (X > O) { bigBoard.b[lastBX][lastBY] = 'X'; }
            if (X < O) { bigBoard.b[lastBX][lastBY] = 'O'; }
            if (X == O) { bigBoard.b[lastBX][lastBY] = 'D'; }
        }
        // no winner
    }
    char terminated() {/*  */
        // check row
        for (int i = 0; i < 3; i++) {
            if (bigBoard.b[i][0] != ' ' && bigBoard.b[i][0] != 'D' && bigBoard.b[i][0] == bigBoard.b[i][1] && bigBoard.b[i][0] == bigBoard.b[i][2]) {
                return bigBoard.b[i][0];
            }
        }
        // check column
        for (int i = 0; i < 3; i++) {
            if (bigBoard.b[0][i] != ' ' && bigBoard.b[0][i] != 'D' && bigBoard.b[0][i] == bigBoard.b[1][i] && bigBoard.b[0][i] == bigBoard.b[2][i]) {
                return bigBoard.b[0][i];
            }
        }
        // check diagonal
        if (bigBoard.b[0][0] != ' ' && bigBoard.b[0][0] != 'D' && bigBoard.b[0][0] == bigBoard.b[1][1] && bigBoard.b[0][0] == bigBoard.b[2][2]) {
            return bigBoard.b[0][0];
        }
        if (bigBoard.b[0][2] != ' ' && bigBoard.b[0][2] != 'D' && bigBoard.b[0][2] == bigBoard.b[1][1] && bigBoard.b[0][2] == bigBoard.b[2][0]) {
            return bigBoard.b[0][2];
        }
        // board is full
        if (bigBoard.isFull) {
            int X = 0, O = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    switch (bigBoard.b[i][j]) {
                        case 'X' :
                            X++;
                            break;
                        case 'O':
                            O++;
                            break;
                        default:
                            break;
                    }
                }
            }
            if (X > O) { return 'X'; }
            if (X < O) { return 'O'; }
            if (X == O) { return 'D'; }
        }
        // no winner
        return ' ';
    }
    int placeNum() {
        int num = 0;
        // destination block is full or just begin
        if ((lastX == -1 && lastY == -1) || (smallBoard[lastX][lastY].isFull)) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (bigBoard.b[i][j] == ' ') {
                        num += smallBoard[i][j].countEmpty();
                    }
                }
            }
        }
        else {
            num = smallBoard[lastX][lastY].countEmpty();
        }
        return num;
    }
    void place(int e) { // place the piece at the e-th available place
        if (lastPiece == 'O') { lastPiece = 'X'; }
        else { lastPiece = 'O'; }
        bool toBreak = false;
        if ((lastX == -1 && lastY == -1) || (smallBoard[lastX][lastY].isFull)) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (bigBoard.b[i][j] != ' ') { continue; }
                    if (smallBoard[i][j].countEmpty() < e) { e -= smallBoard[i][j].countEmpty(); }
                    else {
                        int cnt = 0;
                        for (int row = 0; row < 3; row++) {
                            for (int col = 0; col < 3; col++) {
                                if (smallBoard[i][j].b[row][col] == ' ') { cnt++; }
                                if (cnt == e) {
                                    smallBoard[i][j].b[row][col] = lastPiece;
                                    lastX = row, lastY = col;
                                    lastBX = i, lastBY = j;
                                    toBreak = true;
                                    break;
                                }
                            }
                            if (toBreak) { break; }
                        }
                        break;
                    }
                }
                if (toBreak) { break; }
            }
        }
        else {
            int cnt = 0;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (smallBoard[lastX][lastY].b[row][col] == ' ') { cnt++; }
                    if (cnt == e) {
                        smallBoard[lastX][lastY].b[row][col] = lastPiece;
                        lastBX = lastX, lastBY = lastY;
                        lastX = row, lastY = col;
                        toBreak = true;
                        break;
                    }
                }
                if (toBreak) { break; }
            }
        }
        smallBoard[lastBX][lastBY].updateIsFull();
        updateSmall2Big();
        bigBoard.updateIsFull();
    }
    void place(int x, int y) { // place at (x, y)
        if (lastPiece == 'O') { lastPiece = 'X'; }
        else { lastPiece = 'O'; }
        lastBX = x / 3;
        lastBY = y / 3;
        lastX = x - lastBX * 3;
        lastY = y - lastBY * 3;
        smallBoard[lastBX][lastBY].b[lastX][lastY] = lastPiece;
        smallBoard[lastBX][lastBY].updateIsFull();
        updateSmall2Big();
        bigBoard.updateIsFull();
    }
};

struct node {
    node * parent = nullptr;
    vector<node*> children;
    int playedTimes = 0;
    int winTimes = 0;
    double ucb = numeric_limits<double>::max(); // default ucb: infinity
    game * g = nullptr;
    node() {}
    node(const node & n) { g = new game(*n.g); }
    ~node() {
        int size = children.size();
        for (int i = 0; i < size; i++) {
            delete children[i];
        }
        delete g;
    }
};

using nodePosi = node*;

// global const variables
const int C = 2;
const int MAX_PLAY_TIME = 10000;

// global variables
int totalTimes = 0;
vector<nodePosi> root;

// functions
void inline attachAsChild(nodePosi c, nodePosi p) {
    p->children.push_back(c);
    c->parent = p;
}

double inline UCB(nodePosi e) {
    return ( double(e->winTimes) / double(e->playedTimes) ) + C * sqrt( ( log(double(totalTimes)) / double(e->playedTimes) ) );
}

void inline updateWinAbove(nodePosi e) {
    while (e) {
        e->winTimes++;
        e = e->parent;
    }
}

void inline updatePlayAbove(nodePosi e) {
    while (e) {
        e->playedTimes++;
        e = e->parent;
    }
}

void inline updateUCBAbove(nodePosi e) {
    while (e) {
        e->ucb = UCB(e);
        e = e->parent;
    }
}

nodePosi inline pickMaxUCB(vector<nodePosi> vec) {
    double maxUCB = -1;
    nodePosi selected = nullptr;
    for (auto & element : vec) {
        if (element->ucb > maxUCB) {
            maxUCB = element->ucb;
            selected = element;
        }
        if (maxUCB == numeric_limits<double>::max()) { break; }
    }
    assert(selected != nullptr);
    return selected;
}

nodePosi inline enumerate(nodePosi element) { // return the first child
    int N = element->g->placeNum(); // get how many places can be placed
    for (int i = 1; i <= N; i++) {
        nodePosi t = new node(*element);
        t->g->place(i);
        attachAsChild(t, element);
    }
    return element->children[0];
}

void inline rollout(nodePosi element) { // play quickly and randomly to say who is the winner
    game *g = new game(*(element->g));
    char result = ' ';
    int randNum;
    int i = 0;
    while ((result = g->terminated()) == ' ') {
        int x = g->placeNum();
        randNum = rand() % g->placeNum() + 1;
        g->place(randNum);
        i++;
    }
    delete g;
    if (result == 'O') { updateWinAbove(element); }
    updatePlayAbove(element);
    updateUCBAbove(element);
}

void MCTS() {
    nodePosi search = root[0];
    // search along the max UCB node from the root node till the leaf node
    while (search->children.size()) {
        vector<nodePosi> children = search->children;
        search = pickMaxUCB(children);
    }
    // if haven't played, then expand
    if (search->playedTimes == 0) {
        search = enumerate(search);
    }
    // play
    rollout(search);
}

int main() {
    srand((unsigned)time(NULL));
    nodePosi t = new node();
    root.push_back(t);
    root[0]->g = new game();
    int x, y, childrenSize;
    nodePosi bestChild;
    vector<nodePosi> children;
    while (true) {
        totalTimes = 0;
        cin >> x >> y; // robot places
        x--;
        y--;
        root[0]->g->place(x, y);
        while (totalTimes++ <= MAX_PLAY_TIME) {
            MCTS();
        }
        children = root[0]->children;
        childrenSize = children.size();
        bestChild = new node(*pickMaxUCB(children));
        cout << bestChild->g->lastBX * 3 + bestChild->g->lastX + 1 << ' ' << bestChild->g->lastBY * 3 + bestChild->g->lastY + 1 << endl;
        delete root[0];
        root[0] = bestChild;
    }
    return 0;
}