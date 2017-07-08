/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool abs_eval(void);
extern bool get_started(void);
extern bool lp_box(void);
extern bool min_non_smo(void);
extern bool abs_min_linear(void);
extern bool qp_box(void);
extern bool qp_interior(void);
extern bool simplex_method(void);

// main program that runs all the tests
int main(void)
{	std::string group = "example/abs_norml";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	// external compiled tests
	Run( abs_eval,          "abs_eval"         );
	Run( get_started,         "get_started"        );
	Run( lp_box,              "lp_box"             );
	Run( min_non_smo,         "min_non_smo"        );
	Run( abs_min_linear,           "abs_min_linear"          );
	Run( qp_box,              "qp_box"             );
	Run( qp_interior,         "qp_interior"        );
	Run( simplex_method,      "simplex_method"     );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}