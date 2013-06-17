#ifndef UTILS_HPP
#define UTILS_HPP

#include <ctime>
#include <cstdlib>
#include <vector>

template< typename T > inline std::ostream & operator << ( std::ostream & os, const std::vector<T> & vector );

template < typename T, typename U >
inline
std::ostream &
operator << ( std::ostream & os, const std::pair< T, U> & p )
{
    os << "<" << std::get<0>( p ) << " : " << std::get<1>( p ) << ">";
    return os;
}

template< typename T >
inline
std::ostream &
operator << ( std::ostream & os, const std::vector<T> & vector )
{
    os << "[";
    for ( size_t i = 0; i < vector.size(); ++ i ) {
        os << vector[ i ] << ( ( (vector.size() - 1) == i ) ? "]" : ", " );
    }
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
size_t
dimension ( const std::vector<T> & vector )
{
    return vector.size();
}

template < typename T >
inline constexpr
size_t
dimension ( const T & )
{
    return 1;
}

template < typename T, typename U >
inline
std::vector< std::pair< T, U > >
merge( const std::vector<T> & vector_t, const std::vector<U> & vector_u )
{
    assert( vector_t.size() == vector_u.size() );
    std::vector< std::pair< T, U > > result( vector_t.size() );
    for ( size_t i = 0; i < result.size(); ++i ) {
        result[ i ] = std::make_pair( vector_t[ i ], vector_u[ i ] );
    }
    return result;
}

template < typename T >
inline
std::vector< std::vector<T> >
merge_to_vector( const std::vector<T> & vector_t, const std::vector<T> & vector_u )
{
    assert( vector_t.size() == vector_u.size() );
    std::vector< std::vector<T> > result( vector_t.size() );
    for ( size_t i = 0; i < result.size(); ++i ) {
        std::vector< T > tmp_res( 2 );
        tmp_res[ 0 ] = vector_t[ i ];
        tmp_res[ 1 ] = vector_u[ i ];
        result[ i ] = std::move( tmp_res );
    }
    return result;
}

template < typename T >
inline
std::tuple< std::vector<T>/*Training*/, std::vector<T>/*Testing*/ >
split( const std::vector<T> & dataset, const std::tuple< const double, const double > & distribution ) {
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


#endif // UTILS_HPP
