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
}