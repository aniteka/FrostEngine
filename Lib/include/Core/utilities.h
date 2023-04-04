#pragma once
#include <stdexcept>
#include <valarray>

namespace core
{
	template<typename T>
	struct vector2base
	{
		T x, y;

		T* data() { return reinterpret_cast<T*>(this); }

		friend vector2base& operator+=(vector2base& self, const vector2base& other)
		{
			std::tie(self.x, self.y) += std::tie(other.x, other.y);
			return self;
		}
		friend vector2base& operator-=(vector2base& self, const vector2base& other)
		{
			std::tie(self.x, self.y) -= std::tie(other.x, other.y);
			return self;
		}
		friend vector2base& operator*=(vector2base& self, const vector2base& other)
		{
			std::tie(self.x, self.y) *= std::tie(other.x, other.y);
			return self;
		}
		friend vector2base& operator/=(vector2base& self, const vector2base& other)
		{
			std::tie(self.x, self.y) /= std::tie(other.x, other.y);
			return self;
		}

		friend vector2base operator+(const vector2base& self, const vector2base& other)
		{
			vector2base self1(self);
			return self1 += other;
		}
		friend vector2base operator-(const vector2base& self, const vector2base& other)
		{
			vector2base self1(self);
			return self1 -= other;
		}
		friend vector2base operator*(const vector2base& self, const vector2base& other)
		{
			vector2base self1(self);
			return self1 *= other;
		}
		friend vector2base operator/(const vector2base& self, const vector2base& other)
		{
			vector2base self1(self);
			return self1 /= other;
		}
	};

	template<typename T>
	struct vector3base
	{
		T x, y, z;

		T* data() { return reinterpret_cast<T*>(this); }

		friend vector3base& operator+=(vector3base& self, const vector3base& other)
		{
			std::tie(self.x, self.y, self.z) += std::tie(other.x, other.y, other.z);
			return self;
		}
		friend vector3base& operator-=(vector3base& self, const vector3base& other)
		{
			std::tie(self.x, self.y, self.z) -= std::tie(other.x, other.y, other.z);
			return self;
		}
		friend vector3base& operator*=(vector3base& self, const vector3base& other)
		{
			std::tie(self.x, self.y, self.z) *= std::tie(other.x, other.y, other.z);
			return self;
		}
		friend vector3base& operator/=(vector3base& self, const vector3base& other)
		{
			std::tie(self.x, self.y, self.z) /= std::tie(other.x, other.y, other.z);
			return self;
		}

		friend vector3base operator+(const vector3base& self, const vector3base& other)
		{
			vector3base self1(self);
			return self1 += other;
		}
		friend vector3base operator-(const vector3base& self, const vector3base& other)
		{
			vector3base self1(self);
			return self1 -= other;
		}
		friend vector3base operator*(const vector3base& self, const vector3base& other)
		{
			vector3base self1(self);
			return self1 *= other;
		}
		friend vector3base operator/(const vector3base& self, const vector3base& other)
		{
			vector3base self1(self);
			return self1 /= other;
		}
	};

	template<typename T>
	struct vector4base
	{
		T x, y, z, w;

		T* data() { return reinterpret_cast<T*>(this); }

		friend vector4base& operator+=(vector4base& self, const vector4base& other)
		{
			std::tie(self.x, self.y, self.z, self.w) += std::tie(other.x, other.y, other.z, other.w);
			return self;
		}
		friend vector4base& operator-=(vector4base& self, const vector4base& other)
		{
			std::tie(self.x, self.y, self.z, self.w) -= std::tie(other.x, other.y, other.z, other.w);
			return self;
		}
		friend vector4base& operator*=(vector4base& self, const vector4base& other)
		{
			std::tie(self.x, self.y, self.z, self.w) *= std::tie(other.x, other.y, other.z, other.w);
			return self;
		}
		friend vector4base& operator/=(vector4base& self, const vector4base& other)
		{
			std::tie(self.x, self.y, self.z, self.w) /= std::tie(other.x, other.y, other.z, other.w);
			return self;
		}

		friend vector4base operator+(const vector4base& self, const vector4base& other)
		{
			vector4base self1(self);
			return self1 += other;
		}
		friend vector4base operator-(const vector4base& self, const vector4base& other)
		{
			vector4base self1(self);
			return self1 -= other;
		}
		friend vector4base operator*(const vector4base& self, const vector4base& other)
		{
			vector4base self1(self);
			return self1 *= other;
		}
		friend vector4base operator/(const vector4base& self, const vector4base& other)
		{
			vector4base self1(self);
			return self1 /= other;
		}
	};

	using vector2f = vector2base<float>;
	using vector2i = vector2base<int>;
	using vector3i = vector3base<int>;
	using vector3f = vector3base<float>;

	using color_t = vector4base<float>;
}

