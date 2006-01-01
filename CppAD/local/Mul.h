# ifndef CppADMulIncluded
# define CppADMulIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
-------------------------------------------------------------------------------
$begin Mul$$
$spell
	Var
	const
	Mul
$$


$index binary, multiply$$
$index multiply, binary$$

$index binary, *$$
$index *, binary$$

$section The AD Binary Multiplication Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax%%left% * %right%$$
$tend

$fend 20$$

$head Description$$
Suppose that either $italic left$$ or $italic right$$ is an
$syntax%AD<%Base%>%$$ object, 
and the other operand is an $code int$$, $italic Base$$
or $syntax%AD<%Base%>%$$ object.
In this case
$syntax%
	%left% * %right%
%$$
returns an
$syntax%AD<%Base%>%$$ object
where $code *$$ has the same interpretation as
for the $italic Base$$ type.

$head Operands$$
Note the value of the left and right operands are not changed
by this operation.
If one of the operands has value zero and is a
$xref/glossary/Parameter/parameter/$$ or a $italic Base$$ operand,
the result is a parameter.
Otherwise,
if the left or right operand is a variable,
the result is a 
$xref/glossary/Variable/variable/$$.

$head Assumptions$$

If the $code *$$ operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must be a
$xref/BinaryOp//Base type binary operator/$$
for the base type $italic Base$$.

In addition,
if $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
with the same range dimension,
$latex \[
	\D{[ f(x) * g(x) ]}{x} = g(x)*\D{f(x)}{x} + f(x)*\D{g(x)}{x}
\] $$


$head Example$$
$children%
	Example/Mul.cpp
%$$
The file
$xref/Mul.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// Multiplication operators: assume CppAD.h has already been included
template <class Base>
AD<Base> AD<Base>::operator *(const AD<Base> &right) const
{
	AD<Base> result;
	CppADUnknownError( Parameter(result) );

	result.value  = value * right.value;

	if( Variable(*this) )
	{	if( Variable(right) )
		{	// result = variable * variable
			Tape()->RecordOp(
				MulvvOp, 
				result, 
				taddr, 
				right.taddr
			);
		}
		else if( IdenticalZero(right.value) )
		{	// result = variable * 0
		}
		else if( IdenticalOne(right.value) )
		{	// result = variable * 1
			result.MakeVariable(	
				taddr
			);
		}
		else
		{	// result = variable * parameter
			Tape()->RecordOp(
				MulvpOp, 
				result, 
				taddr, 
				right.value
			);
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value)  )
		{	// result = 0 * variable
		}
		else if( IdenticalOne(value) )
		{	// result = 1 * variable
			result.MakeVariable(
				right.taddr
			);
		}
		else
		{	// result = parameter * variable
			Tape()->RecordOp(
				MulpvOp, 
				result, 
				value, 
				right.taddr
			);
		}
	}

	return result;
}

CppADFoldBinaryOperator(AD<Base>, *)


} // END CppAD namespace

# endif 
