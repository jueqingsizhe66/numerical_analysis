#pragma once

#include <functional>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace numerical_analysis{
	template <typename Arg, typename Ret, typename Func>
	// �񕪖@
	// a, b: �����l(a < b)
	// f: �֐�
	// epsilon: ���x
	inline typename std::remove_cv<Ret>::type bisection_method(
		typename std::remove_cv<Arg>::type a,
		typename std::remove_cv<Arg>::type b,
		Func f,
		const typename std::remove_cv<Ret>::type epsilon = 10E-12l)
	{int n=0;
		typedef typename std::remove_cv<Arg>::type arg_type;
		typedef typename std::remove_cv<Ret>::type ret_type;

		arg_type c((a + b) / 2);
		ret_type fa(f(a)), fc(f(c));

		for(;;){n++;
			if(fc * fa <= 0){
				b = std::move(c);
			}else{
				a = std::move(c);
				fa = f(a);
			}

			if(static_cast<ret_type>(std::abs(b - a)) < epsilon)
				break;
			
			c = (a + b) / 2;
			fc = f(c);
		}
		return std::move(c);
	}

	template <typename Arg, typename Ret, typename Func>
	// �͂��݂����@
	// a, b: �����l(a < b)
	// f: �֐�
	// epsilon: ���x
	inline typename std::remove_cv<Ret>::type regula_falsi_method(
		typename std::remove_cv<Arg>::type a,
		typename std::remove_cv<Arg>::type b,
		Func f,
		const typename std::remove_cv<Ret>::type epsilon = 10E-12l)
	{
		typedef typename std::remove_cv<Arg>::type arg_type;
		typedef typename std::remove_cv<Ret>::type ret_type;

		auto fa(f(a)), fb(f(b));

		arg_type w((fb * a - fa * b) / (fb - fa)), w_prev;
		ret_type fw(f(w));

		for(;;){
			w_prev = w;

			if(fw * fa <= 0){
				b = std::move(w);
				fb = std::move(fw);
			}else{
				a = std::move(w);
				fa = std::move(fw);
			}

			w = (fb * a - fa * b) / (fb - fa);

			if(static_cast<ret_type>(std::abs(w - w_prev)) < epsilon)
				break;

			fw = f(w);
		}

		return std::move(w);
	}

	template <typename Arg, typename Ret, typename Func, typename OnError>
	// �����@
	// x0, x1: �����l
	// f: �֐�
	// n: �v�Z��
	// epsilon: ���x
	// onerror: �v�Z�s�̎��ɌĂяo���֐�
	inline typename std::remove_cv<Ret>::type secant_method(
		typename std::remove_cv<Arg>::type x0,
		typename std::remove_cv<Arg>::type x1,
		Func f,
		OnError onerror,
		const unsigned int n = 10000,
		const typename std::remove_cv<Ret>::type epsilon = 10E-12l
		)
	{
		typedef typename std::remove_cv<Ret>::type ret_type;

		ret_type fx0(f(x0)), fx1(f(x1));

		for(unsigned int i = 0; i < n; ++i){
			x0 = x1 - fx1 * (x1 - x0) / (fx1 - fx0);

			if(static_cast<ret_type>(std::abs(x0 - x1)) < epsilon)
				return std::move(x0);

			std::swap(x0, x1);
			fx0 = std::move(fx1);
			fx1 = f(x1);
		}

		return std::move(onerror());
	}

	template <typename Arg, typename Ret, typename Func, typename Func2, typename OnError>
	// �j���[�g���@(���֐��L)
	// x0: �����l
	// f: �֐�
	// df: ���֐�
	// n: �v�Z��
	// epsilon: ���x
	// onerror: �v�Z�s�̎��ɌĂяo���֐�
	inline typename std::remove_cv<Ret>::type newton_method(
		typename std::remove_cv<Arg>::type x0,
		Func f,
		Func2 df,
		OnError onerror,
		const unsigned int n = 1000,
		const typename std::remove_cv<Ret>::type epsilon = 10E-12l
		)
	{
		typedef typename std::remove_cv<Arg>::type arg_type;
		typedef typename std::remove_cv<Ret>::type ret_type;

		arg_type x;

		for(unsigned int i = 0; i < n; ++i){
			x = x0 - f(x0) / df(x0);

			if(static_cast<ret_type>(std::abs(x - x0)) < epsilon)
				return std::move(x);

			x0 = std::move(x);
		}

		return std::move(onerror());
	}

	template <typename Arg, typename Ret, typename Func, typename OnError>
	// �j���[�g���@(���֐���)
	// x0: �����l
	// f: �֐�
	// n: �v�Z��
	// epsilon: ���x
	// onerror: �v�Z�s�̎��ɌĂяo���֐�
	inline typename std::remove_cv<Ret>::type newton_method(
		typename std::remove_cv<Arg>::type x0,
		Func f,
		OnError onerror,
		const unsigned int n = 1000,
		const typename std::remove_cv<Ret>::type epsilon = 10E-12l
		)
	{
		typedef typename std::remove_cv<Arg>::type arg_type;
		typedef typename std::remove_cv<Ret>::type ret_type;

		arg_type x;
		ret_type fx0(f(x0));

		for(unsigned int i = 0; i < n; ++i){
			x = x0 - fx0 / ((f(x0 + epsilon) - fx0) / epsilon);

			if(static_cast<ret_type>(std::abs(x - x0)) < epsilon)
				return std::move(x);

			x0 = std::move(x);
			fx0 = f(x0);
		}

		return std::move(onerror());
	}
}
