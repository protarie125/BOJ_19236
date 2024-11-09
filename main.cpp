#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

using ll = int64_t;

class Fish {
 public:
  ll r;
  ll c;
  ll d;
  bool l;
};

using vl = vector<ll>;
using vvl = vector<vl>;
using vF = vector<Fish>;
using vb = vector<bool>;
using vvb = vector<vb>;

ll ans = 0;
vvl Map(4, vl(4));
vF fish(17);

vl dr{-1, -1, 0, 1, 1, 1, 0, -1};
vl dc{0, -1, -1, -1, 0, 1, 1, 1};

void Move() {
  for (auto i = 1; i <= 16; ++i) {
    if (!fish[i].l) continue;

    ll r = fish[i].r;
    ll c = fish[i].c;
    ll dir = fish[i].d;

    ll nr = r + dr[dir];
    ll nc = c + dc[dir];
    bool moved = false;

    if (0 <= nr && nr < 4 && 0 <= nc && nc < 4) {
      if (Map[nr][nc] == 0) {
        fish[i].r = nr;
        fish[i].c = nc;

        Map[nr][nc] = i;
        Map[r][c] = 0;

        moved = true;
      } else if (Map[nr][nc] != -1) {
        ll tmpr = fish[Map[nr][nc]].r;
        ll tmpc = fish[Map[nr][nc]].c;
        fish[Map[nr][nc]].r = fish[i].r;
        fish[Map[nr][nc]].c = fish[i].c;
        fish[i].r = tmpr;
        fish[i].c = tmpc;

        ll tmp = Map[nr][nc];
        Map[nr][nc] = i;
        Map[r][c] = tmp;

        moved = true;
      }
    }

    if (!moved) {
      ll ndir = dir + 1;
      ndir %= 8;

      nr = r + dr[ndir];
      nc = c + dc[ndir];

      while (ndir != dir) {
        if (0 <= nr && nr < 4 && 0 <= nc && nc < 4) {
          if (Map[nr][nc] == 0) {
            fish[i].r = nr;
            fish[i].c = nc;

            Map[nr][nc] = i;
            Map[r][c] = 0;

            fish[i].d = ndir;
            moved = true;
            break;
          } else if (Map[nr][nc] != -1) {
            ll tmpr = fish[Map[nr][nc]].r;
            ll tmpc = fish[Map[nr][nc]].c;
            fish[Map[nr][nc]].r = fish[i].r;
            fish[Map[nr][nc]].c = fish[i].c;
            fish[i].r = tmpr;
            fish[i].c = tmpc;

            ll tmp = Map[nr][nc];
            Map[nr][nc] = i;
            Map[r][c] = tmp;

            fish[i].d = ndir;
            moved = true;
            break;
          }
        }

        ++ndir;
        ndir %= 8;

        nr = r + dr[ndir];
        nc = c + dc[ndir];
      }
    }
  }
}

void solve(ll r, ll c, ll dir, ll val) {
  ans = max(ans, val);

  vvl copy_map = Map;
  vF copy_fish = fish;

  Move();

  for (auto i = 1; i < 4; ++i) {
    const auto& nr = r + dr[dir] * i;
    const auto& nc = c + dc[dir] * i;

    if (nr < 0 || 4 <= nr || nc < 0 || 4 <= nc) break;
    if (Map[nr][nc] == 0) continue;

    ll fn = Map[nr][nc];
    ll ndir = fish[fn].d;

    Map[r][c] = 0;
    Map[nr][nc] = -1;
    fish[fn].l = false;
    solve(nr, nc, ndir, val + fn);

    Map[r][c] = -1;
    Map[nr][nc] = fn;
    fish[fn].l = true;
  }

  Map = copy_map;
  fish = copy_fish;
}

int main() {
  ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  for (auto i = 0; i < 4; ++i) {
    for (auto j = 0; j < 4; ++j) {
      ll a, b;
      cin >> a >> b;

      Map[i][j] = a;
      fish[a].r = i;
      fish[a].c = j;
      fish[a].d = b - 1;
      fish[a].l = true;
    }
  }

  auto fn = Map[0][0];
  auto fd = fish[fn].d;
  fish[fn].l = false;
  Map[0][0] = -1;
  solve(0, 0, fd, fn);
  cout << ans;

  return 0;
}