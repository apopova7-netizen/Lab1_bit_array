#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

/**
 * @class BitArray
 * @brief A dynamic array of bits.
 * This class stores bits packed into blocks of unsigned long integers,
 * providing support for bitwise logical operations, shifts, dynamic resizing,
 * and retrieval of information about the array's contents.
 */

class BitArray
{
private:
  unsigned long* arr_;
  int num_bits_;
  int num_blocks_;


public:
   /**
   * @brief Number of bits stored in a single block (an element of an unsigned long array)
   */
  inline static const int BITS_PER_BLOCK = sizeof(unsigned long) * 8;

  /**
   * @brief Creates an empty BitArray with zero size and a null pointer.
   */
  BitArray();
  /**
   * @brief Destroys the BitArray and frees all internal memory blocks.
   */
  ~BitArray();

  /**
  * @brief Creates a BitArray of the specified size and initializes the first block with the provided value.
  * @param num_bits The number of bits in the array.
  * @param value The initial state of the first block (bits 0 to BITS_PER_BLOCK-1).
  * @throws std::length_error If num_bits is negative.
  */
  explicit BitArray(int num_bits, unsigned long value = 0);
  /**
   * @brief Copy constructor. Creates a copy of another BitArray object.
   * @param b Reference to the source BitArray object to be copied.
   */
  BitArray(const BitArray& b);
  /**
   * @brief Swaps the dimensions and internal arrays of this object with another.
   * @param b The target BitArray object for the swap.
   */
  void swap(BitArray& b);
  /**
    * @brief Resizes the container to hold the specified number of bits.
    *
    * If the current size is less than new_num_bits, additional bits are added,
    * initialized to the specified boolean value. If the current size is greater,
    * the array is truncated.
    *
    * @param new_num_bits The target number of bits in the array.
    * @param value The default value used to initialize added bits (defaults to false).
    * @throws std::length_error If new_num_bits is negative.
    */
  void resize(int new_num_bits, bool value = false);
  /**
  * @brief Clears the array contents and resets the size to zero
  */
  void clear();
  /**
 * @brief Appends a new bit with the specified value to the end of the array, increasing its size by one.
 * @param bit The boolean value of the bit to append.
 */
  void push_back(bool bit);
  /**
  * @brief Clears all unused padding bits in the upper memory block.
  */
  void remove_extra_bits();
  /**
   * @brief Copy assignment operator. Replaces the current contents with a copy of another object.
   * @param b The source BitArray object to copy from.
   * @return A reference to this modified instance (*this).
  */
  BitArray& operator=(const BitArray& b);
  /**
   * @brief Computes the bitwise AND operation on the current object using another BitArray object of the same size.
   * @param b The BitArray operand (right-hand argument).
   * @return A reference to this modified object (*this).
   * @throws std::length_error If the sizes of the two bit arrays do not match.
   */
  BitArray& operator&=(const BitArray& b);
  /**
   * @brief Computes the bitwise OR operation on the current object using another BitArray object of the same size.
   * @param b The BitArray operand (right-hand argument).
   * @return A reference to this modified object (*this).
   * @throws std::length_error If the sizes of the two bit arrays do not match.
   */
  BitArray& operator|=(const BitArray& b);
  /**
  * @brief Computes the bitwise XOR operation on the current object using another BitArray object of the same size.
  * @param b The BitArray operand (right-hand argument).
  * @return A reference to this modified object (*this).
  * @throws std::length_error If the sizes of the two bit arrays do not match.
  */
  BitArray& operator^=(const BitArray& b);
  /**
  * @brief Shifts all bits in the array to the left by n positions in-place, filling the vacated positions with zeros.
  * @param n The number of positions to shift.
  * @return A reference to this modified instance (*this).
  * @throws std::invalid_argument If the shift amount is negative.
  */
  BitArray& operator<<=(int n);
  /**
  * @brief Shifts all bits in the array to the right by n positions in-place, filling the vacated positions with zeros.
  * @param n The number of positions to shift.
  * @return A reference to this modified instance (*this).
  * @throws std::invalid_argument If the shift amount is negative.
  */
  BitArray& operator>>=(int n);
  /**
  * @brief Returns a new BitArray containing the result of a left shift by n positions.
  * @param n The number of positions to shift.
  * @return A new BitArray instance containing the shifted bits.
  */
  BitArray operator<<(int n) const;
  /**
  * @brief Returns a new BitArray containing the result of a right shift by n positions.
  * @param n The number of positions to shift.
  * @return A new BitArray instance containing the shifted bits.
  */
  BitArray operator>>(int n) const;
  /**
   * @brief Sets the bit at the specified index to the given boolean value.
   * @param n The index of the target bit.
   * @param val The boolean value to write to the bit position (default is true).
   * @return A reference to this modified object (*this).
   */
  BitArray& set(int n, bool val = true);
  /**
  * @brief Sets all bits within the array to 1.
  * @return A reference to this modified object (*this).
  */
  BitArray& set();
  /**
 * @brief Resets (sets to 0) the bit at the specified index.
 * @param n The index of the bit to clear.
 * @return A reference to this modified object (*this).
 */
  BitArray& reset(int n);
    /**
 * @brief Resets all bits within the array to 0.
 * @return A reference to this modified object (*this).
 */
  BitArray& reset();
  /**
  * @brief Checks whether at least one bit in the array is set to 1.
  * @return True if any bit is 1, false if all bits are 0 or the array is empty.
  */
  bool any() const;
  /**
  * @brief Checks whether all bits in the array are set to 0.
  * @return True if no bits are 1, false otherwise.
  */
  bool none() const;
  /**
 * @brief Checks if the container has no elements (its size is 0).
 * @return True if the array is empty, false otherwise.
 */
  bool empty() const;
  /**
  * @brief Returns a new BitArray with all bits inverted (bitwise NOT operation).
  * @return A new inverted BitArray instance.
  */
  BitArray operator~() const;
  /**
  * @brief Index access operator providing read-only access to the value of a specific bit by its position.
  * @param i Index of the requested bit.
  * @return Boolean value of the bit at index i.
  */
  bool operator[](int i) const;
  /**
  * @brief Calculates the total number of bits set to 1.
  * @return The total number of bits set to 1.
  */
  int count() const;
  /**
  * @brief Returns the total number of bits stored in the array.
  * @return The logical size of the container (number of bits).
  */
  int size() const;
 /**
 * @brief Converts a BitArray to a binary string representation.
 *
 * The resulting string is ordered from the most significant bit
 * to the least significant bit (left to right).
 *
 * @return A string of 0s and 1s representing the contents of the bit array.
 */
  std::string to_string() const;

};
/**
 * @brief Checks if two bit arrays have identical sizes and identical bit sequences.
 */
bool operator==(const BitArray & a, const BitArray & b);
/**
 * @brief Checks if two bit arrays differ in size or contain different bit sequences.
 */
bool operator!=(const BitArray & a, const BitArray & b);

/**
 * @brief Computes the bitwise AND of two bit arrays and returns the result as a new object.
 * @param b1 The left-hand side BitArray operand.
 * @param b2 The right-hand side BitArray operand.
 * @return A new BitArray containing the logical intersection (AND) of the two arrays.
 * @throws std::length_error If the sizes of b1 and b2 do not match.
 */
BitArray operator&(const BitArray& b1, const BitArray& b2);
/**
 * @brief Computes the bitwise OR of two bit arrays and returns the result as a new object.
 * @param b1 The left-hand side BitArray operand.
 * @param b2 The right-hand side BitArray operand.
 * @return A new BitArray containing the logical union (OR) of the two arrays.
 * @throws std::length_error If the sizes of b1 and b2 do not match.
 */
BitArray operator|(const BitArray& b1, const BitArray& b2);
/**
 * @brief Computes the bitwise XOR of two bit arrays and returns the result as a new object.
 * @param b1 The left-hand side BitArray operand.
 * @param b2 The right-hand side BitArray operand.
 * @return A new BitArray containing the exclusive OR (XOR) result of the two arrays.
 * @throws std::length_error If the sizes of b1 and b2 do not match.
 */
BitArray operator^(const BitArray& b1, const BitArray& b2);
/**
 * @brief Helper function to determine how many memory blocks are required to store a given number of bits.
 * @param num_bits The number of bits to store.
 * @return The required count of unsigned long blocks.
 */
int count_blocks(int num_bits);