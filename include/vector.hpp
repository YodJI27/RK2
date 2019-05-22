
// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_POLYNOMIAL_HPP_
#define INCLUDE_POLYNOMIAL_HPP_
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>

template <typename T>
class Polynomial {
 private:
  size_t m;
  std::vector<T> Data;

 public:
  Polynomial()
  {
      m = 0u;
  }

  explicit Polynomial(const size_t a)
  {
      m = a;
      for (size_t i = 0; i < m; ++i) {
          Data.push_back(0);
      }
  }

  explicit Polynomial(const std::vector<T> vec)
  {
      m = vec.size();
      Data = vec;
  }

  Polynomial(const Polynomial& pol)
  {
      m = pol.m;
      Data = pol.Data;
  }

  size_t Degree() const
  {
      //this->coef_correct();
      return Data.size() - 1;
  }

  Polynomial<T>& operator=(const Polynomial<T>& rhs)
  {
      Data.clear();
      m = 0;
      Data = rhs.Data;
      m = rhs.m;
      return *this;
  }

  Polynomial<T>& operator+=(const Polynomial<T>& rhs)
  {
      if (rhs.m == m) {
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i] += rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      if (rhs.m > m) {
          Data.insert(Data.begin(), rhs.m - m, 0);
          m = rhs.m;
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i] += rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      if (rhs.m < m) {
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i + (m - rhs.m)] += rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      return *this;
  }

  Polynomial<T>& operator-=(const Polynomial<T>& rhs)
  {
      if (rhs.m == m) {
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i] -= rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      if (rhs.m > m) {
          Data.insert(Data.begin(), rhs.m - m, 0);
          m = rhs.m;
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i] -= rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      if (rhs.m < m) {
          for (size_t i = 0; i < rhs.m; ++i) {
              Data[i + (m - rhs.m)] -= rhs.Data[i];
          }
          this->coef_correct();
          return *this;
      }
      return *this;
  }

  Polynomial<T>& operator*=(const Polynomial<T>& rhs) {
      Polynomial<T> mult(rhs.m + m - 1);
      mult.m = m + rhs.m - 1;
      for (size_t i = 0; i < m; ++i) {
          for (size_t j = 0; j < rhs.m; ++j) {
              mult.Data[i + j] += Data[i] * rhs.Data[j];
          }
      }
      Data.clear();
      m = mult.m;
      Data = mult.Data;
      return *this;
  }

  Polynomial<T> operator*(const T a)
  {
      if (a == 0) {
          Data.clear();
          m = 0;
          return *this;
      }
      for (size_t i = 0; i < m; ++i) {
          Data[i] *= a;
      }
      return *this;
  }

  Polynomial<T>& operator/=(const Polynomial<T>& rhs)
  {
      /*if (rhs.m == 0) {
          return Polynomial<T>{};
      }*/
      Polynomial<T> tmp = *this;
      Polynomial<T> segmentation(tmp.m - rhs.m + 1);
      for (size_t i = 0; i < tmp.m - rhs.m + 1; ++i) {
          segmentation[i] = tmp[i] / rhs[0];
          for (size_t j = 0; j < rhs.m; ++j) {
              tmp[i + j] -= rhs[j] * segmentation[i];
          }
      }
      Data.clear();
      m = segmentation.m;
      Data = segmentation.Data;
      return *this;
  }

  Polynomial<T>& operator%=(const Polynomial<T>& rhs)
  {
      Polynomial<T> tmp = *this;
      Polynomial<T> segmentation(tmp.m - rhs.m + 1);
      for (size_t i = 0; i < tmp.m - rhs.m + 1; ++i) {
          segmentation[i] = tmp[i] / rhs[0];
          for (size_t j = 0; j < rhs.m; ++j) {
              tmp[i + j] -= rhs[j] * segmentation[i];
          }
      }
      Data.clear();
      m = tmp.m;
      Data = tmp.Data;
      this->coef_correct();
      return *this;
  }

  T operator[](size_t index) const
  {
      return Data[index];
  }

  T& operator[](size_t index)
  {
      return Data[index];
  }

  T Count(T x) const
  {
      if (x == 0 || Data.size() == 0) {
          return 0;
      }
      T sum = 0;
      size_t dgr = this->Degree();
      for (size_t i = 0; i < m; ++i) {
          sum += pow(x, dgr - i) * Data[i];
      }
      return sum;
  }

  bool operator==(const Polynomial<T>& rhs) const
  {
      if (Data == rhs.Data) {
          return true;
      }
      return false;
  }

  void print()
  {
      for (size_t i = 0, j = m - 1; i < m; ++i, --j) {
          std::cout << Data[i] << "*x^" << j << " + ";
      }
  }

  void coef_correct()
  {
      size_t i = 0;
      while (Data[i] == 0) {
          ++i;
      }
      std::reverse(Data.begin(), Data.end());
      for (size_t j = 0; j < i; ++j) {
          Data.pop_back();
      }
      m = Data.size();
      std::reverse(Data.begin(), Data.end());
  }
};

template <typename T>
Polynomial<T> operator+(const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> addition(a);
  addition += b;
  return addition;
}
template <typename T>
Polynomial<T> operator-(const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> subtraction(a);
  subtraction -= b;
  return subtraction;
}

template <typename T>
Polynomial<T> operator*(const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> multiplication(a);
  multiplication *= b;
  return multiplication;
}

template <typename T>
Polynomial<T> operator/(const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> segmentation(a);
  segmentation /= b;
  return segmentation;
}
template <typename T>
Polynomial<T> operator%(const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> modul = a;
  modul %= b;
  return modul;
}


#endif  // INCLUDE_POLYNOMIAL_HPP_
