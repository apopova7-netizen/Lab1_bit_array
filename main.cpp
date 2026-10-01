#include <string>
#include <iostream>
#include <bitset>


class BitArray
{
private:
  unsigned long* arr_;
  int num_bits_;
  int num_blocks_;

  static const int BITS_PER_BLOCK = sizeof(unsigned long) * 8;
public:
  BitArray();//Конструирует массив, хранящий заданное количество бит. ***
             //Первые sizeof(long) бит можно инициализровать с помощью параметра value. ***
  ~BitArray();// деструктор ***
  explicit BitArray(int num_bits, unsigned long value = 0); //конструктор с заданными параметрами ***
  BitArray(const BitArray& b); // Конструктор копирования ***
  void swap(BitArray& b);//Обменивает значения двух битовых массивов.***
  BitArray& operator=(const BitArray& b); // Копирующее присваивание ***


  //Изменяет размер массива. В случае расширения, новые элементы 
  //инициализируются значением value.
  void resize(int new_num_bits, bool value = false);


  //Очищает массив.
  void clear(); // ***


  //Добавляет новый бит в конец массива. В случае необходимости 
  //происходит перераспределение памяти.
  void push_back(bool bit); //***


  //Битовые операции над массивами.
  //Работают только на массивах одинакового размера.
  //Обоснование реакции на параметр неверного размера входит в задачу.
  BitArray& operator&=(const BitArray& b); // ***
  BitArray& operator|=(const BitArray& b); // ***
  BitArray& operator^=(const BitArray& b); // ***
 
  //Битовый сдвиг с заполнением нулями.
  BitArray& operator<<=(int n); // ***
  BitArray& operator>>=(int n); // ***
  BitArray operator<<(int n) const; //***
  BitArray operator>>(int n) const; // ***


  //Устанавливает бит с индексом n в значение val.
  BitArray& set(int n, bool val = true); // ***
  //Заполняет массив истиной.
  BitArray& set(); // ***

  //Устанавливает бит с индексом n в значение false.
  BitArray& reset(int n); // ***
  //Заполняет массив ложью.
  BitArray& reset(); // ***

  //true, если массив содержит истинный бит.
  bool any() const; // ***
  //true, если все биты массива ложны.
  bool none() const; // ***

  //Битовая инверсия
  BitArray operator~() const; // ***
  //Подсчитывает количество единичных бит.
  int count() const; // ***


  //Возвращает значение бита по индексу i.
  bool operator[](int i) const;// ***

  int size() const;// ***
  bool empty() const;// ***
  
  //Возвращает строковое представление массива.
  std::string to_string() const; //***
};

bool operator==(const BitArray & a, const BitArray & b); // ***
bool operator!=(const BitArray & a, const BitArray & b); // ***

BitArray operator&(const BitArray& b1, const BitArray& b2); //***
BitArray operator|(const BitArray& b1, const BitArray& b2); //***
BitArray operator^(const BitArray& b1, const BitArray& b2); //***

BitArray::BitArray() {
    arr_ = nullptr;
    num_bits_ = 0;
    num_blocks_ = 0;
}

BitArray::BitArray(int num_bits, unsigned long value): arr_(nullptr), num_bits_(0) {

    if (num_bits < 0)
        throw std::length_error("The number of bits must be non-negative");

    if (num_bits > 0) {

        num_bits_ = num_bits;

        if (num_bits % BITS_PER_BLOCK == 0)
            num_blocks_ = num_bits_ / BITS_PER_BLOCK;
        else
            num_blocks_ = num_bits_ / BITS_PER_BLOCK + 1;

        arr_ = new unsigned long[num_blocks_]{};
        arr_[0] = value;

        if (num_bits_ < BITS_PER_BLOCK) {
            unsigned long mask = (1UL << num_bits_) - 1UL;
            arr_[0] &= mask;
            }
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

BitArray::BitArray(const BitArray& b): arr_(nullptr), num_bits_(b.num_bits_), num_blocks_(b.num_blocks_){

    if (num_bits_ > 0)
        arr_ = new unsigned long[num_blocks_]{};

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] = b.arr_[i];

}

BitArray& BitArray::operator=(const BitArray& b) {

    if (this == &b)
        return *this;

    unsigned long* new_arr  = nullptr;
    if (b.num_blocks_ > 0)
        new_arr = new unsigned long[b.num_blocks_];

    for (int i = 0; i < b.num_blocks_; i++)
        new_arr[i] = b.arr_[i];

    delete[] arr_;
    arr_ = new_arr;
    num_bits_ = b.num_bits_;
    num_blocks_ = b.num_blocks_;
    return *this;
}

//Устанавливает бит с индексом n в значение val.
BitArray& BitArray::set(int n, bool val) {
    int block_idx = n / BITS_PER_BLOCK;
    int bit_idx = n % BITS_PER_BLOCK;
    if (val == false)
        arr_[block_idx] &= ~(1UL << (bit_idx));
    else
        arr_[block_idx] |= (1UL << (bit_idx));

    return *this;

}
//Заполняет массив истиной.
BitArray& BitArray::set() {

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] = ~0UL;

   int remainder = num_bits_% BITS_PER_BLOCK;

    if (remainder != 0)
        arr_[num_blocks_ - 1] &= (1UL << remainder) - 1UL;
    return *this;
}

//Устанавливает бит с индексом n в значение false.
BitArray& BitArray::reset(int n) {

    int block_idx = n / BITS_PER_BLOCK;
    int bit_idx = n % BITS_PER_BLOCK;
    arr_[block_idx] &= ~(1UL << (bit_idx));

    return *this;
}
//Заполняет массив ложью.
BitArray& BitArray::reset() {

    for (int i = 0; i < num_blocks_; i++)
        arr_[i] = 0UL;
    return *this;
}
//Возвращает значение бита по индексу i.
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


//true, если все биты массива ложны.
bool BitArray::none() const {
    for (int i = num_blocks_ - 1; i >= 0; i--) {
        if (arr_[i])
            return false;
    }
    return true;
}

//true, если массив содержит истинный бит.
bool BitArray::any() const {
    return !none();
}

//Битовая инверсия
BitArray BitArray::operator~() const{

    BitArray res(*this);

    for (int i = 0; i < num_blocks_; i++) {
        res.arr_[i] = ~res.arr_[i];
    }

    int remainder = num_bits_ % BITS_PER_BLOCK;
    if (remainder != 0)
        res.arr_[num_blocks_ - 1] &= (1ul << remainder) - 1UL;

    return res;

}
//Подсчитывает количество единичных бит.
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
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;

    }
    return true;
}

bool operator!=(const BitArray & a, const BitArray & b) {
    return !(a == b);
}

//Битовые операции над массивами.
//Работают только на массивах одинакового размера.
//Обоснование реакции на параметр неверного размера входит в задачу.
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


//Битовый сдвиг с заполнением нулями.
BitArray& BitArray::operator<<=(int n){

    if (n < 0)
        throw std::invalid_argument("Shift count cannot be negative.");


    if (n >= num_bits_)
        return reset();

    for (int i = num_bits_ - 1; i >= 0; i--) {

        if (i >= n )
            set(i, (*this)[i - n]);
        else
            reset(i);

    }
    return *this;
}
BitArray& BitArray::operator>>=(int n){

    if (n < 0)
        throw std::invalid_argument("Shift count cannot be negative.");

    if (n >= num_bits_)
        return reset();

    for (int i = 0; i < num_bits_; i++) {

        if (i + n < num_bits_ )
            set(i, (*this)[i + n]);
        else
            reset(i);


    }
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


//Изменяет размер массива. В случае расширения, новые элементы
//инициализируются значением value.
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

        int new_num_blocks;
        if (new_num_bits % BITS_PER_BLOCK == 0)
            new_num_blocks = new_num_bits/BITS_PER_BLOCK;
        else
            new_num_blocks = new_num_bits/BITS_PER_BLOCK + 1;


        if (new_num_blocks < num_blocks_) {
            unsigned long* new_arr = new unsigned long[new_num_blocks]{};

            for (int i = 0; i < new_num_blocks; i++)
                new_arr[i] = arr_[i];

            delete[] arr_;
            arr_ = new_arr;
            num_blocks_ = new_num_blocks;

        }
        num_bits_ = new_num_bits;

        int remainder = num_bits_ % BITS_PER_BLOCK;
        if (remainder != 0)
            arr_[num_blocks_ - 1] &= (1UL << remainder) - 1UL;

        return;

    }


    int new_num_blocks;
    if (new_num_bits % BITS_PER_BLOCK == 0)
        new_num_blocks = new_num_bits/BITS_PER_BLOCK;
    else
        new_num_blocks = new_num_bits/BITS_PER_BLOCK + 1;

    if (new_num_blocks > num_blocks_) {

        unsigned long* new_arr = new unsigned long[new_num_blocks]{};

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


//Добавляет новый бит в конец массива. В случае необходимости
//происходит перераспределение памяти.
void BitArray::push_back(bool bit) {
    if (num_bits_ % BITS_PER_BLOCK == 0) {

        unsigned long* new_arr = new unsigned long[num_blocks_ + 1]{};

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




