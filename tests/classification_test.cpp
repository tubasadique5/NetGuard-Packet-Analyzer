#include "types.h"
#include <cassert>
#include <iostream>

int main() {
    using DPI::AppType;
    using DPI::sniToAppType;

    assert(sniToAppType("www.twitter.com") == AppType::TWITTER);
    assert(sniToAppType("t.co") == AppType::TWITTER);
    assert(sniToAppType("www.microsoft.com") == AppType::MICROSOFT);
    assert(sniToAppType("www.netflix.com") == AppType::NETFLIX);
    assert(sniToAppType("www.youtube.com") == AppType::YOUTUBE);

    std::cout << "Domain classification tests passed.\\n";
    return 0;
}
