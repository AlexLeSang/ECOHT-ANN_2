#ifndef UTILS_HPP
#define UTILS_HPP

#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>
#include <utility>

//template< typename T > inline std::ostream & operator << ( std::ostream & os, const std::vector<T> & vector );

template< typename T, std::size_t width = 10, std::size_t precision = 6 >
inline
std::ostream &
operator << ( std::ostream & os, const std::vector<T> & vector )
{
    os << "[ ";
    for ( size_t i = 0; i < vector.size(); ++ i ) {
        os.precision( precision );
        os.width( width );
        os.fill('0');
        os << std::fixed;
        os << std::left;
        os << vector[ i ] << ( ( (vector.size() - 1) == i ) ? "" : "    " );
    }
    os << " ]";
    return os;
}

template< typename T >
inline
std::ostream &
operator << ( std::ostream & os, const std::vector< std::vector <T> > & vector )
{
    os << "[\n";
    for ( size_t i = 0; i < vector.size(); ++ i ) {
        os << vector[ i ] << ( ( (vector.size() - 1) == i ) ? "" : "\n" );
    }
    os << "\n]";
    return os;
}



template < typename T, typename U >
inline
std::ostream &
operator << ( std::ostream & os, const std::pair< T, U> & p )
{
    os << "<" << std::get<0>( p ) << " : " << std::get<1>( p ) << ">";
    return os;
}


double rand_range( const double min, const double max ) {
    static bool seeded = false;
    if ( !seeded ) {
        srand( time(NULL) );
        seeded = true;
    }
    const double r = (double)rand() / (double)RAND_MAX;
    return min + r * (max - min);
}

template < typename T >
inline constexpr
std::size_t
dimension ( const std::vector<T> & vector )
{
    return vector.size();
}

template < typename T >
inline constexpr
std::size_t
dimension ( const T & )
{
    return 1;
}

template < typename T, typename U >
inline
std::vector< std::pair< T, U > >
merge_to_pair( const std::vector<T> & vector_t, const std::vector<U> & vector_u )
{
    assert( vector_t.size() == vector_u.size() );
    std::vector< std::pair< T, U > > result( vector_t.size() );
    for ( std::size_t i = 0; i < result.size(); ++i ) {
        result[ i ] = std::make_pair( vector_t[ i ], vector_u[ i ] );
    }
    return result;
}

template < typename T >
inline
std::vector< std::vector< T > >
merge( const std::vector< T > & vector_t, const std::vector<T> & vector_u )
{
    assert( vector_t.size() == vector_u.size() );
    std::vector< std::vector< T > > result( vector_t.size() );
    for ( std::size_t i = 0; i < result.size(); ++i ) {
        std::vector< T > tmp_res( 2 );
        tmp_res[ 0 ] = vector_t[ i ];
        tmp_res[ 1 ] = vector_u[ i ];
        result[ i ] = std::move( tmp_res );
    }
    return std::move( result );
}

template < typename T >
inline
std::vector< std::vector<T> >
merge( const std::vector< std::vector <T> > & vector_t, const std::vector<T> & vector_u )
{
    assert( vector_t.size() == vector_u.size() );
    std::vector< std::vector< T > > result = vector_t;
    for ( std::size_t i = 0; i < vector_t.size(); ++ i ) {
        result[ i ].push_back( vector_u[ i ] );
    }
    return std::move( result );
}


template < typename T >
inline
std::tuple< std::vector< T >/*Training*/, std::vector<T>/*Testing*/ >
split( const std::vector< T > & dataset, const std::tuple< const double, const double > & distribution ) {
    assert( std::get<0>( distribution ) >= 0.0 );
    assert( std::get<0>( distribution ) <= 1.0 );

    assert( std::get<1>( distribution ) >= 0.0 );
    assert( std::get<1>( distribution ) <= 1.0 );

    assert( std::fabs( (std::get<0>( distribution ) + std::get<1>( distribution ) ) ) - 1.0 < 1e-8 );

    std::tuple< std::vector<T>/*Training*/, std::vector<T>/*Testing*/ > result;

    std::vector<T> & training_result = std::get<0>( result );
    std::vector<T> & testing_result = std::get<1>( result );


    const double & training_rate = std::get<0>( distribution );
    const double & testing_rate = std::get<1>( distribution );

    //        std::cerr << "training_rate: " << training_rate << std::endl;
    //        std::cerr << "testing_rate: " << testing_rate << std::endl;

    const unsigned int testing_index = std::ceil( 1.0 / testing_rate );
    const unsigned int training_index = std::ceil( 1.0 / training_rate );


    unsigned int index_for_check;
    std::vector<T> * smaller_insertion_result;
    std::vector<T> * bigger_insertion_result;

    if ( testing_rate < training_rate ) {
        index_for_check = testing_index;
        smaller_insertion_result = & testing_result;
        bigger_insertion_result = & training_result;
    }
    else {
        index_for_check = training_index;
        smaller_insertion_result = & training_result;
        bigger_insertion_result = & testing_result;
    }

    for ( unsigned int i = 0; i < dataset.size(); ++ i ) {

        const bool split_condition = ((i % index_for_check) == 0);

        if ( split_condition ) {
            smaller_insertion_result->push_back( dataset.at( i ) );
            continue;
        }

        bigger_insertion_result->push_back( dataset.at( i ) );
    }

    return std::move( result );
}


template < typename T >
inline
std::pair< std::vector< std::vector <T> >, std::vector< std::vector <T> > >
meshgrid( const std::vector<T> & xgv, const std::vector<T> & ygv )
{
    std::vector< std::vector <T> > X( ygv.size() );
    std::fill( X.begin(), X.end(), xgv );

    std::vector< std::vector <T> > Y( ygv.size() );
    for ( std::size_t i = 0; i < Y.size(); ++ i ) {
        Y[ i ].resize( xgv.size() );
        std::fill( Y[ i ].begin(), Y[ i ].end(), ygv[ i ] );
    }

    return std::move( std::make_pair( X, Y ) );
}


template < typename T >
inline
std::size_t
size( const std::vector< T > & vec )
{
    return vec.size();
}

template < typename T >
inline
std::pair < std::size_t, std::size_t >
size( const std::vector< std::vector< T > > & vec_of_vec )
{
    std::pair < std::size_t, std::size_t > size_pair;
    size_pair.first = vec_of_vec.size();
    size_pair.second = size( vec_of_vec.front() );
    return std::move( size_pair );
}


template < typename T >
inline
std::vector< T >
linspace( const T from, const T to, const std::size_t n_points )
{
    static auto generate_range = []( const double start, const double step, int & i ) {
        return start + step * i++;
    };

    std::vector< T > result( n_points );
    const T step = ( to - from )/( n_points - 1 );
    std::generate( result.begin(), result.end(), std::bind( generate_range, from, step, 0 ) );
    return std::move( result );
}

template < typename T >
inline
std::vector< T >
range( const T from, const T step, const T to )
{
    static auto generate_range = []( const double start, const double step, int & i ) {
        return start + step * i++;
    };

    const std::size_t n_points = (to - from)/( step ) + 1;

    std::vector< T > result( n_points );
    std::generate( result.begin(), result.end(), std::bind( generate_range, from, step, 0 ) );
    return std::move( result );
}



template < typename T, typename F >
inline
std::vector< T >
dot_operator( const std::vector< T > & v1, const std::vector< T > & v2, F op )
{
    assert( v1.size() == v2.size() );
    std::vector< T > res( v1.size() );
    std::transform( v1.cbegin(), v1.cend(), v2.cbegin(), res.begin(), op );
    return std::move( res );
}

template < typename T, typename F >
inline
std::vector< T >
feval( F op, const std::vector< T > & arg1 )
{
    std::vector< T > res( arg1.size() );
    std::transform( arg1.cbegin(), arg1.cend(), res.begin(), op );
    return std::move( res );
}

template < typename T >
inline
std::vector< T > colon( const std::vector< std::vector < T > > & v_of_v )
{
    const auto size_pair = size( v_of_v );
    std::vector< T > res( size_pair.first * size_pair.second );
    auto iter = res.begin();
    for ( std::size_t i = 0; i < size_pair.second; ++ i ) {
        std::for_each( v_of_v.cbegin(), v_of_v.cend(), [&]( const std::vector<T> & v ) {
            (*iter) = v[ i ];
            ++iter;
        } );
    }
    return std::move( res );
}


template < typename T >
inline
std::vector< T >
mean( const std::vector< std::vector< T > > & vv )
{
    const auto size_pair = size( vv );
    std::vector< T > mean_vector( size_pair.second );
    for ( std::size_t i = 0; i < size_pair.second; ++ i ) {
        std::for_each( vv.cbegin(), vv.cend(), [&]( const std::vector< T > & v ) {
            mean_vector[ i ] += v[ i ];
        } );
    }
    std::for_each( mean_vector.begin(), mean_vector.end(), [&]( T & t ) { t /= size_pair.first; } );

    return std::move( mean_vector );
}

template < typename T >
inline
T
mean( const std::vector< T > & v )
{
    return (T) ( std::accumulate( v.cbegin(), v.cend(), 0.0 ) / v.size() );
}

template < typename T >
inline
std::vector< T >
stand( const std::vector< std::vector< T > > & vv )
{

    const auto mean_vector = mean( vv );
    const auto size_pair = size( vv );
    std::vector< T > stand_vector( size_pair.second );
    for ( std::size_t i = 0; i < size_pair.second; ++ i ) {
        std::for_each( vv.cbegin(), vv.cend(), [&]( const std::vector< T > & v ) {
            stand_vector[ i ] += std::pow( (v[ i ] - mean_vector[ i ] ), 2.0 );
        } );
    }
    std::for_each( stand_vector.begin(), stand_vector.end(), [&]( T & t ) {
        t /= (size_pair.first - 1);
        t = std::sqrt( t );
    } );

    return std::move( stand_vector );
}

template < typename T >
inline
T
stand( const std::vector< T > & v )
{
    const auto mean_v = mean( v );
    T sum = 0.0;
    std::for_each( v.cbegin(), v.cend(), [&]( const T & t ) {
        sum += std::pow( (t - mean_v), 2.0 );
    } );
    sum /= (v.size() - 1);
    sum = std::sqrt( sum );
    return sum;
}

template< typename T >
inline
std::vector< T >
operator + ( const std::vector< T > & v, const T num )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t += num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator + ( const std::vector< std::vector < T > > & vv, const T num )
{
    std::vector< std::vector < T > > res = vv;
    std::for_each( res.begin(), res.end(), [&]( std::vector < T > & t ) { t = t + num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< T >
operator - ( const std::vector< T > & v, const T num )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t -= num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator - ( const std::vector< std::vector < T > > & vv, const T num )
{
    std::vector< std::vector < T > > res = vv;
    std::for_each( res.begin(), res.end(), [&]( std::vector < T > & t ) { t = t - num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< T >
operator * ( const std::vector< T > & v, const T num )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t *= num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator * ( const std::vector< std::vector < T > > & vv, const T num )
{
    std::vector< std::vector < T > > res = vv;
    std::for_each( res.begin(), res.end(), [&]( std::vector < T > & t ) { t = t * num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< T >
operator / ( const std::vector< T > & v, const T num )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t /= num; } );
    return std::move( res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator / ( const std::vector< std::vector < T > > & vv, const T num )
{
    std::vector< std::vector < T > > res = vv;
    std::for_each( res.begin(), res.end(), [&]( std::vector < T > & t ) { t = t / num; } );
    return std::move( res );
}


template< typename T = double_t >
inline
std::vector< T >
ones( const std::size_t n )
{
    std::vector< T > ones_vector( n );
    std::fill( ones_vector.begin(), ones_vector.end(), 1.0 );
    return std::move( ones_vector );
}


#endif // UTILS_HPP
