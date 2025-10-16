#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class NDA {
public:
    NDA();
    NDA(NDA&&) noexcept = default;
    NDA& operator=(NDA&&) noexcept = default;
    NDA(const NDA&) = delete;
    NDA& operator=(const NDA&) = delete;

    void Build(const std::string& rpn);

    int LongestPrefix(const std::string& u) const;

private:
    struct Node {
        int id = -1;
        bool term = false;
        bool live = false;
        std::unordered_map<char, std::vector<Node*>> edges;
    };

    struct Frag {
        Node* start;
        Node* accept;
    };

    std::vector<std::unique_ptr<Node>> nodes;
    Node* start = nullptr;
    Node* accept = nullptr;

    Node* makeNode();
    Frag symbol(char ch);
    Frag uni(Frag left, Frag right);
    Frag concat(Frag left, Frag right);
    Frag star(Frag base);

    void markLive();

    using Bitset = std::vector<unsigned char>;
    void setAdd(Bitset& s, int id) const;
    bool setHas(const Bitset& s, int id) const;
    bool setEmpty(const Bitset& s) const;
    bool setIntersectsLive(const Bitset& s) const;

    void epsilonClosure(Bitset& s) const;
    Bitset moveOn(const Bitset& s, char ch) const;
};
