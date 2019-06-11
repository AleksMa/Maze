#include <bits/stdc++.h>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct point {
 public:
  point() : x(0), y(0) {};
  point(int x, int y) : x(x), y(y) {};

  bool operator==(const point &p) const {
    return x == p.x && y == p.y;
  }

  point &operator=(const point &p) {
    if (this == &p)
      return *this;
    x = p.x;
    y = p.y;
    return *this;
  }

  int x;
  int y;
};

// Является ли поле (j, i) выходом из лабиринта
bool is_exit(int H, int W, int i, int j, const vector<string> &maze) {
  return (i == 0 || i == H - 1 || j == 0 || j == W - 1) && maze[i][j] == ' ';
}

// Время, затраченное на прохождение поля (у песка - вдвое больше)
int weight(int H, int W, int i, int j, const vector<string> &maze) {
  assert(i >= 0 && i < H && j >= 0 && j < W);
  return maze[i][j] == '.' ? 2 : 1;
}

// Фактически обход лабиринта в ширину: для всех доступных из поля (j, i) полей смотрим,
// есть ли уже путь в это поле (если нет, то в routes лежит INT_MAX)
// Если же путь есть, проверяем, быстрее ли путь из поля (j, i).
// Если пути нет или новый путь быстрее, то прокладываем новый путь (в routes соответствующего поля кладем routes текущего + weight текущего)
// В таком случае кладем то поле в очередь и впоследствии обрабатываем
bool move(int H, int W, int i, int j, int y_diff, int x_diff,
          vector<vector<int>> &routes, const vector<string> &maze, std::queue<point> &Q) {
  assert(i >= 0 && i < H && j >= 0 && j < W);

  if (i + y_diff < H && i + y_diff >= 0 &&
      j + x_diff < W && j + x_diff >= 0 &&
      routes[i + y_diff][j + x_diff] > routes[i][j] + weight(H, W, i, j, maze) &&
      maze[i + y_diff][j + x_diff] != '#') {
    routes[i + y_diff][j + x_diff] = routes[i][j] + weight(H, W, i, j, maze);
    Q.push(point(j + x_diff, i + y_diff));
    return true;
  }

  return false;
}

// Восстановление пути из лабиринта
// Если из текущего поля (j, i) есть путь в соответствующее поле (j + x_diff, i + y_diff) и
// routes текущего поля больше routes соответствующего поля на weight соответствующего поля,
// возвращаемся в это поле
bool previous(int H, int W, int i, int j, int y_diff, int x_diff,
              const vector<vector<int>> &routes, const vector<string> &maze) {
  assert(i >= 0 && i < H && j >= 0 && j < W);

  return i + y_diff < H &&
      i + y_diff >= 0 &&
      j + x_diff < W &&
      j + x_diff >= 0 &&
      routes[i + y_diff][j + x_diff] == routes[i][j] - weight(H, W, i + y_diff, j + x_diff, maze);
}

int main() {

  vector<string> maze;
  point start, exit;

  string temp;
  while (cin) {
    std::getline(cin, temp);
    int start_pos = temp.find('s');
    if (start_pos != -1)
      start = point(start_pos, maze.size());
    maze.push_back(temp);

  }

  int H = maze.size(), W = maze[0].size();
  // routes - минимальное время пути из начальной точки.
  // Изначально для всех полей бесконечно, для s равно нулю
  vector<vector<int>> routes(H, vector<int>(W, INT_MAX));
  routes[start.y][start.x] = 0;

  vector<vector<bool>> visited(H, vector<bool>(W, false));

  bool escape = false;
  std::queue<point> Q;
  Q.push(start);
  while (!Q.empty()) {
    point cur = Q.front();
    Q.pop();
    if (is_exit(H, W, cur.y, cur.x, maze)) {
      exit = cur;
      escape = true;
      break;
    }
    if(weight(H, W, cur.y, cur.x, maze) == 2 && !visited[cur.y][cur.x]){
      visited[cur.y][cur.x] = true;
      Q.push(cur);
      continue;
    }
    if(visited[cur.y][cur.x])
      visited[cur.y][cur.x] = false;
    move(H, W, cur.y, cur.x, 1, 0, routes, maze, Q);
    move(H, W, cur.y, cur.x, 0, 1, routes, maze, Q);
    move(H, W, cur.y, cur.x, -1, 0, routes, maze, Q);
    move(H, W, cur.y, cur.x, 0, -1, routes, maze, Q);
  }

  if (!escape)
    return 1;

  point t = exit;

  while (true) {
    if (previous(H, W, t.y, t.x, 0, -1, routes, maze)) {
      maze[t.y][t.x--] = '*';
    } else if (previous(H, W, t.y, t.x, -1, 0, routes, maze)) {
      maze[t.y--][t.x] = '*';
    } else if (previous(H, W, t.y, t.x, 0, 1, routes, maze)) {
      maze[t.y][t.x++] = '*';
    } else if (previous(H, W, t.y, t.x, 1, 0, routes, maze)) {
      maze[t.y++][t.x] = '*';
    }

    if (t == start)
      break;
  }

  for (int i = 0; i < maze.size(); i++) {
    cout << maze[i];
    if (i < maze.size() - 1)
      cout << endl;
  }

  return 0;
}
