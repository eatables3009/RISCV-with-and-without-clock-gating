#ifndef CPU_OUTPUT_DT_SC_WRAPPER_TYPE
#define CPU_OUTPUT_DT_SC_WRAPPER_TYPE 1

#include "cynw_memory.h"

struct CPU_OUTPUT_DT
{
    //
    // Member declarations.
    //
    sc_uint< 16 > out1;

    typedef sc_uint< 16 > raw_type;

    //
    // Default constructor.
    //
    CPU_OUTPUT_DT()
    {
    }

    //
    // Copy constructor.
    //
    CPU_OUTPUT_DT( const CPU_OUTPUT_DT& other )
    {
        out1 = other.out1;
    }

    //
    // Comparison operator.
    //
    inline bool operator == ( const CPU_OUTPUT_DT& other )
    {
        if ( !(out1 == other.out1) )
            return false;
        return true;
    }

    //
    // Assignment operator from CPU_OUTPUT_DT.
    //
    inline CPU_OUTPUT_DT& operator = ( const CPU_OUTPUT_DT& other )
    {
        out1 = other.out1;
        return *this;
    }

};
//
// sc_trace function.
//
inline void sc_trace( sc_trace_file* tf, const CPU_OUTPUT_DT& object, const std::string& in_name )
{
    if (tf)
    {
        tf->trace( object.out1, in_name + std::string(".out1"));
    }
}

//
// stream operator.
//
inline ostream & operator << ( ostream & os, const CPU_OUTPUT_DT& object )
{
#ifndef STRATUS_HLS
    os << "(";
    os <<  object.out1;
    os << ")";
#endif
    return os;
}

//
// cynw_interpret function to generate a flat vector.
//
inline void cynw_interpret ( CPU_OUTPUT_DT& from, CPU_OUTPUT_DT::raw_type& to )
{
    to = (
                from.out1
                );
}

//
// cynw_interpret function to generate a CPU_OUTPUT_DT from a flat vector.
//
inline void cynw_interpret ( const CPU_OUTPUT_DT::raw_type& from, CPU_OUTPUT_DT& to )
{
    cynw_interpret( (sc_uint<16>)from.range( 15,0 ), to.out1 );
}


template <>
struct cynw_sc_wrap< CPU_OUTPUT_DT >
{
    typedef CPU_OUTPUT_DT spec;
    typedef CPU_OUTPUT_DT sc;
};
#endif

