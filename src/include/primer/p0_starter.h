//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

namespace bustub {

/*
 * The base class defining a Matrix
 */
template <typename T>
class Matrix {
 protected:
  // TODO(P0): Add implementation
  Matrix(int r, int c) {
    rows=r;
    cols=c;
    linear=new T[r*c];
  }

  // # of rows in the matrix
  int rows;
  // # of Columns in the matrix
  int cols;
  // Flattened array containing the elements of the matrix
  // TODO(P0) : Allocate the array in the constructor. Don't forget to free up
  // the array in the destructor.
  T *linear;

 public:
  // Return the # of rows in the matrix
  virtual int GetRows() = 0;

  // Return the # of columns in the matrix
  virtual int GetColumns() = 0;

  // Return the (i,j)th  matrix element
  virtual T GetElem(int i, int j) = 0;

  // Sets the (i,j)th  matrix element to val
  virtual void SetElem(int i, int j, T val) = 0;

  // Sets the matrix elements based on the array arr
  virtual void MatImport(T *arr) = 0;

  // TODO(P0): Add implementation
  virtual ~Matrix(){
    delete[] linear;
  }
};

template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  // TODO(P0): Add implementation
  RowMatrix(int r, int c) : Matrix<T>(r, c) {
    data_= new T*[r];
    for(int i=0;i<r;i++){
      data_[i]=new T[c];
    }

    for(int i=0;i<r;i++){
      for(int j=0;j<c;j++){
        data_[i][j]=0;
      }
    }
  }

  // TODO(P0): Add implementation
  int GetRows() override { 
    return Matrix<T>::rows; 
  }

  // TODO(P0): Add implementation
  int GetColumns() override { 
    return Matrix<T>::cols;
  }

  // TODO(P0): Add implementation
  T GetElem(int i, int j) override { 
    return data_[i][j];
  }

  // TODO(P0): Add implementation
  void SetElem(int i, int j, T val) override {
    data_[i][j]=val;
  }

  // TODO(P0): Add implementation
  void MatImport(T *arr) override {
    int count=0;
    for(int i=0;i<Matrix<T>::rows;i++){
      for(int j=0;j<Matrix<T>::cols;j++){
        data_[i][j]=arr[count];
        count=count+1;
      }
    }
  }

  // TODO(P0): Add implementation
  ~RowMatrix() override{
    for(int i=0;i<Matrix<T>::rows; i++){
        delete[] data_[i];
    }
    delete[] data_;
  }

 private:
  // 2D array containing the elements of the matrix in row-major format
  // TODO(P0): Allocate the array of row pointers in the constructor. Use these pointers
  // to point to corresponding elements of the 'linear' array.
  // Don't forget to free up the array in the destructor.
  T **data_;
};

template <typename T>
class RowMatrixOperations {
 public:
  // Compute (mat1 + mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> AddMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                   std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int rows1=mat1->GetRows();
    int cols1=mat1->GetColumns();
    int rows2=mat2->GetRows();
    int cols2=mat2->GetColumns();
    if(rows1!=rows2 || cols1!=cols2){
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    std::unique_ptr<RowMatrix<T>> result (new RowMatrix<T>(rows1,cols1));
    int sumVal=0;

    for (int i=0;i<rows1;i++){
      for(int j=0;j<cols1;j++){
        T val1=mat1->GetElem(i,j);
        T val2=mat2->GetElem(i,j);
        sumVal=val1+val2;
        result->SetElem(i,j,sumVal);
      }
    }

    return result;
  }

  // Compute matrix multiplication (mat1 * mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> MultiplyMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                        std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int rows1=mat1->GetRows();
    int cols1=mat1->GetColumns();
    int rows2=mat2->GetRows();
    int cols2=mat2->GetColumns();
    if(cols1!=rows2){
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    std::unique_ptr<RowMatrix<T>> result (new RowMatrix<T>(rows1,cols2));
    
    for (int i=0; i<rows1;i++) {
      for (int j=0; j<cols2; j++) {
        int matMultVal=0;
        for (int k=0; k<cols1;k++) {
          matMultVal=matMultVal+mat1->GetElem(i,k)*mat2->GetElem(k,j);
        }
        result->SetElem(i,j,matMultVal);
      }
    }
    return result;
  }

  // Simplified GEMM (general matrix multiply) operation
  // Compute (matA * matB + matC). Return nullptr if dimensions mismatch for input matrices
  static std::unique_ptr<RowMatrix<T>> GemmMatrices(std::unique_ptr<RowMatrix<T>> matA,
                                                    std::unique_ptr<RowMatrix<T>> matB,
                                                    std::unique_ptr<RowMatrix<T>> matC) {

    std::unique_ptr<RowMatrix<T>> Multi_Result = RowMatrixOperations<T>::MultiplyMatrices(std::move(matA), std::move(matB));

    int rows1=Multi_Result->GetRows();
    int cols1=Multi_Result->GetColumns();
    int rows2=matC->GetRows();
    int cols2=matC->GetColumns();

    if(rows1!=rows2 || cols1!=cols2){
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }
    
    std::unique_ptr<RowMatrix<T>> result = RowMatrixOperations<T>::AddMatrices(std::move(Multi_Result), std::move(matC));

    return result;
  }
};
}  // namespace bustub
