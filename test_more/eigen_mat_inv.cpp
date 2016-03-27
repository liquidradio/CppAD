// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
Test atomic_eigen_mat_inv using a non-symetric matrix

f(x) = [ x[0]   -1  ]^{-1}
       [ 2     x[1] ]

     = [ x[1]  1    ] / (x[0] * x[1] + 2)
       [ -2    x[0] ]

y[0] = x[1] / (x[0] * x[1] + 2)
y[1] = 1.0  / (x[0] * x[1] + 2)
y[2] = -2.0 / (x[0] * x[1] + 2)
y[3] = x[0] / (x[0] * x[1] + 2)
*/
# include <cppad/cppad.hpp>
# include <cppad/example/eigen_mat_inv.hpp>


bool eigen_mat_inv(void)
{
	typedef double                                            scalar;
	typedef CppAD::AD<scalar>                                 ad_scalar;
	typedef typename atomic_eigen_mat_inv<scalar>::ad_matrix  ad_matrix;
	//
	bool ok    = true;
	scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
	using CppAD::NearEqual;
	// -------------------------------------------------------------------
	// object that computes invers of a 2x2 matrix
	size_t nr  = 2;
	atomic_eigen_mat_inv<scalar> mat_inv(nr);
	// -------------------------------------------------------------------
	// declare independent variable vector x
	size_t n = 2;
	CPPAD_TESTVECTOR(ad_scalar) ad_x(n);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = ad_scalar(j);
	CppAD::Independent(ad_x);
	// -------------------------------------------------------------------
	// arg = [ x[0]  -1   ]
	//       [ 2     x[1] ]
	ad_matrix ad_arg(nr, nr);
	ad_arg(0, 0) = ad_x[0];
	ad_arg(0, 1) = ad_scalar(-1.0);
	ad_arg(1, 0) = ad_scalar(2.0);
	ad_arg(1, 1) = ad_x[1];
	// -------------------------------------------------------------------
	// use atomic operation to compute arg^{-1}
	ad_matrix ad_result = mat_inv.op(ad_arg);
	// -------------------------------------------------------------------
	// declare the dependent variable vector y
	size_t m = 4;
	CPPAD_TESTVECTOR(ad_scalar) ad_y(4);
	for(size_t i = 0; i < nr; i++)
		for(size_t j = 0; j < nr; j++)
			ad_y[ i * nr + j ] = ad_result(i, j);
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	// -------------------------------------------------------------------
	// check zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(n), y(m);
	for(size_t i = 0; i < n; i++)
		x[i] = scalar(i + 2);
	scalar dinv = 1.0 / (x[0] * x[1] + 2.0);
	y          = f.Forward(0, x);
	ok        &= NearEqual(y[0], x[1] * dinv,  eps, eps);
	ok        &= NearEqual(y[1], 1.0  * dinv,  eps, eps);
	ok        &= NearEqual(y[2], -2.0 * dinv,  eps, eps);
	ok        &= NearEqual(y[3], x[0] * dinv,  eps, eps);
	// -------------------------------------------------------------------
	// check first order forward mode
	CPPAD_TESTVECTOR(scalar) x1(n), y1(m);
	scalar dinv_x0 = - x[1] * dinv * dinv;
	x1[0] = 1.0;
	x1[1] = 0.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], x[1] * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[1], 1.0  * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[2], -2.0 * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[3], dinv + x[0] * dinv_x0, eps, eps);
	//
	scalar dinv_x1 = - x[0] * dinv * dinv;
	x1[0] = 0.0;
	x1[1] = 1.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], dinv + x[1] * dinv_x1, eps, eps);
	ok   &= NearEqual(y1[1], 1.0  * dinv_x1,        eps, eps);
	ok   &= NearEqual(y1[2], -2.0 * dinv_x1,        eps, eps);
	ok   &= NearEqual(y1[3], x[0] * dinv_x1,        eps, eps);
	// -------------------------------------------------------------------
	// check second order forward mode
	CPPAD_TESTVECTOR(scalar) x2(n), y2(m);
	scalar dinv_x1_x1 = 2.0 * x[0] * x[0] * dinv * dinv * dinv;
	x2[0] = 0.0;
	x2[1] = 0.0;
	y2    = f.Forward(2, x2);
	ok   &= NearEqual(2.0*y2[0], 2.0*dinv_x1 + x[1]*dinv_x1_x1, eps, eps);
	ok   &= NearEqual(2.0*y2[1], 1.0  * dinv_x1_x1,             eps, eps);
	ok   &= NearEqual(2.0*y2[2], -2.0 * dinv_x1_x1,             eps, eps);
	ok   &= NearEqual(2.0*y2[3], x[0] * dinv_x1_x1,             eps, eps);
	// -------------------------------------------------------------------
	return ok;
}