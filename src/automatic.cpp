#include "automatic.h"
#include <queue>
#include <stack>
#include <algorithm>

NDA::NDA() = default;

NDA::Node* NDA::makeNode() {
    nodes.push_back(std::make_unique<Node>());
    Node* p = nodes.back().get();
    p->id = static_cast<int>(nodes.size()) - 1;
    return p;
}

NDA::Frag NDA::symbol(char ch) {
    Node* s = makeNode();
    Node* t = makeNode();
    s->edges[ch].push_back(t);
    t->term = true;
    return {s, t};
}

NDA::Frag NDA::uni(Frag L, Frag R) {
    Node* s = makeNode();
    Node* t = makeNode();
    t->term = true;
    s->edges['1'].push_back(L.start);
    s->edges['1'].push_back(R.start);
    L.accept->term = false;
    R.accept->term = false;
    L.accept->edges['1'].push_back(t);
    R.accept->edges['1'].push_back(t);
    return {s, t};
}

NDA::Frag NDA::concat(Frag L, Frag R) {
    L.accept->term = false;
    L.accept->edges['1'].push_back(R.start);
    return {L.start, R.accept};
}

NDA::Frag NDA::star(Frag A) {
    Node* s = makeNode();
    Node* t = makeNode();
    t->term = true;
    s->edges['1'].push_back(A.start);
    s->edges['1'].push_back(t);
    A.accept->term = false;
    A.accept->edges['1'].push_back(A.start);
    A.accept->edges['1'].push_back(t);
    return {s, t};
}

void NDA::Build(const std::string& rpn) {
    nodes.clear();
    start = accept = nullptr;

    std::stack<Frag> st;
    for (char tok : rpn) {
        switch (tok) {
            case 'a': case 'b': case 'c': case '1':
                st.push(symbol(tok));
                break;
            case '+': {
                if (st.size() < 2) throw std::runtime_error("RPN error: '+' needs two operands");
                auto r = st.top(); st.pop();
                auto l = st.top(); st.pop();
                st.push(uni(l, r));
                break;
            }
            case '.': {
                if (st.size() < 2) throw std::runtime_error("RPN error: '.' needs two operands");
                auto r = st.top(); st.pop();
                auto l = st.top(); st.pop();
                st.push(concat(l, r));
                break;
            }
            case '*': {
                if (st.empty()) throw std::runtime_error("RPN error: '*' needs one operand");
                auto a = st.top(); st.pop();
                st.push(star(a));
                break;
            }
            default:
                throw std::runtime_error("RPN error: unknown token");
        }
    }

    if (st.size() != 1) throw std::runtime_error("RPN error: leftover operands/operators");

    auto res = st.top();
    start = res.start;
    accept = res.accept;

    markLive();
}

void NDA::markLive() {
    const int n = static_cast<int>(nodes.size());
    std::vector<std::vector<int>> rev(n);
    for (const auto& up : nodes) {
        const Node* u = up.get();
        for (const auto& kv : u->edges) {
            for (Node* v : kv.second) {
                rev[v->id].push_back(u->id);
            }
        }
    }

    for (auto& up : nodes) up->live = false;

    std::queue<int> q;
    for (const auto& up : nodes) if (up->term) { up->live = true; q.push(up->id); }

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : rev[v]) {
            if (!nodes[u]->live) { nodes[u]->live = true; q.push(u); }
        }
    }
}

void NDA::setAdd(Bitset& s, int id) const {
    if (id < 0) return;
    if (id >= static_cast<int>(s.size())) s.resize(id + 1, 0);
    s[id] = 1;
}

bool NDA::setHas(const Bitset& s, int id) const {
    return id >= 0 && id < static_cast<int>(s.size()) && s[id] != 0;
}

bool NDA::setEmpty(const Bitset& s) const {
    for (unsigned char v : s) if (v) return false; return true;
}

bool NDA::setIntersectsLive(const Bitset& s) const {
    int n = static_cast<int>(nodes.size());
    int m = std::min<int>(n, s.size());
    for (int i = 0; i < m; ++i) if (s[i] && nodes[i]->live) return true;
    return false;
}

void NDA::epsilonClosure(Bitset& s) const {
    std::queue<int> q;
    int n = static_cast<int>(std::max<size_t>(s.size(), nodes.size()));
    for (int i = 0; i < n; ++i) if (i < (int)s.size() && s[i]) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u < 0 || u >= (int)nodes.size()) continue;
        auto it = nodes[u]->edges.find('1');
        if (it == nodes[u]->edges.end()) continue;
        for (Node* v : it->second) {
            if (!setHas(s, v->id)) { setAdd(s, v->id); q.push(v->id); }
        }
    }
}

NDA::Bitset NDA::moveOn(const Bitset& s, char ch) const {
    Bitset nxt;
    int n = std::min<int>(nodes.size(), s.size());
    for (int u = 0; u < n; ++u) if (s[u]) {
        auto it = nodes[u]->edges.find(ch);
        if (it != nodes[u]->edges.end()) {
            for (Node* v : it->second) setAdd(nxt, v->id);
        }
    }
    epsilonClosure(nxt);
    return nxt;
}

int NDA::LongestPrefix(const std::string& u) const {
    if (!start) return 0;
    Bitset cur; setAdd(cur, start->id); epsilonClosure(cur);

    int best = 0;
    if (setIntersectsLive(cur)) best = 0;

    for (int i = 0; i < (int)u.size(); ++i) {
        if (!setEmpty(cur) && setIntersectsLive(cur)) best = i;
        char ch = u[i];
        if (ch != 'a' && ch != 'b' && ch != 'c')
            throw std::runtime_error("input word contains invalid symbol");
        cur = moveOn(cur, ch);
        if (setEmpty(cur)) return best;
    }
    if (setIntersectsLive(cur)) best = (int)u.size();
    return best;
}