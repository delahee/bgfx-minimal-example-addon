#pragma once 

#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <bx/math.h>
#include <bx/float4x4_t.h>

namespace bm {
	struct Matrix4 {
		union
		{
			float			val[16];
			bx::float4x4_t	f4x4;
		} mat;


		Matrix4() {
			setIdentity();
		};

		void setIdentity()
		{
			bx::memSet(mat.val, 0, sizeof(mat.val));
			mat.val[0] = mat.val[5] = mat.val[10] = mat.val[15] = 1.0f;
		};
	};

	struct Vec4
	{
		Vec4() = delete;

		///
		Vec4(bx::init::NoneType);

		///
		constexpr Vec4(bx::init::ZeroType);

		///
		constexpr Vec4(bx::init::IdentityType);

		///
		explicit constexpr Vec4(float _v);

		///
		constexpr Vec4(float _x, float _y, float _z, float _w);

		float x, y, z, w;
	};

	static Vec4 white{ 1,1,1,1 };
	static Vec4 magenta{ 1,0,1,1 };
	static Vec4 pink{ 0xf0/255.0,0x76/255.0,0x8b/255.0,1 };

	inline Vec4::Vec4(bx::init::NoneType)	{}

	constexpr Vec4::Vec4(bx::init::ZeroType)
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(0.0f)
	{
	}

	constexpr Vec4::Vec4(bx::init::IdentityType)
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(0.0f)
	{
	}

	constexpr Vec4::Vec4(float _v)
		: x(_v)
		, y(_v)
		, z(_v)
		, w(_v)
	{
	}

	constexpr Vec4::Vec4(float _x, float _y, float _z, float _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}

	struct Vec2	{
		Vec2() = delete;

		///
		Vec2(bx::init::NoneType);

		///
		constexpr Vec2(bx::init::ZeroType);

		///
		constexpr Vec2(bx::init::IdentityType);

		///
		explicit constexpr Vec2(float _v);

		///
		constexpr Vec2(float _x, float _y);

		float x, y;
	};

	inline Vec2::Vec2(bx::init::NoneType){}
	constexpr Vec2::Vec2(bx::init::ZeroType)
		: x(0.0f)
		, y(0.0f)
	{
	}

	constexpr Vec2::Vec2(bx::init::IdentityType)
		: x(0.0f)
		, y(0.0f)
	{
	}

	constexpr Vec2::Vec2(float _v)
		: x(_v)
		, y(_v)
	{
	}

	constexpr Vec2::Vec2(float _x, float _y )
		: x(_x)
		, y(_y)
	{
	}
}