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
#include <random>

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
    return std::move(result);
}

//template < typename T > inline std::vector< std::vector<T> > merge( const std::vector< std::vector <T> > & vector_t, const std::vector<T> & vector_u );

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

template < typename T >
std::vector < std::vector< T > >
trans( const std::vector < std::vector< T > > & vv )
{
    const auto vv_sp = size( vv );
    std::vector < std::vector< T > > res( vv_sp.second );
    for ( std::size_t i = 0; i < res.size(); ++ i ) {
        res[ i ].resize( vv_sp.first );
        for ( std::size_t j = 0; j < res[ i ].size(); ++ j ) {
            res[ i ][ j ] = vv[ j ] [ i ];
        }
    }

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


template < typename T, typename F >
inline
std::vector< std::vector< T > >
feval( F op, const std::vector< std::vector< T > > & arg1 )
{
    const auto sp = size( arg1 );
    std::vector< std::vector< T > > res = arg1;
    for ( std::size_t i = 0; i < sp.first; ++ i ) {
        std::for_each( res[ i ].begin(), res[ i ].end(), op );
    }
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
std::vector< T >
operator + ( const T num, const std::vector< T > & v )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t = num + t; } );
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
std::vector< std::vector < T > >
operator + ( const std::vector< std::vector < T > > & vv, const std::vector< std::vector < T > > & vv1 )
{
    const auto vv_sp = size( vv );
    const auto vv_sp_1 = size( vv1 );
    assert( vv_sp == vv_sp_1 );
    std::vector< std::vector < T > > res = vv;
    for ( std::size_t i = 0; i < vv_sp.first; ++ i ) {
        for ( std::size_t j = 0; j < vv_sp.second; ++ j ){
            res[ i ][ j ] += vv1[ i ][ j ];
        }
    }
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
std::vector< T >
operator - ( const T num, const std::vector< T > & v )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t = num - t; } );
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
operator - ( const std::vector< T >& v1, const std::vector< T >& v2 )
{
    assert( v1.size() == v2.size() );
    std::vector< T > minus_res( v1.size() );
    std::transform( v1.cbegin(), v1.cend(), v2.cbegin(), minus_res.begin(), std::minus< T >() );
    return std::move( minus_res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator - ( const std::vector< std::vector < T > > & vv, const std::vector< std::vector < T > > & vv1 )
{
    const auto vv_sp = size( vv );
    const auto vv_sp_1 = size( vv1 );
    assert( vv_sp == vv_sp_1 );
    std::vector< std::vector < T > > res = vv;
    for ( std::size_t i = 0; i < vv_sp.first; ++ i ) {
        for ( std::size_t j = 0; j < vv_sp.second; ++ j ){
            res[ i ][ j ] -= vv1[ i ][ j ];
        }
    }
    return std::move( res );
}

template< typename T >
inline
std::vector< T >
operator + ( const std::vector< T >& v1, const std::vector< T >& v2 )
{
    assert( v1.size() == v2.size() );
    std::vector< T > plus_res( v1.size() );
    std::transform( v1.cbegin(), v1.cend(), v2.cbegin(), plus_res.begin(), std::plus< T >() );
    return std::move( plus_res );
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
std::vector< T >
operator * ( const T num, const std::vector< T > & v )
{
    std::vector< T > res = v;
    std::for_each( res.begin(), res.end(), [&]( T & t ) { t = num * t; } );
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
std::vector < T >
operator * ( const std::vector < T > & v,  const std::vector< std::vector < T > > & vv )
{
    const auto vv_sp = size( vv );
    assert( v.size() == vv_sp.first );
    std::vector< T > res( vv_sp.second );
    for ( std::size_t i = 0; i < res.size(); ++ i ) {
        for ( std::size_t j = 0; j < vv_sp.first; ++ j ) {
            res[ i ] += v[ j ] * vv[ j ] [ j ];
        }
    }

    return std::move( res );
}


template< typename T >
inline
std::vector< std::vector < T > >
operator * ( const std::vector< std::vector < T > > & vv, const std::vector< T > & v )
{
    const auto vv_size_pair = size( vv );
    std::vector< std::vector < T > > res( vv_size_pair.first );
    for ( std::size_t i = 0; i < res.size(); ++ i ) {
        std::vector< T > v1 = v;
        std::for_each( v1.begin(), v1.end(), [&]( double & val ) {
            val = val * vv[ i ][ 0 ];
        } );
        res[ i ] = std::move( v1 );
    }
    return std::move( res );
}

template< typename T >
inline
std::vector< std::vector < T > >
operator * ( const std::vector< std::vector < T > > & vv, const std::vector< std::vector < T > > & vv1 )
{
    const auto vv_sp = size( vv );
    const auto vv1_sp = size( vv1 );

    assert( vv_sp.second == vv1_sp.first );

    std::vector< std::vector < T > > res( vv_sp.first );
    for ( std::size_t i = 0; i < res.size(); ++ i ) {
        res[ i ].resize( vv1_sp.second );
        for ( std::size_t j = 0; j < res[ i ].size(); ++ j ) {
            for ( std::size_t k = 0; k < vv_sp.second; ++ k ) {
                res[ i ][ j ] += vv[ i ][ k ] * vv1[ k ][ j ];
            }
        }
    }

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


template< typename T, typename I = uint32_t >
inline
std::vector< T >
operator ^ ( const std::vector< T > & v, const I power )
{
    std::vector< T > res( v.size() );
    std::transform( v.cbegin(), v.cend(), res.begin(), [&]( const double & v ) { return std::pow( v, power ); } );
    return std::move( res );
}

template < typename T >
std::vector< std::vector< T > >
vec_to_vecvec( const std::vector< T > & v )
{
    std::vector< std::vector< T > > res( v.size() );
    for ( std::size_t i = 0; i < res.size(); ++ i ) {
        std::vector< T > vt( 1 );
        vt[ 0 ] = v[ i ];
        res[ i ] = std::move( vt );
    }
    return std::move( res );
}


template< typename T = double >
inline
std::vector< T >
ones ( const std::size_t n )
{
    std::vector< T > ones_vector( n );
    std::fill( ones_vector.begin(), ones_vector.end(), 1.0 );
    return std::move( ones_vector );
}

template< typename T = double >
inline
std::vector< std::vector < T >  >
ones ( const std::size_t n, const std::size_t n1 )
{
    std::vector < std::vector< T > > ones_vector( n );
    std::for_each( ones_vector.begin(), ones_vector.end(), [&]( std::vector< T > & v ) {
        v.resize( n1 );
        std::fill( v.begin(), v.end(), 1.0 );
    } );
    return std::move( ones_vector );
}

template< typename T = double >
inline
std::vector< T >
zeros ( const std::size_t n )
{
    std::vector< T > ones_vector( n );
    return std::move( ones_vector );
}

template< typename T = double >
inline
std::vector< std::vector < T >  >
zeros ( const std::size_t n, const std::size_t n1 )
{
    std::vector < std::vector< T > > ones_vector( n );
    std::for_each( ones_vector.begin(), ones_vector.end(), [&]( std::vector< T > & v ) {
        v.resize( n1 );
    } );
    return std::move( ones_vector );
}


// TODO rand
double randd()
{
#ifndef FAKE_RAND
    return rand_range( 0.0, 1.0);
#else
    static const double a[] = { 0.63358600,    0.83480100,    0.23745400,    0.52321100,    0.85529400,    0.49205700,    0.78515400,    0.06630500,    0.10606000,    0.87117500,    0.26862500,    0.56327400,    0.82896800,    0.93081200,    0.28282200,    0.10661300,    0.76146600,    0.00686100,    0.13602700,    0.70438700,    0.23325000,    0.90437700,    0.85558400,    0.88981400,    0.74716000,    0.92519400,    0.20502000,    0.47298900,    0.19015200,    0.30558800,    0.05330500,    0.82373800,    0.14038900,    0.29075900,    0.34694900,    0.99568300,    0.78281600,    0.13210300,    0.06198800,    0.88887600,    0.00327800,    0.33061300,    0.45215000,    0.83224500,    0.26142500,    0.73497200,    0.93885800,    0.02289000,    0.74183400,    0.07488400,    0.72727700,    0.97508300,    0.97926200,    0.58286100,    0.86489700,    0.72642100,    0.50805500,    0.06991800,    0.19941000,    0.69820700,    0.37550600,    0.25271500,    0.52194500,    0.51589500,    0.54347400,    0.86889400,    0.51157800,    0.32629100,    0.00099600,    0.57356600,    0.21516600,    0.00427400,    0.90417900,    0.66731600,    0.83651900,    0.16560300,    0.40228800,    0.77537700,    0.18849400,    0.14412200,    0.85026100,    0.91577100,    0.11920500,    0.82952300,    0.49863200,    0.98410300,    0.55594400,    0.00668700,    0.05402000,    0.75535400,    0.70489400,    0.42952600,    0.00807000,    0.22683900,    0.94542100,    0.55154400,    0.09573300,    0.45699900,    0.87783500,    0.09672900 };
    static std::size_t i = 0;
    return a[i++];
#endif
}

// TODO dot product
//Use dot_operator for that

template< typename T >
inline
T
operator * ( const std::vector< T >& v1, const std::vector< T >& v2 )
{
    T res = std::inner_product(v1.cbegin(), v1.cend(), v2.cbegin(), 0.0, std::plus< T >(), std::multiplies< T >() );
    return res;
}


template< typename T = double >
inline
std::vector< T >
randn( const std::size_t n )
{
#ifndef FAKE_RAND
    std::vector< T > randn_vector( n );
    //std::default_random_engine generator;
    std::linear_congruential_engine< std::size_t, 3571 , 3559 , RAND_MAX  >  generator(time(NULL));
    std::normal_distribution< T > distribution;
    std::generate( randn_vector.begin(), randn_vector.end(), [&](){
        return distribution(generator);
    } );
    return std::move( randn_vector );
#else
    std::vector< T > randn_vector = { -0.0621970,    -0.1586820,    0.01842900,    -0.1575190,    -0.0466730,    0.02448400,    -0.0466390,    -0.1026640,    -0.0037470,    -0.0299300,    0.16246800,    -0.0128280,    0.05209600,    0.02834100,    -0.0627840,    -0.0836810,    -0.1187150,    0.08084200,    -0.1435840,    -0.3172370,    -0.1272740,    -0.1021570,    -0.2515290,    -0.0449630,    -0.1394620,    -0.0513170,    -0.0684680,    -0.1179450,    -0.0573740,    0.03582500 };
    return randn_vector;
#endif
}

template< typename T = double >
inline
std::vector< std::vector< T > >
randn( const std::size_t n , const std::size_t m)
{
#ifndef FAKE_RAND
    std::vector< std::vector< T > > randn_vector(n);
    //std::default_random_engine generator;
    std::linear_congruential_engine< std::size_t, 3571 , 3559 , RAND_MAX > generator(time(NULL));
    std::normal_distribution< T > distribution;
    std::for_each( randn_vector.begin(), randn_vector.end(), [&]( std::vector< T >& val){
        val.resize( m );
        std::generate( val.begin(), val.end(), [&](){
            return distribution(generator);
        } );
    } );
    return std::move( randn_vector );
#else
    std::vector< std::vector< T > > randn_vector = {
            { -0.0621970,    -0.1586820,    0.01842900,    -0.1575190,    -0.0466730,    0.02448400,    -0.0466390,    -0.1026640,    -0.0037470,    -0.0299300,    0.16246800,    -0.0128280,    0.05209600,    0.02834100,    -0.0627840,    -0.0836810,    -0.1187150,    0.08084200,    -0.1435840,    -0.3172370,    -0.1272740,    -0.1021570,    -0.2515290,    -0.0449630,    -0.1394620,    -0.0513170,    -0.0684680,    -0.1179450,    -0.0573740,    0.03582500 },
            { -0.0123090,    -0.1249270,    0.09023100,    0.03059000,    -0.0835230,    0.01725700,    0.07115200,    -0.0453520,    -0.0785110,    0.06508700,    -0.1814280,    -0.0631080,    -0.0970960,    -0.0427730,    0.11732700,    -0.0510240,    -0.0905610,    -0.0815260,    -0.1106960,    -0.2096330,    -0.1488120,    0.02971400,    0.06047000,    -0.1825570,    -0.1471750,    0.10695700,    -0.0341260,    -0.0661900,    0.09181700,    -0.2493640 },
            { 0.00679600,    -0.0514150,    0.00156000,    -0.1697540,    0.06167500,    0.00573300,    -0.1935720,    -0.0614030,    0.04410100,    -0.0718840,    -0.0917900,    -0.1548330,    -0.0634850,    -0.1600940,    0.02461100,    -0.0582440,    -0.0404150,    -0.0566740,    -0.0481160,    0.06417400,    -0.0657850,    0.08276900,    0.06021600,    -0.0049430,    -0.0367390,    -0.0440990,    -0.0373080,    -0.2352440,    0.04906000,    -0.0726160 }
            };
    return randn_vector;
#endif
}

template< typename T = double >
inline
std::vector< T >
round( const std::vector< T >& v)
{
    std::vector< T > round_vector( v.size() );
    std::transform( v.cbegin(), v.cend(), round_vector.begin(), std::round );
    return std::move( round_vector );
}

#endif // UTILS_HPP
