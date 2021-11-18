#include <sstream>

#include <gtest/gtest.h>

#include "Core/matrix_base.hh"
#include "config.hh"

using namespace Limno::_detail;

TEST(MatrixBase, Constructors) 
{
    //Create empty matrix
    LimnoMatrixBase<double, 2, 2> m1;
    EXPECT_EQ(m1.numRows(), 2);
    EXPECT_EQ(m1.numCols(), 2);
    EXPECT_EQ(m1.size(), 2*2);

    LimnoMatrixBase<double, DYNAMIC, 2> m2;
    EXPECT_EQ(m2.numRows(), 0);
    EXPECT_EQ(m2.numCols(), 0);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_TRUE(m2.empty());


    LimnoMatrixBase<double, 2, DYNAMIC> m3;
    EXPECT_EQ(m3.numRows(), 0);
    EXPECT_EQ(m3.numCols(), 0);
    EXPECT_EQ(m3.size(), 0);
    EXPECT_TRUE(m3.empty());


    LimnoMatrixBase<double, DYNAMIC, DYNAMIC> m4;
    EXPECT_EQ(m4.numRows(), 0);
    EXPECT_EQ(m4.numCols(), 0);
    EXPECT_EQ(m4.size(), 0);
    EXPECT_TRUE(m4.empty());

    LimnoMatrixBase<double, DYNAMIC, DYNAMIC> n1(2, 3);
    EXPECT_EQ(n1.numRows(), 2);
    EXPECT_EQ(n1.numCols(), 3);
    EXPECT_EQ(n1.size(), 0);

    //Create non-empty matrix 
    //Fill with single value
    LimnoMatrixBase<double, 2, 3> l1(0.0);
    EXPECT_EQ(l1.numRows(), 2);
    EXPECT_EQ(l1.numCols(), 3);
    EXPECT_EQ(l1.size(), 6);
    EXPECT_EQ(l1(0, 0), 0.0);

    LimnoMatrixBase<double, DYNAMIC, DYNAMIC> l2(0.0, 2, 3);
    EXPECT_EQ(l2.numRows(), 2);
    EXPECT_EQ(l2.numCols(), 3);
    EXPECT_EQ(l2.size(), 6);
    EXPECT_EQ(l2(0, 0), 0.0);

    //Create matrix from iterator pairs 
    std::vector<int> test_vals = {1, 2, 3, 4, 5, 6};

    LimnoMatrixBase<int, 2, 3> o1(test_vals.begin(), test_vals.end());
    EXPECT_EQ(o1.numRows(), 2);
    EXPECT_EQ(o1.numCols(), 3);
    EXPECT_EQ(o1.size(), 6);
    EXPECT_EQ(o1(0, 0), 1);

    LimnoMatrixBase<int, DYNAMIC, DYNAMIC> o2(test_vals.begin(), test_vals.end(), 2, 3);
    EXPECT_EQ(o2.numRows(), 2);
    EXPECT_EQ(o2.numCols(), 3);
    EXPECT_EQ(o2.size(), 6);
    EXPECT_EQ(o2(0, 0), 1);

    //Constructor matrix from another container 
    std::vector<std::vector<int>> c = {
        {1, 2, 3}, 
        {1, 2, 3}
    };

    //Test exact size
    LimnoMatrixBase<double, 2, 3> p1(c);
    EXPECT_EQ(p1.size(), 6);
    EXPECT_EQ(p1.numRows(), 2);
    EXPECT_EQ(p1.numCols(), 3);
    EXPECT_EQ(p1(0, 0), 1);

    LimnoMatrixBase<int, DYNAMIC, DYNAMIC> p2(c);
    EXPECT_EQ(p1.size(), 6);
    EXPECT_EQ(p1.numRows(), 2);
    EXPECT_EQ(p1.numCols(), 3);
    EXPECT_EQ(p1(0, 0), 1);

    //Test padding 
    LimnoMatrixBase<double, 2, 4> p3(c);
    EXPECT_EQ(p3.size(), 8);
    EXPECT_EQ(p3.numRows(), 2);
    EXPECT_EQ(p3.numCols(), 4);
    EXPECT_EQ(p3(0, 0), 1);
    EXPECT_EQ(p3(1, 3), 0);

    //Bad idea to let Matrix have fewer elements than container 
    LimnoMatrixBase<double, 2, 1> p4(c);

    //Test from C-style array 
    double arr[] = {1, 2, 3, 4, 5, 6, 7, 8};

    LimnoMatrixBase<double, 2, 4> q1(arr);
    EXPECT_EQ(q1.size(), 8);
    EXPECT_EQ(q1.numRows(), 2);
    EXPECT_EQ(q1.numCols(), 4);
    EXPECT_EQ(q1(0, 0), 1);
    EXPECT_EQ(q1(1, 3), 8);

    LimnoMatrixBase<double, DYNAMIC, DYNAMIC> q2(arr, 2, 4);
    EXPECT_EQ(q1.size(), 8);
    EXPECT_EQ(q1.numRows(), 2);
    EXPECT_EQ(q1.numCols(), 4);
    EXPECT_EQ(q1(0, 0), 1);
    EXPECT_EQ(q1(1, 3), 8);

    //Test padding 
    double arr2[] = {1, 2, 3, 4, 5, 6, 7, 8};
    LimnoMatrixBase<double, 3, 5> q3(arr2);
    EXPECT_EQ(q3.size(), 15);
    EXPECT_EQ(q3.numRows(), 3);
    EXPECT_EQ(q3.numCols(), 5);
    EXPECT_EQ(q3(0, 0), 1);
    EXPECT_EQ(q3(2, 4), 0);
}

TEST(MatrixBase, Indexing)
{
    std::vector<std::vector<double>> v = {
        {1, 2},
        {3, 4}
    };

    LimnoMatrixBase<double, DYNAMIC, DYNAMIC>m1(v);
    EXPECT_EQ(m1(0, 0), 1);
    EXPECT_EQ(m1(0, 1), 2);
    EXPECT_EQ(m1(1, 0), 3);
    EXPECT_EQ(m1(1, 1), 4);

    LimnoMatrixBase<int, 2, 2> m2;
    m2(0, 0) = 1;
    m2(0, 1) = 2;
    m2(1, 0) = 3;
    m2(1, 1) = 4;
    EXPECT_EQ(m2(0, 0), 1);
    EXPECT_EQ(m2(0, 1), 2);
    EXPECT_EQ(m2(1, 0), 3);
    EXPECT_EQ(m2(1, 1), 4);
}

TEST(MatrixBase, OstreamOperator)
{
    LimnoMatrixBase<int, 2, 2> m2;
    m2(0, 0) = 1;
    m2(0, 1) = 2;
    m2(1, 0) = 3;
    m2(1, 1) = 4;
    std::string expected = "{1, 2},\n{3, 4}";
    std::ostringstream actual;
    actual << m2;
    EXPECT_STREQ(expected.c_str(), actual.str().c_str());

    LimnoMatrixBase<double, DYNAMIC, DYNAMIC> m3;
    std::string expected2 = "";
    std::ostringstream actual2;
    actual2 << m3;
    EXPECT_STREQ(expected2.c_str(), actual2.str().c_str());
}