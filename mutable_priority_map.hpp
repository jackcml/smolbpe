#pragma once

#include <optional>
#include <set>
#include <unordered_map>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class MutablePriorityMap {
  private:
    std::unordered_map<Key, Value, Hash> m_values;
    std::set<std::pair<Value, Key>> m_ordered;

  public:
    MutablePriorityMap() : m_values(), m_ordered() {}

    std::pair<const Key &, const Value &> max();
    std::optional<Value> at(Key key);
    void insert_or_assign(Key key, Value value);
    void erase(Key key);
    bool empty();
};

template <typename Key, typename Value, typename Hash>
std::pair<const Key &, const Value &>
MutablePriorityMap<Key, Value, Hash>::max() {
    auto it = m_ordered.rbegin();
    return {it->second, it->first};
}

template <class Key, class Value, typename Hash>
std::optional<Value> MutablePriorityMap<Key, Value, Hash>::at(Key key) {
    return m_values[key];
}

template <typename Key, typename Value, typename Hash>
void MutablePriorityMap<Key, Value, Hash>::insert_or_assign(Key key,
                                                            Value value) {
    if (m_values.contains(key)) {
        m_ordered.erase({m_values[key], key});
    }
    m_values[key] = value;
    m_ordered.insert({value, key});
}

template <typename Key, typename Value, typename Hash>
void MutablePriorityMap<Key, Value, Hash>::erase(Key key) {
    m_ordered.erase({m_values[key], key});
    m_values.erase(key);
}

template <typename Key, typename Value, typename Hash>
bool MutablePriorityMap<Key, Value, Hash>::empty() {
    return m_values.empty();
}