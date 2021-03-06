#include "auto_util_header.hpp"

// #define ENABLE_UPD
class SegTreeMin {
	using val_t = int;
	static const val_t VALMAX = INT_MAX;
	static const int IDXMAX = INT_MAX;
private:
	struct idxval_t {
		int idx;
		val_t val;
		bool operator<(const idxval_t &another) const {
			return val != another.val ? val < another.val : idx < another.idx;
		}
	};
	struct segval_t {
		bool enable;
		val_t upd, add;
		idxval_t min;
	};
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<segval_t> nodes;
	vi idl, idr;
	void merge(int id) {
		val_t val_l = nodes[idl[id]].min.val + nodes[idl[id]].add;
		val_t val_r = nodes[idr[id]].min.val + nodes[idr[id]].add;
		nodes[id].min.idx = (val_l <= val_r ? nodes[idl[id]].min.idx : nodes[idr[id]].min.idx);
		nodes[id].min.val = std::min(val_l, val_r);
	}
#ifdef ENABLE_UPD
	void lazy(int id, int l, int r) {
		if (id >= base) return;
		if (nodes[id].enable) {
			val_t upd = nodes[id].upd + nodes[id].add;
			nodes[idl[id]] = { true, upd, 0, {l, upd} };
			nodes[idr[id]] = { true, upd, 0, {(l + r) >> 1, upd} };
			nodes[id] = { false, 0, 0, {l, upd} };
		}
		else {
			nodes[idl[id]].add += nodes[id].add;
			nodes[idr[id]].add += nodes[id].add;
			nodes[id].add = 0;
			merge(id);
		}
	}
#endif
	enum change_t {
		ADD,
#ifdef ENABLE_UPD
		UPD
#endif
	};
	void change_rec(int s, int t, int l, int r, int id, val_t x, change_t op) {
		if (s == l && t == r) {
#ifdef ENABLE_UPD
			if (op == ADD) nodes[id].add += x;
			else if (op == UPD) nodes[id] = { true, x, 0, {s, x} };
#else
			nodes[id].add += x;
#endif
		}
		else {
#ifdef ENABLE_UPD
			lazy(id, l, r);
#endif
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				change_rec(s, m, l, m, idl[id], x, op);
				change_rec(m, t, m, r, idr[id], x, op);
			}
			else if (s < m) {
				change_rec(s, t, l, m, idl[id], x, op);
			}
			else if (m < t) {
				change_rec(s, t, m, r, idr[id], x, op);
			}
			merge(id);
		}
	}
	idxval_t solve_rec(int s, int t, int l, int r, int id) {
		idxval_t v;
		if (s == l && t == r) {
			v = nodes[id].min;
		}
		else {
#ifdef ENABLE_UPD
			lazy(id, l, r);
#endif
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				idxval_t v0 = solve_rec(s, m, l, m, idl[id]);
				idxval_t v1 = solve_rec(m, t, m, r, idr[id]);
				v = std::min(v0, v1);
			}
			else if (s < m) {
				v = solve_rec(s, t, l, m, idl[id]);
			}
			else if (m < t) {
				v = solve_rec(s, t, m, r, idr[id]);
			}
		}
		v.val += nodes[id].add;
		return v;
	}
	void common_init() {
		idl.resize(base + N, -1);
		idr.resize(base + N, -1);
		Loop(i, base) {
			idl[i] = (i << 1) + 1;
			idr[i] = (i << 1) + 2;
		}
	}
public:
	SegTreeMin(int n, val_t init = VALMAX) {
		this->n = n;
		this->N = 1 << ceillog2(n);
		this->base = N - 1;
		this->nodes = vector<segval_t>(base + N, { false, 0, 0, {IDXMAX, VALMAX} });
		common_init();
		Loop(i, n) {
			this->nodes[base + i] = { true, init, 0, {i, init} };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
	SegTreeMin(const vector<val_t> &a) {
		this->n = int(a.size());
		this->N = 1 << ceillog2(n);
		this->base = N - 1;
		this->nodes = vector<segval_t>(base + N, { false, 0, 0, {IDXMAX, VALMAX} });
		common_init();
		Loop(i, n) {
			this->nodes[base + i] = { true, a[i], 0, {i, a[i]} };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
#ifdef ENABLE_UPD
	void upd(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, UPD);
	}
#endif
	void add(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, ADD);
	}
	// the smallest argmin_i\in[s, t) a[i]
	int minidx(int s, int t) {
		if (s >= t) return IDXMAX;
		return solve_rec(s, t, 0, N, 0).idx;
	}
	val_t minval(int s, int t) {
		if (s >= t) return VALMAX;
		return solve_rec(s, t, 0, N, 0).val;
	}
	// the smallest argmin_i\in[s, t) a[i], and its value
	idxval_t minidxval(int s, int t) {
		if (s >= t) return {IDXMAX, VALMAX};
		return solve_rec(s, t, 0, N, 0);
	}
};
#undef ENABLE_UPD