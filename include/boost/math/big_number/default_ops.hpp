///////////////////////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BIG_NUM_DEF_OPS
#define BOOST_MATH_BIG_NUM_DEF_OPS

#include <boost/math/big_number/big_number_base.hpp>

namespace boost{ namespace math{ namespace big_num_default_ops{

//
// Default versions of mixed arithmetic, these just construct a temporary
// from the arithmetic value and then do the arithmetic on that:
//
template <class T, class V>
inline typename enable_if<mpl::or_<is_arithmetic<V>, is_convertible<V, const char*>, is_same<V, std::string> > >::type 
   add(T& result, V const& v)
{
   T t;
   t = v;
   add(result, t);
}
template <class T, class V>
inline typename enable_if<mpl::or_<is_arithmetic<V>, is_convertible<V, const char*>, is_same<V, std::string> > >::type
   subtract(T& result, V const& v)
{
   T t;
   t = v;
   subtract(result, t);
}
template <class T, class V>
inline typename enable_if<mpl::or_<is_arithmetic<V>, is_convertible<V, const char*>, is_same<V, std::string> > >::type
   multiply(T& result, V const& v)
{
   T t;
   t = v;
   multiply(result, t);
}
template <class T, class V>
inline typename enable_if<mpl::or_<is_arithmetic<V>, is_convertible<V, const char*>, is_same<V, std::string> > >::type
   divide(T& result, V const& v)
{
   T t;
   t = v;
   divide(result, t);
}
template <class T, class V>
inline typename enable_if<mpl::or_<is_arithmetic<V>, is_convertible<V, const char*>, is_same<V, std::string> > >::type
   modulus(T& result, V const& v)
{
   T t;
   t = v;
   modulus(result, t);
}

template <class T>
inline bool is_same_object(const T& u, const T&v)
{  return &u == &v;  }
template <class T, class U>
inline bool is_same_object(const T& u, const U&v)
{  return false;  }

//
// Default versions of 3-arg arithmetic functions, these just forward to the 2 arg versions:
//
template <class T, class U, class V>
inline void add(T& t, const U& u, const V& v)
{
   if(is_same_object(t, v))
   {
      add(t, u);
   }
   else if(is_same_object(t, u))
   {
      add(t, v);
   }
   else
   {
      t = u;
      add(t, v);
   }
}
template <class T, class U, class V>
inline void subtract(T& t, const U& u, const V& v)
{
   if(is_same_object(t, u))
      subtract(t, v);
   else if(is_same_object(t, v))
   {
      subtract(t, u);
      t.negate();
   }
   else
   {
      t = u;
      subtract(t, v);
   }
}
template <class T, class U, class V>
inline void multiply(T& t, const U& u, const V& v)
{
   if(is_same_object(t, u))
      multiply(t, v);
   else if(is_same_object(t, v))
      multiply(t, u);
   else
   {
      t = u;
      multiply(t, v);
   }
}
template <class T, class U, class V>
inline void divide(T& t, const U& u, const V& v)
{
   if(is_same_object(t, u))
      divide(t, v);
   else if(is_same_object(t, v))
   {
      T temp = t;
      divide(temp, u, v);
      temp.swap(t);
   }
   else
   {
      t = u;
      divide(t, v);
   }
}
template <class T, class U, class V>
inline void modulus(T& t, const U& u, const V& v)
{
   if(is_same_object(t, u))
      modulus(t, v);
   else if(is_same_object(t, v))
   {
      T temp;
      modulus(temp, u, v);
      temp.swap(t);
   }
   else
   {
      t = u;
      modulus(t, v);
   }
}

//
// Functions:
//
template <class T>
void abs(T* result, const T& arg)
{
   typedef typename T::signed_types type_list;
   typedef typename mpl::front<type_list>::type front;
   *result = arg;
   if(arg.compare(front(0)) < 0)
      result->negate();
}
template <class T>
void fabs(T* result, const T& arg)
{
   typedef typename T::signed_types type_list;
   typedef typename mpl::front<type_list>::type front;
   *result = arg;
   if(arg.compare(front(0)) < 0)
      result->negate();
}

//
// These have to implemented by the backend, declared here so that our macro generated code compiles OK.
//
template <class T>
typename enable_if_c<sizeof(T) == 0>::type floor();
template <class T>
typename enable_if_c<sizeof(T) == 0>::type ceil();
template <class T>
typename enable_if_c<sizeof(T) == 0>::type trunc();
template <class T>
typename enable_if_c<sizeof(T) == 0>::type sqrt();
template <class T>
typename enable_if_c<sizeof(T) == 0>::type ldexp();
template <class T>
typename enable_if_c<sizeof(T) == 0>::type frexp();

}


template <class Backend>
class big_number;

namespace detail{

template<typename Expr>
struct big_number_exp;

}

#define UNARY_OP_FUNCTOR(func)\
namespace detail{\
template <class Backend>\
struct BOOST_JOIN(func, _funct)\
{\
   void operator()(Backend* result, const Backend& arg)const\
   {\
      using big_num_default_ops::func;\
      func(result, arg);\
   }\
};\
\
}\
\
template <class Exp>\
typename proto::result_of::make_expr<\
    proto::tag::function\
  , detail::BOOST_JOIN(func, _funct)<typename detail::backend_type<Exp>::type>\
  , detail::big_number_exp<Exp>\
>::type const \
func(const detail::big_number_exp<Exp>& arg)\
{\
    return proto::make_expr<proto::tag::function>(\
        detail::BOOST_JOIN(func, _funct)<typename detail::backend_type<Exp>::type>() \
      , arg   \
    );\
}\
template <class Backend>\
typename proto::result_of::make_expr<\
    proto::tag::function\
  , detail::BOOST_JOIN(func, _funct)<Backend>\
  , detail::big_number_exp<typename proto::terminal<big_number<Backend>*>::type>\
>::type const \
func(const big_number<Backend>& arg)\
{\
    return proto::make_expr<proto::tag::function>(\
        detail::BOOST_JOIN(func, _funct)<Backend>() \
      , static_cast<const detail::big_number_exp<typename proto::terminal<big_number<Backend>*>::type>&>(arg)   \
    );\
}

#define BINARY_OP_FUNCTOR(func)\
namespace detail{\
template <class Backend>\
struct BOOST_JOIN(func, _funct)\
{\
   template <class A2>\
   void operator()(Backend* result, const Backend& arg, const A2& a)const\
   {\
      using big_num_default_ops::func;\
      func(result, arg, a);\
   }\
};\
\
}\
\
template <class Exp, class A2>\
typename proto::result_of::make_expr<\
    proto::tag::function\
  , detail::BOOST_JOIN(func, _funct)<typename detail::backend_type<Exp>::type>\
  , detail::big_number_exp<Exp>\
  , typename proto::result_of::as_child<const A2&>::type\
>::type const \
func(const detail::big_number_exp<Exp>& arg, const A2& a)\
{\
    return proto::make_expr<proto::tag::function>(\
        detail::BOOST_JOIN(func, _funct)<typename detail::backend_type<Exp>::type>() \
      , arg, proto::as_child(a)   \
    );\
}\
template <class Backend, class A2>\
typename proto::result_of::make_expr<\
    proto::tag::function\
  , detail::BOOST_JOIN(func, _funct)<Backend>\
  , detail::big_number_exp<typename proto::terminal<big_number<Backend>*>::type>\
  , typename proto::result_of::as_child<const A2&>::type\
>::type const \
func(const big_number<Backend>& arg, const A2& a)\
{\
    return proto::make_expr<proto::tag::function>(\
        detail::BOOST_JOIN(func, _funct)<Backend>() \
      , static_cast<const detail::big_number_exp<typename proto::terminal<big_number<Backend>*>::type>&>(arg),\
      proto::as_child(a)\
    );\
}

UNARY_OP_FUNCTOR(abs)
UNARY_OP_FUNCTOR(fabs)
UNARY_OP_FUNCTOR(sqrt)
UNARY_OP_FUNCTOR(floor)
UNARY_OP_FUNCTOR(ceil)
UNARY_OP_FUNCTOR(trunc)

BINARY_OP_FUNCTOR(ldexp)
BINARY_OP_FUNCTOR(frexp)

#undef BINARY_OP_FUNCTOR
#undef UNARY_OP_FUNCTOR

}} // namespaces

#endif

