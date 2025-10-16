#include <iostream>
#include <string>
#include "automatic.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string rpn, u;
    if (!(std::cin >> rpn)) return 0;
    if (!(std::cin >> u)) u = "";

    NDA nda;
    try {
        nda.Build(rpn);
        int ans = nda.LongestPrefix(u);
        std::cout << ans << '\n';
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}