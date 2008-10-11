#ifndef _TEMPLDEFS_H_
#define _TEMPLDEFS_H_

//FOR_EACH Macro implementation

//Boost type_of code
// Copyright (C) 2005 Igor Chesnokov, mailto:ichesnokov@gmail.com (VC 6.5,VC 7.1 + counter code)
// Copyright (C) 2005-2007 Peder Holt (VC 7.0 + framework)
// Copyright (C) 2006 Steven Watanabe (VC 8.0)

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)



#include <typeinfo>

#ifdef _WIN32_

template< int N /**/ > struct int_;

  template <bool B, class T = void>
  struct enable_if_c {
    typedef T type;
  };

  template <class T>
  struct enable_if_c<false, T> {};

  template <class Cond, class T = void> 
  struct enable_if : public enable_if_c<Cond::value, T> {};

  template <bool B, class T = void>
  struct disable_if_c {
    typedef T type;
  };

  template <class T>
  struct disable_if_c<true, T> {};

  template <class Cond, class T = void> 
  struct disable_if : public disable_if_c<Cond::value, T> {};

        //Compile time constant code
        template<int N> struct the_counter;

        template<typename T,int N = 5/*for similarity*/>
        struct encode_counter
        {
            __if_exists(the_counter<N + 256>)
            {
                static const unsigned count=(encode_counter<T,N + 257>::count);
            }
            __if_not_exists(the_counter<N + 256>)
            {
                __if_exists(the_counter<N + 64>)
                {
                    static const unsigned count=(encode_counter<T,N + 65>::count);
                }
                __if_not_exists(the_counter<N + 64>)
                {
                    __if_exists(the_counter<N + 16>)
                    {
                        static const unsigned count=(encode_counter<T,N + 17>::count);
                    }
                    __if_not_exists(the_counter<N + 16>)
                    {
                        __if_exists(the_counter<N + 4>)
                        {
                            static const unsigned count=(encode_counter<T,N + 5>::count);
                        }
                        __if_not_exists(the_counter<N + 4>)
                        {
                            __if_exists(the_counter<N>)
                            {
                                static const unsigned count=(encode_counter<T,N + 1>::count);
                            }
                            __if_not_exists(the_counter<N>)
                            {
                                static const unsigned count=N;
                                typedef the_counter<N> type;
                            }
                        }
                    }
                }
            }
        };

# define BOOST_TYPEOF_INDEX(T) (encode_counter<T>::count)
# define BOOST_TYPEOF_NEXT_INDEX(next)


        //Typeof code



        struct msvc_extract_type_default_param {};

        template<typename ID, typename T = msvc_extract_type_default_param>
        struct msvc_extract_type;

        template<typename ID>
        struct msvc_extract_type<ID, msvc_extract_type_default_param> {
            template<bool>
            struct id2type_impl;

            typedef id2type_impl<true> id2type;
        };

        template<typename ID, typename T>
        struct msvc_extract_type : msvc_extract_type<ID,msvc_extract_type_default_param>
        {
            template<>
            struct id2type_impl<true>  //VC8.0 specific bugfeature
            {
                typedef T type;
            };
            template<bool>
            struct id2type_impl;

            typedef id2type_impl<true> id2type;
        };

        template<typename T, typename ID>
        struct msvc_register_type : msvc_extract_type<ID, T>
        {
        };

        template<int ID>
        struct msvc_typeid_wrapper {
            typedef typename msvc_extract_type<int_<ID> >::id2type id2type;
            typedef typename id2type::type type;
        };
        //Workaround for ETI-bug for VC6 and VC7
        template<>
        struct msvc_typeid_wrapper<1> {
            typedef msvc_typeid_wrapper<1> type;
        };
        //Workaround for ETI-bug for VC7.1
        template<>
        struct msvc_typeid_wrapper<4> {
            typedef msvc_typeid_wrapper<4> type;
        };

        //Tie it all together
        template<typename T>
        struct encode_type
        {
            //Get the next available compile time constants index
            static const unsigned value=BOOST_TYPEOF_INDEX(T);
            //Instantiate the template
            typedef typename msvc_register_type<T,int_<value> >::id2type type;
            //Set the next compile time constants index
            static const unsigned next=value+1;
            //Increment the compile time constant (only needed when extensions are not active
            BOOST_TYPEOF_NEXT_INDEX(next);
        };

        template<class T>
        struct sizer
        {
            typedef char(*type)[encode_type<T>::value];
        };
        //template<typename T> typename disable_if<
        //    false/*typename is_function<T>::type*/,
        //    typename sizer<T>::type>::type encode_start(T const&);

        //template<typename T> typename enable_if<
        //    false/*typename is_function<T>::type*/,
        //    typename sizer<T>::type>::type encode_start(T&);

		template<typename T> typename sizer<T>::type encode_start(T&);

        template<typename Organizer, typename T>
        msvc_register_type<T,Organizer> typeof_register_type(const T&,Organizer* =0);

# define typeof(expr) \
    msvc_typeid_wrapper<sizeof(*encode_start(expr))>::type
#else
#endif
//End Boost code

//template <typename t> 
//ForEachGetType<t> ForEachGetTypeCreate(t& v) {return ForEachGetType<t>();}

//#define FOR_EACH(var, lst) \
//	for((GetType(lst)::type)::iterator For_Each_Temp_It_##var = lst.begin(), For_Each_Temp_It_##var::value_type var = (*For_Each_Temp_It_##var); For_Each_Temp_It_##var != lst.end; For_Each_Temp_It_##var++, var = (*For_Each_Temp_It_##var))


#define FOR_EACH(var, lst) \
	for(typeof(lst)::iterator var = (lst).begin(); \
	var != (lst).end(); \
	var++)



#endif _TEMPLDEFS_H_
