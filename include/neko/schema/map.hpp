#pragma once

#include <neko/schema/types.hpp>

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

namespace neko {
    template <typename Key, typename Value, neko::uint64 N>
    struct ConstexprMap {

        std::array<std::pair<Key, Value>, N> data;

        constexpr std::optional<Value> find(const Key &key) const {
            for (const auto &kv : data) {
                if (kv.first == key) {
                    return kv.second;
                }
            }
            return std::nullopt;
        }

        constexpr neko::uint64 size() const noexcept { return N; }
        constexpr bool empty() const noexcept { return N == 0; }

        constexpr auto begin() const { return data.begin(); }
        constexpr auto end() const { return data.end(); }
    };

    template <typename Key, typename Value, std::size_t N>
    ConstexprMap(std::array<std::pair<Key, Value>, N>) 
        -> ConstexprMap<Key, Value, static_cast<neko::uint64>(N)>;

    
} // namespace neko
