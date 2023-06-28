#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include "HashTable.h"

HashTable::HashTable(size_t size) noexcept 
    : _filled{0},  _capacity{static_cast<int32_t>(size)}, table{} {
    table.reserve(_capacity);
    table.resize(_capacity);
}

void HashTable::insert(const KeyType &key, const ValueType &value) {
    size_t hash = hash_function(key);
    size_t index = get_index(hash);
    bool ok = false;
    for (auto it = table.at(index).begin(); it != table.at(index).end();) {
        if (it->first == key) {
            it->second = value;
            ok = true;
            break;
        }else{
            ++it;
        }
    }
    if (!ok) { 
        if (getLoadFactor() > 0.75) {
            HashTable new_table{static_cast<size_t>(_capacity << 1)};
            for (auto & list : table) {
                for (auto & keyValue : list) {
                    new_table.insert_to_table(keyValue.first, keyValue.second);
                    new_table._filled++;
                } 
            }
            table = new_table.table;
            _capacity = new_table._capacity;
        }
        insert_to_table(key, value);
        _filled++;
    }
}

void HashTable::insert_to_table(const KeyType &key, const ValueType &value) {
    size_t hash = hash_function(key);
    size_t index = get_index(hash);
    table.at(index).push_back({key, value});
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t hash = hash_function(key);
    size_t index = get_index(hash);
    for (const auto & keyVal : table.at(index)) {
        if (key == keyVal.first) {
            value = keyVal.second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key) {
    size_t hash = hash_function(key);
    size_t index = get_index(hash);
    for (auto it = table.at(index).begin(); it != table.at(index).end();) {
        if (it->first == key) {
            it = table.at(index).erase(it);
            _filled--;
            break;
        }else{
            ++it;
        }
    }
}

ValueType& HashTable::operator[](const KeyType &key) {
    size_t hash = hash_function(key);
    size_t index = get_index(hash);
    for (auto & keyVal : table.at(index)) {
        if (key == keyVal.first) {
            return keyVal.second;
        }
    }
}

size_t HashTable::hash_function(const KeyType &key) const {
    size_t hash = std::hash<KeyType>{}(key);
    return hash;
}

size_t HashTable::get_index(const size_t &hash) const {
    size_t index = static_cast<size_t>(hash % _capacity);
    return index;
}

double HashTable::getLoadFactor() {
    double loadFactor = static_cast<double>(_filled) / _capacity;
    return loadFactor;
}

// int main() {
//     HashTable hashTable(1);
//     ValueType a = 2;
//     ValueType b = 4;
//     hashTable.insert("123", a);
//     hashTable.insert("123", b);
//     std::cout << hashTable.find("123", a) << '\n';
//     std::cout << hashTable.find("123", b) << '\n';
//     std::cout << '\n' << hashTable._capacity << '\n';
    // std::cout << hashTable.find("13") << '\n';
    // hashTable.insert("123", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("13", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("12", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("14", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("15", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("16", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // hashTable.insert("17", 2);
    // std::cout << '\n' << hashTable._capacity << '\n';
    // std::cout << hashTable.find("123") << '\n';
    // std::cout << hashTable.find("13") << '\n';
    // std::cout << hashTable.find("12") << '\n';
    // std::cout << hashTable.find("14") << '\n';
    // std::cout << hashTable.find("15") << '\n';
//     return 0;
// }