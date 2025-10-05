
#ifndef __LCS_FA_H__
#define __LCS_FA_H__

#include "module.h"
#include "bus.h"

namespace lcs
{ // Start of namespace lcs.

/// A class which encapsulates a 1-bit full-adder.
///
class FullAdder : public Module
{
public:

    /// The only meaningfull constructor for the class.
    /// A default constructor is not explicitly provided. Moreover, the one synthesized
    /// by the compiler is practically useless.
    ///
    /// \param S The single line output Bus which carries the sum.
    /// \param Cout The single line output Bus which carries the carry output.
    /// \param A The single line InputBus object which carries one of the data lines.
    /// \param B The single line InputBus object which carries the other data line.
    /// \param Cin The single line InputBus object which carries the carry input to the
    /// full-adder.
    ///
    FullAdder(const Bus<1> &S, const Bus<1> Cout, const InputBus<1> &A,
              const InputBus<1> &B, const InputBus<1> &Cin);

    /// Destructor.
    ///
    ~FullAdder();

    /// Propogates the input data line states to the output.
    /// Here, binary 1-bit addition is performed on the line states of input busses
    /// and the result is propogated to the output busses.
    virtual void propogate();

private:
    Bus<1> s, cout;
    InputBus<1> a, b, c;
};

} // End of namespace lcs.

#endif // __LCS_FA_H__
