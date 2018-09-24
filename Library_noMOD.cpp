ll powll(ll n, ll p) {
  if (p == 0) return 1;
  else if (p == 1) return n;
  else {
    ll ans = powll(n, p / 2);
    ans = ans * ans;
    if (p % 2 == 1) ans = ans * n;
    return ans;
  }
}

//A[k] = k!
vll factlist(ll n) {
  if (n < 0) return{};
  else {
    vll ret((int)n + 1);
    ret[0] = 1;
    Loop1(i, (int)n) ret[i] = ret[i - 1] * i;
    return ret;
  }
}

//A[n][r] = nCr 
vvll combinationlist(int n) {
  vvll ret(n, vll(n, 1));
  Loop1(i, n - 1) {
    Loop1(j, i - 1) {
      ret[i][j] = ret[i - 1][j - 1] + ret[i - 1][j];
    }
  }
  return ret;
}

// ret[i] = a[i] + a[i + 1] + ... (for length times, with looping)
vll loop_vec_accumulate(vll a, ll length) {
  int n = a.size();
  vll ret(n, 0);
  if (n == 0) return ret;
  ll p = length / n;
  if (p > 0) {
    Loop(i, n) ret[0] += a[i];
    ret[0] *= p;
  }
  Loop(i, length % n) ret[0] += a[i];
  Loop1(i, n - 1) {
    ret[i] = ret[i - 1] - a[i - 1] + a[(i + length - 1) % n];
  }
  return ret;
}

vvll loop_mx_accumulate(vvll A, ll i_length, ll j_length) {
  int m = A.size();
  int n = A[0].size();
  Loop(i, m) A[i] = loop_vec_accumulate(A[i], j_length);
  vvll trans_A(n, vll(m, 0));
  Loop(i, n) {
    Loop(j, m) trans_A[i][j] = A[j][i];
  }
  Loop(i, n) trans_A[i] = loop_vec_accumulate(trans_A[i], i_length);
  Loop(i, m) {
    Loop(j, n) A[i][j] = trans_A[j][i];
  }
  return A;
}

vll divisors(ll x) {
  vll ret;
  Loop1(i, x) {
    ll y = (ll)i * i;
    if (y >= x) {
      if (y == x) ret.push_back(i);
      break;
    }
    else {
      if (x % i == 0) {
        ret.push_back(i);
        ret.push_back(x / i);
      }
    }
  }
  return ret;
}

// n = 1.5e7 -> 80 ms
vll list_prime_until(ll n) {
	vll ret;
	vector<bool> is_prime(n + 1, true);
	if (is_prime.size() > 0) is_prime[0] = false;
	if (is_prime.size() > 1) is_prime[1] = false;
	Loop(i, n + 1) {
		if (is_prime[i]) {
			ret.push_back(i);
			int k = (i << 1);
			while (k < n + 1) {
				is_prime[k] = false;
				k += i;
			}
		}
	}
	return ret;
}

// prime_list has to be generated by list_prime_until(>=sqrt(n))
vector<Pll> prime_factorize(ll n, vll &prime_list) {
  vector<Pll> ret;
  Loop(i, prime_list.size()) {
    if (n == 1) break;
    while (n % prime_list[i] == 0) {
      if (ret.size() == 0 || ret.back().first != prime_list[i]) {
        ret.push_back({ prime_list[i], 0 });
      }
      ret.back().second++;
      n /= prime_list[i];
    }
  }
  if (n != 1) ret.push_back({ n, 1 });
  return ret;
}


pair<vector<Pll>, vector<Pll>> reduce_common_factors(const vector<Pll> &x_factors, const vector<Pll> &y_factors) {
	pair<vector<Pll>, vector<Pll>> ret;
	int n = int(x_factors.size()), m = int(y_factors.size());
	int i = 0, j = 0;
	while (i < n && j < m) {
		if (x_factors[i].first < y_factors[j].first) ret.first.push_back(x_factors[i++]);
		else if (x_factors[i].first > y_factors[j].first) ret.second.push_back(y_factors[j++]);
		else {
			ll cnt = min(x_factors[i].second, y_factors[i].second);
			if (x_factors[i].second - cnt > 0) ret.first.push_back({ x_factors[i].first, x_factors[i].second - cnt });
			if (y_factors[j].second - cnt > 0) ret.second.push_back({ y_factors[j].first, y_factors[j].second - cnt });
			++i; ++j;
		}
	}
	while (i < n) ret.first.push_back(x_factors[i++]);
	while (j < m) ret.second.push_back(y_factors[j++]);
	return ret;
}

pair<ll, ll> reduce_fraction(ll x, ll y, const vll &prime_list) {
	pair<ll, ll> ret = { 1, 1 };
	vector<Pll> x_factors = prime_factorize(x, prime_list);
	vector<Pll> y_factors = prime_factorize(y, prime_list);
	pair<vector<Pll>, vector<Pll>> factors = reduce_common_factors(x_factors, y_factors);
	Loop(i, factors.first.size()) ret.first *= powll(factors.first[i].first, factors.first[i].second);
	Loop(i, factors.second.size()) ret.second *= powll(factors.second[i].first, factors.second[i].second);
	return ret;
}