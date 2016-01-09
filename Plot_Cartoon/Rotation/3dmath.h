// this file uses JavaDoc style comment blocks for automatic extraction of source code documentation.

/*!\file
 \brief Contains some useful math helpers for common 3D graphics entities.

 In this file are implemented some mathematical entities often used in 3D graphics (vecors, points, etc.) plus some useful constants, macros and definitions
 \version 0.1
 \date 12/06/99
 \author Alessandro Falappa
*/

#ifndef _3D_MATH_H
#define _3D_MATH_H

#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;
#ifdef HIGH_PRECISION

//! The base type for all the math helpers
typedef double myReal;
//! the treshold for comparisons with zero, mainly used to avoid division by zero errors
const myReal epsilon=1e-12;
//! defined when high precision is requested
#define REAL_IS_DOUBLE

#else

// WARNING: these pragmas below could be MSVC compiler specific
#pragma warning( push )// memorize the warning status
#pragma warning( disable : 4305 )// disable "initializing : truncation from 'const double' to 'float'" warning
#pragma warning( disable : 4244 )// disable "double to float conversion possible loss of data" warning
#pragma warning( disable : 4136 )// disable "conversion between different floating-point types" warning
#pragma warning( disable : 4309 )// disable " 'conversion' : truncation of constant value" warning
#pragma warning( disable : 4051 )// disable " 'type conversion' ; possible loss of data" warning
//! The base type for all the math helpers
typedef float myReal;
//! the treshold for comparisons with zero, mainly used to avoid division by zero errors
const myReal epsilon=1e-7;
//! defined when high precision is not requested
#define REAL_IS_FLOAT

#endif

//=============================================================================
//=============================================================================

//!A myVector class.
/*!
The myVector class incapsulates a classic C++ myVector of three myReal values and treats them as a 3 dimensional mathematical vectors.

The most common operations between mathematical vectors (and some which involves scalars too) are defined.
*/
class myVector
{
private:
	static int counter;//!< counts how many myVector objects are present
	myReal vec[3];//!< the actual myVector

public:
// operators
	myVector();//!< default constructor
	myVector(const myReal& x,const myReal& y,const myReal& z);//!< constructs a myVector from three values
	myVector(myVector& from, myVector& to);//!< constructs a myVector from two other vectors
	myVector(const myVector& other);//!< the copy constructor
	~myVector();//!< the distructor
	myReal& x();//!< accessor for the x component (can be used as l-value too)
	myReal& y();//!< accessor for the y component (can be used as l-value too)
	myReal& z();//!< accessor for the z component (can be used as l-value too)
	myReal x() const;//!< returns the x component (r-value only)
	myReal y() const;//!< returns the y component (r-value only)
	myReal z() const;//!< returns the z component (r-value only)
	myVector& operator=(const myVector& other);//!< the assignment
	myVector& operator+=(const myVector& other);//!< the sum & assign
	myVector& operator-=(const myVector& other);//!< the subtract & assign
	myVector& operator*=(const myReal& fact);//!< the short multiply by a scalar factor & assign
	myVector& operator/=(const myReal& fact);//!< the short divide by a scalar factor & assign
	myReal& operator[](const int& index);//!< an "access like a C++ myVector"
#ifdef HIGH_PRECISION
	operator double*();//!< the conversion to a 3 double elements C++ myVector
#else
	operator float*();//!< the conversion to a 3 float elements C++ myVector
#endif
//	operator char*();//!< the conversion of the myVector into a textual form (null terminated string)
	myReal normalize();//!< normalize the myVector
	myVector normalized() const;//!< normalized copy of the myVector
	myReal length() const;//!< get the length of the myVector
	myReal length2() const;//!< get the squared length of the myVector
	void EpsilonCorrect(const myVector& v);//!< if the myVector is almost equal to the origin substitute it with v
// ststic functions
	static int howMany();//!< returns how many myVector objects exists

// friend functions	
	friend int operator==(const myVector& v1,const myVector& v2);//!< the equality operator
	friend int operator!=(const myVector& v1,const myVector& v2);//!< the inequality operator
	friend myVector operator+(const myVector& v1,const myVector& v2);//!< the sum
	friend myVector operator-(const myVector& v1,const myVector& v2);//!< the difference
	friend myVector operator-(const myVector& v1);//!< the negation
	friend myReal operator*(const myVector& v1,const myVector& v2);//!< the myVector dot product
	friend myVector operator^(const myVector& v1,const myVector& v2);//!< the myVector cross product
	friend myVector operator*(const myVector& v,const myReal& fact);//!< the multiply a myVector by a scalar factor
	friend myVector operator*(const myReal& fact,const myVector& v);//!< the multiply a scalar factor by a myVector
	friend myVector operator/(const myVector& v,const myReal& fact);//!< the divide a myVector by a scalar factor
	friend myVector Bisect(myVector v0,myVector v1);//!< returns the unit myVector which halves the arc between v0 and v1
	friend void clamp(myVector& vec,const myReal& low,const myReal& high);//!< clamps all the myVector components between the given tresholds
	friend ostream& operator<<(ostream& os,const myVector& vect);//!< the print myVector to C++ stream
#ifdef _AFXDLL // see if we are using MFC...
#ifdef _DEBUG		//.. and if we are in a debug build
	friend CDumpContext& operator<<(CDumpContext& cd,const myVector& vect);//!< the print myVector to MSVC++ debug console
#endif
#endif
};
//-----------------------------------------------------------------------------
// inlines

inline myVector::myVector()
{
	vec[0]=vec[1]=vec[2]=0.0;
	counter++;
}

inline myVector::myVector(const myReal& x,const myReal& y,const myReal& z)
{
	vec[0]=x;
	vec[1]=y;
	vec[2]=z;
	counter++;
}

/*!
This function constructs the myVector which goes from the myVector \e from to the myVector \e to.
It is useful when a myVector has to be constructed from the difference of two other vectors.
\param from the first myVector
\param to the second myVector
*/
inline myVector::myVector(myVector& from, myVector& to)
{
	vec[0]=to.vec[0]-from.vec[0];
	vec[1]=to.vec[1]-from.vec[1];
	vec[2]=to.vec[2]-from.vec[2];
	counter++;
}

inline myVector::myVector(const myVector& other)
{
	*this=other;
	counter++;
}

inline myVector::~myVector()
{
	counter--;
}

inline myReal& myVector::x()
{
	return vec[0];
}

inline myReal& myVector::y()
{
	return vec[1];
}

inline myReal& myVector::z()
{
	return vec[2];
}

inline myReal myVector::x() const
{
	return vec[0];
}

inline myReal myVector::y() const
{
	return vec[1];
}

inline myReal myVector::z() const
{
	return vec[2];
}

inline myVector& myVector::operator=(const myVector& other)
{
	//check for 'a=a' case
	if (this==&other) return *this;
	vec[0]=other.vec[0];
	vec[1]=other.vec[1];
	vec[2]=other.vec[2];
	return *this;
}

inline myVector& myVector::operator+=(const myVector& other)
{
	vec[0]+=other.vec[0];
	vec[1]+=other.vec[1];
	vec[2]+=other.vec[2];
	return *this;
}

inline myVector& myVector::operator-=(const myVector& other)
{
	vec[0]-=other.vec[0];
	vec[1]-=other.vec[1];
	vec[2]-=other.vec[2];
	return *this;
}

inline myVector& myVector::operator*=(const myReal& fact)
{
	vec[0]*=fact;
	vec[1]*=fact;
	vec[2]*=fact;
	return *this;
}

inline myVector& myVector::operator/=(const myReal& fact)
{
	assert(fabs(fact) >= epsilon);
	vec[0]/=fact;
	vec[1]/=fact;
	vec[2]/=fact;
	return *this;
}

/*!
This operator redirects the access to the internal myVector. It does make a range check on the index in debug builds trough the ANSI assert function.
It can be used on both sides of an assignment.
\return a reference to the requested element
\param index the index which should be one of 0,1,2
*/
inline myReal& myVector::operator[](const int& index)
{
	assert(index>=0 && index<=2);
	return vec[index];
}

inline myReal myVector::length2() const
{
	return (*this)*(*this);
}

inline myReal myVector::length() const
{
	return sqrt(this->length2());
}

/*!
The counting of the myVector objects is realized trough a static counter variable.
\return the number of myVector objects in memory
*/
inline int myVector::howMany()
{
	return counter;
}

/*!
this conversion operator allows to use a myVector in places of a classic myReal[3]
*/
/*
inline myVector::operator myReal*()
{
	return (myReal*)vec;
}
*/
#ifdef HIGH_PRECISION

/*!
this conversion operator allows to use a myVector in places of a classic double[3]
*/
inline myVector::operator double*()
{
	return (double*)vec;
}

#else

/*!
this conversion operator allows to use a myVector in place of a classic float[3]
*/
inline myVector::operator float*()
{
	return (float*)vec;
}

#endif

//-----------------------------------------------------------------------------
// useful constants (declaration)

extern const myVector ORIGIN;
extern const myVector X_AXIS;
extern const myVector Y_AXIS;
extern const myVector Z_AXIS;

//=============================================================================
//=============================================================================

/*!
A transformation matrix class.

The matrix class groups sixteen myReal values an treats them as a 4x4 matrix. Standard
C++ matrices (2 dimensional vectors) are stored by row, since for graphical applications
a column major order is preferable the access indices are internally swapped.

The most common operators between matrices (and some which involves scalars and vectors too) are defined.
*/
class tmatrix
{
private:
	static int counter;//!< counts how many matrices objects are present
	myReal mat[4][4];//!< the matrix data
public:
	//! Espresses how to store a tmatrix in a single 16 elements vector, by column or by row
	enum ordermode
	{
		COLUMN,//!< column major order
		ROW//!< row major order
	};
	tmatrix();//!< default constructor
	tmatrix(const myReal& val);//!< constructs a tmatrix and fills it with a value
	tmatrix(const myReal vector[16],ordermode ord=COLUMN);//!< constructs a tmatrix from a myVector which contains a 4x4 matrix in row major or column major order
	tmatrix(const tmatrix& other);//!< the copy constructor
	~tmatrix();//!< the distructor
	tmatrix& operator-();//!< negation
	tmatrix& operator=(const tmatrix& other);//!< assignment
	tmatrix& operator+=(const tmatrix& other);//!< sum & assign
	tmatrix& operator-=(const tmatrix& other);//!< subtract & assign
	tmatrix& operator*=(const tmatrix& other);//!< multiply by a tmatrix & assign
	tmatrix& operator*=(const myReal& fact);//!< multiply by a scalar factor & assign
	tmatrix& operator/=(const myReal& fact);//!< divide by a scalar factor & assign
	myReal& operator()(const int& row,const int& col);//!< an "access like a C++ tmatrix"
#ifdef HIGH_PRECISION
	operator double*();//!< conversion to a 16 doubles C++ myVector (column major order)
#else
	operator float*();//!< conversion to a 16 floats C++ myVector (column major order)
#endif
	void loadIdentity();//!< fills the matrix with the identity matrix
//	operator char*();//!< conversion of the tmatrix into a textual form (null terminated string)
//static functions
	static int howMany();//!< returns how many tmatrix objects exists
// friend functions	
	friend int operator==(const tmatrix& t1,const tmatrix& t2);//!< the equality operator
	friend int operator!=(const tmatrix& t1,const tmatrix& t2);//!< the inequality operator
	friend tmatrix operator+(const tmatrix& t1,const tmatrix& t2);//!< the sum
	friend tmatrix operator-(const tmatrix& t1,const tmatrix& t2);//!< the difference
	friend tmatrix operator*(const tmatrix& t1,const tmatrix& t2);//!< tmatrix product
	friend tmatrix operator*(const tmatrix& tmat,const myReal& fact);//!< multiply a tmatrix by a scalar factor
	friend tmatrix operator*(const myReal& fact,const tmatrix& tmat);//!< multiply a scalar factor by a tmatrix
	friend tmatrix operator/(const tmatrix& tmat,const myReal& fact);//!< divide a tmatrix by a scalar factor
	friend ostream& operator<<(ostream& os,const tmatrix& m);//!< print tmatrix to C++ stream
#ifdef _AFXDLL // see if we are using MFC
#ifdef _DEBUG
	friend CDumpContext& operator<<(CDumpContext& cd,const tmatrix& m);//!< print tmatrix to MSVC++ debug console
#endif
#endif
};

//-----------------------------------------------------------------------------
// inlines

inline tmatrix::tmatrix(const tmatrix& other)
{
	*this=other;
	counter++;
}

inline tmatrix::~tmatrix()
{
	counter--;
}

/*!
The counting of the tmatrix objects is realized trough a static counter variable.
\return the number of tmatrix objects in memory
*/
inline int tmatrix::howMany()
{
	return counter;
}

/*!
This operator redirects the access to the internal matrix. It does make a range
check on the index in debug builds trough the ANSI assert function.
It can be used on both sides of an assignment.
\return a reference to the requested element
\param row the row index which should be one of 0,1,2,3
\param col the column index which should be one of 0,1,2,3
\date 18/06/99
*/
inline myReal& tmatrix::operator()(const int& row,const int& col)
{
	assert(row>=0 && row<=3);
	assert(col>=0 && col<=3);
	return mat[col][row];// swap indices to store by column
}

#ifdef HIGH_PRECISION

/*!
this conversion operator allows to use a tmatrix in places where a column major
order myVector of 16 double elements is requested (e.g. the OpenGL functions for
retrieving/setting the modelview or projection matrix).
*/
inline tmatrix::operator double*()
{
	return (double*)mat;
}

#else

/*!
this conversion operator allows to use a tmatrix in places where a column major
order myVector of 16 float elements is requested (e.g. the OpenGL functions for
retrieving/setting the modelview or projection matrix).
*/
inline tmatrix::operator float*()
{
	return (float*)mat;
}

#endif

//-----------------------------------------------------------------------------
// useful constants


//=============================================================================
//=============================================================================

/*!
A generic quaternion class.
The quaternion uses internally the (s,<B>v</B>) representation, where s is a scalar and <B>v</B> a myVector.
The most common operations from quaternion calculus are implemented.

Some internet references are:
<UL>
<LI>http://www.cs.berkley.edu/~laura/cs184/quat/quaternion.html
<LI>http://www.astro.virginia.edu/~eww6n/math/Quaternion.html
<LI>http://forum.swarthmore.edu/dr.math/problems/prasad2.17.96.html
<LI>http://graphics.cs.ucdavis.edu/GraphicsNotes/Quaternions/Quaternions.html
</UL>

A paper by Ken Shoemake (the quaternions "inventor") is available at<BR>
<UL>
<LI>ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/
</UL>
*/
class quaternion
{
private:
	static int counter;//!< counts how many quaternion objects are present
protected:
	myReal s;//!< the scalar part of a quaternion
	myVector v;//!< the myVector part of a quaternion
public:
	quaternion();//!< default constructor
	quaternion(const myReal& scal, const myVector& vec);//!< constructs a quaternion from the scalar and myVector components
	quaternion(const myReal& s1,const myReal& s2,const myReal& s3,const myReal& s4);//!< constructs a quaternion from four myReal values
	quaternion(const quaternion& other);//!< the copy constructor
	virtual ~quaternion();//!< the distructor
	myReal& x();//!< accessor for the x component of the myVector part (can be used as l-value too)
	myReal& y();//!< accessor for the y component of the myVector part (can be used as l-value too)
	myReal& z();//!< accessor for the z component of the myVector part (can be used as l-value too)
	myReal& w();//!< accessor for the scalar part (can be used as l-value too)
	myReal& scalarPart();//!< other accessor for the scalar component (can be used as l-value too)
	myVector& vectorPart();//!< accessor for the myVector part (can be used as l-value too)
	quaternion& operator=(const quaternion& other);//!< assignment
	quaternion& operator+=(const quaternion& other);//!< sum & assign
	quaternion& operator-=(const quaternion& other);//!< subtract & assign
	quaternion& operator*=(const quaternion& other);//!< multiply by a quaternion & assign
	quaternion& operator/=(const quaternion& other);//!< divide by a quaternion & assign
	quaternion& operator*=(const myReal& fact);//!< multiply by a scalar factor & assign
	quaternion& operator/=(const myReal& fact);//!< divide by a scalar factor & assign
//	operator float*();//!< conversion to a 4 elements C++ myVector
//	operator char*();//!< conversion of the quaternion into a textual form (null terminated string)
	myReal normalize();//!< normalize the quaternion
	quaternion normalized() const;//!< normalized copy of the quaternion
	void conjugate();//!< conjugate of the quaternion
	quaternion conjugated() const;//!< conjugated copy of the quaternion
	myReal inverse();//!< inverse of the quaternion
	quaternion inversed() const;//!< inversed copy of the quaternion
	myReal length() const;//!< get the length of the quaternion
	myReal norm() const;//!< get the norm (similar to the squared length) of the quaternion
	virtual tmatrix getRotMatrix();//<! constructs a rotation matrix from the quaternion
// static functions
	static int howMany();//!< returns how many quaternion objects exists
// friend functions	
	friend int operator==(const quaternion& q1,const quaternion& q2);//!< the equality operator
	friend int operator!=(const quaternion& q1,const quaternion& q2);//!< the inequality operator
	friend quaternion operator+(const quaternion& q1,const quaternion& q2);//!< the sum
	friend quaternion operator-(const quaternion& q1,const quaternion& q2);//!< the difference
	friend quaternion operator-(const quaternion& q1);//!< negation
	friend quaternion operator*(const quaternion& q1,const quaternion& q2);//!< quaternion product
	friend quaternion operator*(const quaternion& q,const myReal& s);//!< multiply a quaternion by a scalar
	friend quaternion operator*(const myReal& s,const quaternion& q);//!< multiply a scalar by a quaternion
	friend quaternion operator/(const quaternion& q,const myReal& s);//!< divide a quaternion by a scalar factor
	friend quaternion operator/(const quaternion& q1,const quaternion& q2);//!< divide a quaternion by a quaternion
	friend ostream& operator<<(ostream& os,const quaternion& q);//!< print quaternion to C++ stream
#ifdef _AFXDLL // see if we are using MFC
#ifdef _DEBUG
	friend CDumpContext& operator<<(CDumpContext& ad,const quaternion& q);//!< print quaternion to MSVC++ debug console
#endif
#endif
};

//-----------------------------------------------------------------------------
// inlines

inline quaternion::quaternion()
{
	s=0.0;
	counter++;
};

/*!
This constructor assumes an (s,\b v) form so interprets the myReal calues as w,x,y,z respectively.
Another common representation for quaternions, the "homogeneous" one, is x,y,x,w ordered.
\param s1 the scalar part
\param s2 the x component of the myVector part
\param s3 the y component of the myVector part
\param s4 the z component of the myVector part
\date 15/06/99
*/
inline quaternion::quaternion (const myReal& s1,const myReal& s2,const myReal& s3,const myReal& s4)
:s(s1),v(s2,s3,s4)
{
	counter++;
}

inline quaternion::quaternion (const myReal& scal, const myVector& vec)
:s(scal),v(vec)
{
	s=scal;
	counter++;
}

inline quaternion::~quaternion()
{
	counter--;
}

inline quaternion::quaternion(const quaternion& other)
{
	*this=other;
	counter++;
}

/*!
The counting of the quaternion objects is realized trough a static counter variable.
\return the number of quaternion objects in memory
*/
inline int quaternion::howMany()
{
	return counter;
}

inline myReal& quaternion::x()
{
	return v.x();
}

inline myReal& quaternion::y()
{
	return v.y();
}

inline myReal& quaternion::z()
{
	return v.z();
}

inline myReal& quaternion::w()
{
	return s;
}

inline myReal& quaternion::scalarPart()
{
	return s;
}

inline myVector& quaternion::vectorPart()
{
	return v;
}

inline quaternion& quaternion::operator=(const quaternion& other)
{
	//check for 'a=a' case
	if (this==&other) return *this;
	s=other.s;
	v=other.v;
	return *this;
}

inline quaternion& quaternion::operator+=(const quaternion& other)
{
	s+=other.s;
	v+=other.v;
	return *this;
}

inline quaternion& quaternion::operator-=(const quaternion& other)
{
	s-=other.s;
	v-=other.v;
	return *this;
}

inline quaternion& quaternion::operator*=(const myReal& fact)
{
	s*=fact;
	v*=fact;
	return *this;
}

inline quaternion& quaternion::operator/=(const myReal& fact)
{
	assert(fabs(fact) >= epsilon);
	s/=fact;
	v/=fact;
	return *this;
}

inline myReal quaternion::length() const
{
	return sqrt(norm());
}

inline void quaternion::conjugate()
{
	v=-v;
}

inline quaternion quaternion::conjugated() const
{
	return quaternion(s,-v);
}

/*!
This function check if all elements of \e v1 are equal to the corresponding elements of \e v2.
*/
inline int operator==(const myVector& v1,const myVector& v2)
{
	if(v1.vec[0]==v2.vec[0] && v1.vec[1]==v2.vec[1] && v1.vec[2]==v2.vec[2]) return 1;
	else return 0;
}

/*!
This function check if some of the elements of \e v1 differs from the corresponding elements of \e v2.
*/
inline int operator!=(const myVector& v1,const myVector& v2)
{
	if(v1.vec[0]==v2.vec[0] && v1.vec[1]==v2.vec[1] && v1.vec[2]==v2.vec[2]) return 0;
	else return 1;
}


//=============================================================================
//=============================================================================

/*!
A unit lenght quaternion class.

The unitquaternion class manages quaternions of unit length, such quaternions
can be used to represent arbitrary rotations. To mantain the unit lenght property
along quaternion calculus addition and subtraction as well as multiplication and
division by scalars are not allowed (they're made private or they're overloaded
by "trap" functions).

Actually only a few member functions of the base class are overloaded since
the quaternion calculus apply with no modification to unit quaternions.
*/
class unitquaternion : public quaternion
{
private:
	unitquaternion& operator+=(const unitquaternion& other);//!< sum & assign is not allowed
	unitquaternion& operator-=(const unitquaternion& other);//!< subtract & assign is not allowed
	unitquaternion& operator*=(const myReal& fact);//!< multiply by a scalar factor & assign is not allowed
	unitquaternion& operator/=(const myReal& fact);//!< divide by a scalar factor & assign is not allowed
	unitquaternion(const myVector& v, const myReal& s);//!< constructs a unitquaternion assigning directly the scalar and myVector parts
public:
	myReal x() const;//!< accessor for the x component of the myVector part
	myReal y() const;//!< accessor for the y component of the myVector part
	myReal z() const;//!< accessor for the z component of the myVector part
	myReal w() const;//!< accessor for the scalar part
	myReal scalarPart() const;//!< other accessor for the scalar component
	myVector vectorPart() const;//!< accessor for the myVector part
	unitquaternion();//!< default constructor
	unitquaternion(const myReal& angle, const myVector& axis);//!< constructs a unitquaternion representing a rotation of angle radiants about axis 
	unitquaternion(const quaternion& q);//!< constructs a unitquaternion from a generic one (conversion by normalizing)
	~unitquaternion();//!< the distructor
	unitquaternion inversed() const;//!< inversed copy of the unitquaternion
	void inverse();//!< inverse of the unitquaternion
	tmatrix getRotMatrix();//<! constructs a rotation matrix from the quaternion
	void getVectorsOnSphere(myVector& vfrom,myVector& vto);//<! converts a unitquaternion to two vectors on a unit sphere (the extremes of a rotation)
	unitquaternion& operator*=(const unitquaternion& other);//!< multiply by another unitquaternion & assign is not allowed
// friend functions	
	friend unitquaternion operator+(const unitquaternion& q1,const unitquaternion& q2);//!< the sum is not allowed
	friend unitquaternion operator-(const unitquaternion& q1,const unitquaternion& q2);//!< the difference is not allowed
	friend unitquaternion operator*(const unitquaternion& q,const myReal& s);//!< multiply a unitquaternion by a scalar is not allowed
	friend unitquaternion operator*(const myReal& s,const unitquaternion& q);//!< multiply a scalar by a unitquaternion is not allowed
	friend unitquaternion operator/(const unitquaternion& q,const myReal& s);//!< divide a unitquaternion by a scalar factor is not allowed
};

//-----------------------------------------------------------------------------
// inlines

inline unitquaternion::unitquaternion()
:quaternion(1,0,0,0)
{
};


inline unitquaternion::unitquaternion (const quaternion& q)
:quaternion(q)
{
	normalize();
}


/*!
This constructor has a different meaning from the same in the base class. Here
the myVector means an axis of rotation while the myReal means the angle to rotate
about the axis.

\b NOTE: the angle and axis myVector are not directly assigned to the myReal part
and the myVector part, respectively, of the quaternion. The unit quaternion (<I>s</I>,<B>v</B>)
represents a rotation of \b angle radians about the axis \b a if:

\e s = cos(\e angle / 2)<BR>
\b v = \b a * sin( \e angle / 2)
\param angle the rotation angle
\param axis the axis of rotation
\date 18/06/99
*/
inline unitquaternion::unitquaternion (const myReal& angle, const myVector& axis)
:quaternion( cos(angle/2), sin(angle/2)*axis.normalized() )
{
}

/*!
This constructor has been introduced exclusively to make the inversed function
more efficient by avoiding too many unitquaternion copies and conversions to
quaternions.
*/
inline unitquaternion::unitquaternion (const myVector& v, const myReal& s)
:quaternion( s, v )
{
}

inline unitquaternion::~unitquaternion()
{
}

/*!
For unitquaternions the inverse equals the conjugate (which is simpler to calculate)
This function doesn't modifies the quaternion upon which has been called, it returns a new quaternion instead.
\return the normalized copy of the quaternion
\date 18/06/99
*/
inline unitquaternion unitquaternion::inversed() const
{
	return unitquaternion(-v,s);
}

/*!
For unitquaternions the inverse equals the conjugate (which is simpler
to calculate). The function doesn't call the base conjugate function to
avoid an expensive quaternion to unitquaternion conversion.
This function modifies the quaternion upon which has been called.
*/
inline void unitquaternion::inverse()
{
	v=-v;
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the x component of the myVector part).
*/
inline myReal unitquaternion::x() const
{
	return ((myVector&)v).x();// explicit cast to use the non const myVector x() function
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the y component of the myVector part).
*/
inline myReal unitquaternion::y() const
{
	return ((myVector&)v).y();// explicit cast to use the non const myVector y() function
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the z component of the myVector part).
*/
inline myReal unitquaternion::z() const
{
	return ((myVector&)v).z();// explicit cast to use the non const myVector z() function
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the scalar part).
*/
inline myReal unitquaternion::w() const
{
	return s;
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the scalar part).
*/
inline myReal unitquaternion::scalarPart() const
{
	return s;
}

/*!
This function overrides the same in base class to prevent the use as an l-value
(that is to modify the myVector part).
*/
inline myVector unitquaternion::vectorPart() const
{
	return v;
}


//-----------------------------------------------------------------------------
// useful constants

//=============================================================================
//=============================================================================

/* inserire qui alcune macro o funzioni globali come ad esempio:
	- clamping
	- conversione angoli gradi<->radianti
*/

/*!
limits a value in a range, modifying it.
\param val the value to clamp
\param low the lower treshold
\param high the higher treshold
*/
inline void clamp(myReal& val,const myReal& low,const myReal& high)
{
	if(val<low) val=low;
	if(val>high) val=high;
}

/*!
limits a value in a range, returning the clamped value.
\return the clamped value
\param val the value to clamp
\param low the lower treshold
\param high the higher treshold
*/
inline myReal clamped(const myReal& val,const myReal& low,const myReal& high)
{
	if(val<low) return low;
	else if(val>high) return high;
		else return val;
}


/*!
limits the myVector components in a range, modifying it.
\param theVec the myVector to clamp
\param low the lower treshold
\param high the higher treshold
*/
inline void clamp(myVector& theVec,const myReal& low,const myReal& high)
{
	clamp(theVec.vec[0],low,high);
	clamp(theVec.vec[1],low,high);
	clamp(theVec.vec[2],low,high);
}

/*!
\short returns \e angle expressed in degrees.
\return the angle expresses in radians
\param angle the angle value
\author Alessandro Falappa
*/
inline myReal RadToDeg(const myReal& angle)
{
	return angle*57.29577951308;
}

/*!
\short returns \e angle expressed in radians.
\return the angle expresses in degrees
\param angle the angle value
*/
inline myReal DegToRad(const myReal& angle)
{
	return angle*0.01745329251994;
}

/*!
\short converts radiants to degrees.
This function modify its argument.
\param angle the angle to be converted
*/
inline void ConvertToDeg(myReal& angle)
{
	angle*=57.29577951308;
}

/*!
\short converts degrees to radiants.
This function modify its argument.
\param angle the angle to be converted
*/
inline void ConvertToRad(myReal& angle)
{
	angle*=0.01745329251994;
}

/*!
\short absolute value function which executes a simple test
This function executes a simple test on \e val negativity returning the
opposite if true. Such a test can be faster than the call to the \e fabs
library routine
\return the absolute value of \e val
*/
inline myReal simpleabs(const myReal &val)
{
	return val>0?val:-val;
}
//! the greek pi constant
extern const myReal G_PI;

//! greek pi / 2
extern const myReal G_HALF_PI;

//!2 * greek pi
extern const myReal G_DOUBLE_PI;

#ifdef _AFXDLL
#undef assert
//!maps the assert function to the MFC ASSERT macro
#define assert(exp) ASSERT(exp)
#endif

#ifndef HIGH_PRECISION
// this below could be MSVC compiler specific
#pragma warning( pop )// reset the warning status
#endif

#endif // _3D_MATH_H