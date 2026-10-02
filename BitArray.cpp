#include "BitArray.h"


BitArray::BitArray() {
    arr_ = nullptr;
    num_bits_ = 0;
    num_blocks_ = 0;
}


int count_blocks(int num_bits) {
    if (num_bits % BitArray::BITS_PER_BLOCK == 0)
        return num_bits / BitArray::BITS_PER_BLOCK;
    return num_bits / BitArray::BITS_PER_BLOCK + 1;
}


BitArray::BitArray(int num_bits, unsigned long value): arr_(nullptr), num_bits_(0), num_blocks_(0) {

    if (num_bits < 0)
        throw std::length_error("The number of bits must be non-negative");

    if (num_bits > 0) {

        num_bits_ = num_bits;
        num_blocks_ = count_blocks(num_bits);

        arr_ = new unsigned long[num_blocks_]{};
        arr_[0] = value;

        remove_extra_bits();
        }
    }


BitArray::~BitArray() {
    delete[] arr_;
}


void BitArray::swap(BitArray& b) {

    int tmp = num_bits_;
    num_bits_ = b.num_bits_;
    b.num_bits_ = tmp;

    tmp = num_blocks_;
    num_blocks_ = b.num_blocks_;
    b.num_blocks_ = tmp;

    unsigned long* tmp_arr = arr_;
    arr_ = b.arr_;
    b.arr_ = tmp_arr;
}


BitArray::BitArray(const BitArray& b): arr_(nullptr), num_bits_(b.num_bits_), num_blocks_(b.num_blocks_) {

    if (num_bits_ > 0) {
        arr_ = new unsigned long[num_blocks_]{};

        for (int i = 0; i < num_blocks_; i++)
            arr_[i] = b.arr_[i];
    }
}


BitArray& BitArray::operator=(const BitArray& b) {

    if (this == &b)
        return *this;

    unsigned long* new_arr  = nullptr;
    if (b.num_blocks_ > 0) {
        new_arr = new unsigned long[b.num_blocks_]{};

        for (int i = 0; i < b.num_blocks_; i++)
            new_arr[i] = b.arr_[i];
    }

    delete[] arr_;
    arr_ = new_arr;
    num_bits_ = b.num_bits_;
    num_blocks_ = b.num_blocks_;
    return *this;
}


BitArray& BitArray::set(int n, bool val) {

    int block_idx = n / BITS_PER_BLOCK;
    int bit_idx = n % BITS_PER_BLOCK;

    if (val == false)
        arr_[block_idx] &= ~(1UL << (bit_idx));
    else
        arr_[block_idx] |= (1UL << (bit_idx));

    return *this;
}



BitArray& BitArray::set() {

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] = ~0UL;

    remove_extra_bits();
    return *this;
}


BitArray& BitArray::reset(int n) {

    int block_idx = n / BITS_PER_BLOCK;
    int bit_idx = n % BITS_PER_BLOCK;

    arr_[block_idx] &= ~(1UL << (bit_idx));

    return *this;
}


BitArray& BitArray::reset() {

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] = 0UL;

    return *this;
}


bool BitArray::operator[](int i) const {

    int block_idx = i / BITS_PER_BLOCK;
    int bit_idx = i % BITS_PER_BLOCK;

    return (arr_[block_idx] >> bit_idx) & 1UL;
}


int BitArray::size() const {
    return num_bits_;
}


bool BitArray::empty() const {
    return num_bits_ == 0;
}


std::string BitArray::to_string() const {

    std::string str;
    for (int i = num_bits_ - 1; i >= 0; i--) {
        if ((*this)[i])
            str.push_back('1');
        else
            str.push_back('0');
    }
    return str;
}


void BitArray::clear() {

    delete[] arr_;
    arr_ = nullptr;
    num_bits_ = 0;
    num_blocks_ = 0;
}


void BitArray::remove_extra_bits() {

    int remainder = num_bits_% BITS_PER_BLOCK;
    if (remainder != 0 && num_blocks_ > 0)
        arr_[num_blocks_ - 1] &= (1UL << remainder) - 1UL;

}


bool BitArray::none() const {

    for (int i = num_blocks_ - 1; i >= 0; i--) {
        if (arr_[i])
            return false;
    }
    return true;
}


bool BitArray::any() const {
    return !none();
}


BitArray BitArray::operator~() const{

    BitArray res(*this);

    for (int i = 0; i < num_blocks_; i++)
        res.arr_[i] = ~res.arr_[i];

    res.remove_extra_bits();
    return res;
}


int BitArray::count() const {

    int cnt = 0;
    for (int i = 0; i < num_blocks_; i++) {

        unsigned long copy = arr_[i];

        while (copy) {
            copy &= (copy - 1);
            cnt++;
        }
    }
    return cnt;
}


bool operator==(const BitArray & a, const BitArray & b) {

    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}


bool operator!=(const BitArray & a, const BitArray & b) {
    return !(a == b);
}


BitArray& BitArray::operator&=(const BitArray& b) {

    if (num_bits_ != b.num_bits_)
        throw std::length_error("The lengths of the bit arrays must match.");

    for (int i = 0; i < num_blocks_; i++)
       arr_[i] &= b.arr_[i];

    return *this;
}


BitArray& BitArray::operator|=(const BitArray& b){

    if (num_bits_ != b.num_bits_)
        throw std::length_error("The lengths of the bit arrays must match.");

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] |= b.arr_[i];

    return *this;
}


BitArray& BitArray::operator^=(const BitArray& b){

    if (num_bits_ != b.num_bits_)
        throw std::length_error("The lengths of the bit arrays must match.");

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] ^= b.arr_[i];

    return *this;
}



BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray res(b1);
    return res &= b2;
}


BitArray operator|(const BitArray& b1, const BitArray& b2){
    BitArray res(b1);
    return res |= b2;
}


BitArray operator^(const BitArray& b1, const BitArray& b2){
    BitArray res(b1);
    return res ^= b2;
}


BitArray& BitArray::operator<<=(int n) {
    if (n < 0)
        throw std::invalid_argument("Shift count cannot be negative");

    if (n == 0)
        return *this;

    if (n >= num_bits_)
        return reset();

    int block_shift = n / BITS_PER_BLOCK;
    int bit_shift   = n % BITS_PER_BLOCK;

    if (block_shift > 0) {

        for (int i = num_blocks_ - 1; i >= block_shift; i--)
            arr_[i] = arr_[i - block_shift];

        for (int i = 0; i < block_shift; ++i)
            arr_[i] = 0UL;

    }

    if (bit_shift != 0) {

        for (int i = num_blocks_ - 1; i > block_shift; i--)
            arr_[i] = (arr_[i] << bit_shift) | (arr_[i - 1] >> (BITS_PER_BLOCK - bit_shift));

        arr_[block_shift] <<= bit_shift;
    }
    remove_extra_bits();
    return *this;
}

BitArray& BitArray::operator>>=(int n){
    if (n < 0)
        throw std::invalid_argument("Shift count cannot be negative");

    if (n == 0)
        return *this;

    if (n >= num_bits_)
        return reset();

    int block_shift = n / BITS_PER_BLOCK;
    int bit_shift   = n % BITS_PER_BLOCK;

    if (block_shift > 0) {

        for (int i = 0; i < num_blocks_ - block_shift; i++)
            arr_[i] = arr_[i + block_shift];

        for (int i = num_blocks_ - 1; i >= num_blocks_ - block_shift; i--)
            arr_[i] = 0UL;

    }

    if (bit_shift != 0) {

        for (int i = 0; i < num_blocks_ - block_shift - 1; i++)
            arr_[i] = (arr_[i] >> bit_shift) | (arr_[i + 1] << (BITS_PER_BLOCK - bit_shift));

        arr_[num_blocks_ - block_shift - 1] >>= bit_shift;
    }
    remove_extra_bits();
    return *this;
}


BitArray BitArray::operator<<(int n) const{
    BitArray res(*this);
    return res <<= n ;
}


BitArray BitArray::operator>>(int n) const {
    BitArray res(*this);
    return res >>= n ;
}


void BitArray::resize(int new_num_bits, bool value){

    if (new_num_bits < 0)
        throw std::length_error("The number of bits must be non-negative");

    if (new_num_bits == num_bits_)
        return;

    if (new_num_bits == 0) {
        clear();
        return;
    }

    if (new_num_bits < num_bits_) {

        int new_num_blocks = count_blocks(new_num_bits);

        if (new_num_blocks < num_blocks_) {
            auto* new_arr = new unsigned long[new_num_blocks]{};

            for (int i = 0; i < new_num_blocks; i++)
                new_arr[i] = arr_[i];

            delete[] arr_;
            arr_ = new_arr;
            num_blocks_ = new_num_blocks;
        }

        num_bits_ = new_num_bits;
        remove_extra_bits();
        return;
    }


    int new_num_blocks = count_blocks(new_num_bits);

    if (new_num_blocks > num_blocks_) {
        auto* new_arr = new unsigned long[new_num_blocks]{};

        if (num_blocks_ != 0) {
            for (int i = 0; i < num_blocks_; i++)
                new_arr[i] = arr_[i];
        }

        delete[] arr_;
        arr_ = new_arr;
        num_blocks_ = new_num_blocks;
    }

    int old_num_bits = num_bits_;
    num_bits_ = new_num_bits;
    for (int i = old_num_bits; i < num_bits_; i++)
        set(i,value);
}


void BitArray::push_back(bool bit) {

    if (num_bits_ % BITS_PER_BLOCK == 0) {
        auto* new_arr = new unsigned long[num_blocks_ + 1]{};

        if (num_blocks_ != 0) {
            for (int i = 0; i < num_blocks_; i++)
                new_arr[i] = arr_[i];
        }

        delete[] arr_;
        arr_ = new_arr;
        num_blocks_++;

    }

    num_bits_++;
    set(num_bits_ - 1,bit);
}

void PrintBitArray(BitArray a) {
    for (int i = a.size() - 1; i >= 0; i--) {
        std::cout << a[i];
    }
    std::cout << std::endl;
}


