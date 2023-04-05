#pragma once
#define GENERATE_OP_1(OP, VAR1) \
	self.##VAR1 OP##= other.##VAR1
#define GENERATE_OP_2(OP, VAR1, VAR2) \
	GENERATE_OP_1(OP, VAR1); \
	GENERATE_OP_1(OP, VAR2); 
#define GENERATE_OP_3(OP, VAR1, VAR2, VAR3) \
	GENERATE_OP_1(OP, VAR1); \
	GENERATE_OP_1(OP, VAR2); \
	GENERATE_OP_1(OP, VAR3);
#define GENERATE_OP_4(OP, VAR1, VAR2, VAR3, VAR4) \
	GENERATE_OP_1(OP, VAR1); \
	GENERATE_OP_1(OP, VAR2); \
	GENERATE_OP_1(OP, VAR3); \
	GENERATE_OP_1(OP, VAR4);

#define GENERATE_OP_T_1(OP, OTHER, VAR1) \
	self.##VAR1 OP##= OTHER
#define GENERATE_OP_T_2(OP, OTHER, VAR1, VAR2) \
	GENERATE_OP_T_1(OP, OTHER, VAR1); \
	GENERATE_OP_T_1(OP, OTHER, VAR2); 
#define GENERATE_OP_T_3(OP, OTHER, VAR1, VAR2, VAR3) \
	GENERATE_OP_T_1(OP, OTHER, VAR1); \
	GENERATE_OP_T_1(OP, OTHER, VAR2); \
	GENERATE_OP_T_1(OP, OTHER, VAR3);
#define GENERATE_OP_T_4(OP, OTHER, VAR1, VAR2, VAR3, VAR4) \
	GENERATE_OP_T_1(OP, OTHER, VAR1); \
	GENERATE_OP_T_1(OP, OTHER, VAR2); \
	GENERATE_OP_T_1(OP, OTHER, VAR3); \
	GENERATE_OP_T_1(OP, OTHER, VAR4);

#define GENERATE_OPERATOR(OP, OTHER_T) \
	friend self_t operator##OP (const self_t& self, const OTHER_T& other) \
	{\
		self_t to_ret(self);\
		return to_ret OP##= other; \
	}
		

namespace core
{
	template<typename T>
	struct vector2base
	{
		using self_t = vector2base;
		T x, y;

		T* data() { return reinterpret_cast<T*>(this); }

		friend self_t& operator+=(self_t& self, const self_t& other)
		{
			GENERATE_OP_2(+, x, y);
			return self;
		}
		friend self_t& operator-=(self_t& self, const self_t& other)
		{
			GENERATE_OP_2(-, x, y);
			return self;
		}
		friend self_t& operator*=(self_t& self, const self_t& other)
		{
			GENERATE_OP_2(*, x, y);
			return self;
		}
		friend self_t& operator/=(self_t& self, const self_t& other)
		{
			GENERATE_OP_2(/, x, y);
			return self;
		}
		friend self_t& operator+=(self_t& self, const T& other)
		{
			GENERATE_OP_T_2(+, other, x, y);
			return self;
		}
		friend self_t& operator-=(self_t& self, const T& other)
		{
			GENERATE_OP_T_2(-, other, x, y);
			return self;
		}
		friend self_t& operator*=(self_t& self, const T& other)
		{
			GENERATE_OP_T_2(*, other, x, y);
			return self;
		}
		friend self_t& operator/=(self_t& self, const T& other)
		{
			GENERATE_OP_T_2(/, other, x, y);
			return self;
		}
		GENERATE_OPERATOR(+, self_t)
		GENERATE_OPERATOR(-, self_t)
		GENERATE_OPERATOR(*, self_t)
		GENERATE_OPERATOR(/, self_t)
		GENERATE_OPERATOR(+, T)
		GENERATE_OPERATOR(-, T)
		GENERATE_OPERATOR(*, T)
		GENERATE_OPERATOR(/, T)
	};

	template<typename T>
	struct vector3base
	{
		using self_t = vector3base;
		T x, y, z;

		T* data() { return reinterpret_cast<T*>(this); }

		friend self_t& operator+=(self_t& self, const self_t& other)
		{
			GENERATE_OP_3(+, x, y, z);
			return self;
		}
		friend self_t& operator-=(self_t& self, const self_t& other)
		{
			GENERATE_OP_3(-, x, y, z);
			return self;
		}
		friend self_t& operator*=(self_t& self, const self_t& other)
		{
			GENERATE_OP_3(*, x, y, z);
			return self;
		}
		friend self_t& operator/=(self_t& self, const self_t& other)
		{
			GENERATE_OP_3(/, x, y, z);
			return self;
		}
		friend self_t& operator+=(self_t& self, const T& other)
		{
			GENERATE_OP_T_3(+, other, x, y, z);
			return self;
		}
		friend self_t& operator-=(self_t& self, const T& other)
		{
			GENERATE_OP_T_3(-, other, x, y, z);
			return self;
		}
		friend self_t& operator*=(self_t& self, const T& other)
		{
			GENERATE_OP_T_3(*, other, x, y, z);
			return self;
		}
		friend self_t& operator/=(self_t& self, const T& other)
		{
			GENERATE_OP_T_3(/, other, x, y, z);
			return self;
		}
		GENERATE_OPERATOR(+, self_t)
		GENERATE_OPERATOR(-, self_t)
		GENERATE_OPERATOR(*, self_t)
		GENERATE_OPERATOR(/, self_t)
		GENERATE_OPERATOR(+, T)
		GENERATE_OPERATOR(-, T)
		GENERATE_OPERATOR(*, T)
		GENERATE_OPERATOR(/, T)
	};

	template<typename T>
	struct vector4base
	{
		using self_t = vector4base;
		T x, y, z, w;

		T* data() { return reinterpret_cast<T*>(this); }

		friend self_t& operator+=(self_t& self, const self_t& other)
		{
			GENERATE_OP_4(+, x, y, z, w);
			return self;
		}
		friend self_t& operator-=(self_t& self, const self_t& other)
		{
			GENERATE_OP_4(-, x, y, z, w);
			return self;
		}
		friend self_t& operator*=(self_t& self, const self_t& other)
		{
			GENERATE_OP_4(*, x, y, z, w);
			return self;
		}
		friend self_t& operator/=(self_t& self, const self_t& other)
		{
			GENERATE_OP_4(/, x, y, z, w);
			return self;
		}
		friend self_t& operator+=(self_t& self, const T& other)
		{
			GENERATE_OP_T_4(+, other, x, y, z, w);
			return self;
		}
		friend self_t& operator-=(self_t& self, const T& other)
		{
			GENERATE_OP_T_4(-, other, x, y, z, w);
			return self;
		}
		friend self_t& operator*=(self_t& self, const T& other)
		{
			GENERATE_OP_T_4(*, other, x, y, z, w);
			return self;
		}
		friend self_t& operator/=(self_t& self, const T& other)
		{
			GENERATE_OP_T_4(/, other, x, y, z, w);
			return self;
		}
		GENERATE_OPERATOR(+, self_t)
		GENERATE_OPERATOR(-, self_t)
		GENERATE_OPERATOR(*, self_t)
		GENERATE_OPERATOR(/, self_t)
		GENERATE_OPERATOR(+, T)
		GENERATE_OPERATOR(-, T)
		GENERATE_OPERATOR(*, T)
		GENERATE_OPERATOR(/, T)
	};

	using vector2f = vector2base<float>;
	using vector2i = vector2base<int>;
	using vector3i = vector3base<int>;
	using vector3f = vector3base<float>;

	using color_t = vector4base<float>;
}

