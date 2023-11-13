//===-- utils/sw/circular_buffer.h - CircularBuffer class definition ------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the CircularBuffer which is a
/// software utility class for a stack based FIFO container implementation.
///
/// This class is able to store a preset amount of data of given type in a
/// continuous manner as long as the buffer isn't full.
///
/// The container is NOT threadsafe!
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_SW_CIRCULAR_BUFFER_H
#define UTILS_SW_CIRCULAR_BUFFER_H

#include <initializer_list>

namespace PTS
{

template<typename TYPE, std::size_t SIZE>
class CircularBuffer {

// CIRCULAR BUFFER EXPLAINER
//
//  default pos of m_write_offset
//  |       after writing 4 values
//  |       |
//  v       v
// +-+-+-+-+-+-+-+ cell amount set by BUFFER_SIZE
// |r|r|w|w|*|*|*|
// +-+-+-+-+-+-+-+ offsets wrap around at the end of buffer
//  ^   ^
//  |   |
//  |   after reading 2 values
//  default pos of m_read_offset
//
// * - garbage value
// w - written value
// r - read value

//===-- Member types ------------------------------------------------------===//

 public:
  using value_type = TYPE;
  using size_type = std::size_t;
  using reference = TYPE &;
  using const_reference = const TYPE &;

//===-- Member functions --------------------------------------------------===//

  /// Creates an empty container.
  explicit CircularBuffer()
  : m_write_offset(0), m_read_offset(0), m_buffer{0}
  { }

  /// Creates a container filled with the values given as parameter.
  /// \param init_list initializer_list of the stored type.
  explicit CircularBuffer(std::initializer_list<value_type> init_list)
  : m_write_offset(init_list.size()), m_read_offset(0), m_buffer{0}
  {
    std::copy(init_list.begin(), init_list.end(), m_buffer);
  }

  ~CircularBuffer() = default;

  CircularBuffer& operator=(const CircularBuffer &other)
  {
    if (this != &other)
    {
      m_write_offset = other.m_write_offset;
      m_read_offset = other.m_read_offset;
      for (size_t idx = 0; idx != SIZE; idx++)
        m_buffer[idx] = other.m_buffer[idx];
    }
    return *this;
  }

//===-- Element access ----------------------------------------------------===//

  /// Accesses the first element.
  /// If the container is empty, the returned reference should be discarded.
  /// @return the first element in the container.
  reference front() { return m_buffer[m_read_offset]; }
  const_reference front() const { return m_buffer[m_read_offset]; }

  /// Accesses the last element.
  /// If the container is empty, the returned reference should be discarded.
  /// @return the last element in the container.
  reference back() { return m_buffer[correct_wrap(m_write_offset - 1)]; }
  const_reference back() const { return m_buffer[correct_wrap(m_write_offset - 1)]; }

//===-- Capacity ----------------------------------------------------------===//

  /// Check whether the container is empty.
  /// @return true if the contrainer is empty, false otherwise.
  bool empty() const { return m_write_offset == m_read_offset; }

  /// Returns the number of elements.
  /// \return the number of elements in the container.
  size_type size() const
  {
    return m_write_offset < m_read_offset ?
              m_read_offset - m_write_offset :
              m_write_offset - m_read_offset;
  }

//===-- Modifiers ---------------------------------------------------------===//
  //TODO emplace? swap?

  /// Inserts element at the end.
  /// If the container is full, no new element can be inserted.
  /// \param value the given element to insert.
  void push(const value_type &value)
  {
    if (correct_wrap(m_write_offset + 1) != m_read_offset)
    {
      m_buffer[m_write_offset] = value;
      m_write_offset = correct_wrap(m_write_offset + 1);
    }
  }

  /// Removes the first element.
  void pop()
  {
    if (!empty())
    {
      m_read_offset = correct_wrap(m_read_offset + 1);
    }
  }

//===-- Internals ---------------------------------------------------------===//

 private:
  /// Corrects the given offset adjusting for wrap-around.
  /// \param offset the offset to wrap-correct.
  /// \return the corrected offset value.
  size_type correct_wrap(size_type offset) const noexcept
  {
    return offset % SIZE;
  }

//===-- Members ---------------------------------------------------------===//

 private:
  size_type m_write_offset;
  size_type m_read_offset;
  TYPE m_buffer[SIZE];
}; // class CircularBuffer

} // namespace PTS

#endif // UTILS_SW_CIRCULAR_BUFFER_H