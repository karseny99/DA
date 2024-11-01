
// enum UnscopedColor { red, green, blue };
// enum class ScopedColor { red, green, blue };

// int main() {
//     [[maybe_unused]] UnscopedColor uc = red;
//     // [[maybe_unused]] ScopedColor sc = red; // Doesn't compile
//     [[maybe_unused]] ScopedColor sc = ScopedColor::red;
// }
как

// #include <type_traits>

// template<typename T>
// T foo([[maybe_unused]] T x, [[maybe_unused]] T y) {
//     if constexpr (std::is_same_v<T, int>) {
//         return x + y;
//     } else if constexpr (std::is_same_v<T, float>)  {
//         return x + 42;
//     } else {
//         return y + 42;
//     }
// }

// int main() {
//     foo(1, 2);
//     foo(1.f, 2.f);
//     foo(1., 2.);
// }


// #include <iostream>
// #include <type_traits>

// enum UnscopedColor { red, green, blue };
// enum class ScopedColor { red, green, blue };
// struct S{};

// int main() {
//     ScopedColor e = ScopedColor::red;
//     std::cout << std::boolalpha
//               << std::is_enum<UnscopedColor>::value << '\n'
//               << std::is_enum<ScopedColor>::value << '\n'
//               << std::is_enum_v<S> << '\n';
// }

// #include <iostream>
// #include <type_traits>

// enum UnscopedColor { red, green, blue };
// enum class ScopedColor { red, green, blue };
// enum class CharBasedColor : char { red = 'r', green = 'g', blue = 'b' };

// int main() {
 
//   constexpr bool isUnscopedColorInt = std::is_same_v< std::underlying_type<UnscopedColor>::type, int >;
//   constexpr bool isScopedColorInt = std::is_same_v< std::underlying_type_t<ScopedColor>, int >;
//   constexpr bool isCharBasedColorInt = std::is_same_v< std::underlying_type_t<CharBasedColor>, int >;
//   constexpr bool isCharBasedColorChar = std::is_same_v< std::underlying_type_t<CharBasedColor>, char >;

//   std::cout
//     << "underlying type for 'UnscopedColor' is " << (isUnscopedColorInt ? "int" : "non-int") << '\n'
//     << "underlying type for 'ScopedColor' is " << (isScopedColorInt ? "int" : "non-int") << '\n'
//     << "underlying type for 'CharBasedColor' is " << (isCharBasedColorInt ? "int" : "non-int") << '\n'
//     << "underlying type for 'CharBasedColor' is " << (isCharBasedColorChar ? "char" : "non-char") << '\n'
//     ;
// }

// #include <type_traits>

// enum struct ScopedColor { red, green, blue };
// enum class CharBasedColor : char { red = 'r', green = 'g', blue = 'b' };

// int main() {
//   using enum ScopedColor; // OK!
//   ScopedColor bval = red;
// //   using enum CharBasedColor; // error: 'CharBasedColor CharBasedColor::red' conflicts with a previous declaration
// }
// #include <iostream>
// namespace t {
//     enum ab : char {
//         a = '1', b = '2', c = '3'
//     };
// }


// int main() {
//     t::ab abc = t::ab::a;
//     std::cout << abc;
// }

#include <vector>
#include <algorithm>
#include <iostream>

// returns the number of divisors of `v`
constexpr int find_divisors_count(int v) {
    int res = 0;
    for (int i = 1; i <= v; ++i) {
        if (v % i == 0) {
            ++res;
        }
    }
    return res;
}                                                                       

struct A {
    int a, b, c;
};

// returns the biggest pair {divisors count of `v`, `v`}
constexpr std::pair<int, int> find_most_divisible(int n) {
    std::array<std::pair<int, int>, 1000> vec;
    for (int i = 1; i <= n; ++i) {
        vec[i] = {find_divisors_count(i), i};
    }
    std::sort(vec.begin(), vec.end());
    return vec.back();
}

int main() {
    A re;
    re.a = 10;
	std::cout << (find_most_divisible(1000) == std::make_pair(32, 840));
}