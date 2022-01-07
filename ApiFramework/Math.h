#pragma once
#include <Windows.h>
#include <stdlib.h>

class Math
{

public:
	static float Lerp(float a, float b, float alpha) {
		return a + ((a - b) * alpha);
	}
	//static bool CollisionAABB_(Quad& dest, Quad& src) {
	//	return	src.x - src.w > dest.x + dest.w || src.x + src.w > dest.x - dest.w ||
	//		src.y - src.h > dest.y + dest.h || src.y + src.h > dest.y - dest.h;
	//}

	//static bool CollisionAABB(Quad& src, Quad& dest) {
	//	if (src.x + src.w < dest.x - dest.w || src.x - src.w < dest.x + dest.w) {
	//		return false;
	//	}
	//	if (src.y + src.h < dest.y - dest.h || src.y - src.h < dest.y + dest.h) {
	//		return false;
	//	}
	//	return true;
	//}

	static bool CollisionAABB___(Quad& src, Quad& dest) {
		if (src.x + src.w > dest.x - dest.w && dest.x + dest.x > src.x - src.w) {
			if (src.y + src.h > dest.y - dest.h && dest.y + dest.h > src.y - src.h) {
				return true;
			}
		}
		return false;
	}

	static bool CollisionAABB(Quad& src, Quad& dest) {
		if (src.x - src.w > dest.x + dest.w) {
			return false;
		}
		else if (src.x + src.w < dest.x - dest.w) {
			return false;
		}

		if (src.y - src.h > dest.y + dest.h) {
			return false;
		}
		else if (src.y + src.h < dest.y - dest.h) {
			return false;
		}
		return true;
	}

	//static bool CollisionAABB(Quad& src, Quad& dest) {
	//	if (src.x - src.w <= dest.x - dest.w && dest.x - dest.w <= src.x + src.w) {
	//		if (src.y - src.h <= dest.y - dest.h && dest.y - dest.h <= src.y + src.h) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}
};

// WELL ������� ���� ���� ������
// ���� : ������ �׳��ϱ⿡�� �ɽ��ϰ� ������ ������ ���� ���°� �� ���ٰ� ����� ����
// �ı� : ���� ���θ� ������...
/*------------------
*		Random
------------------*/
class Random
{
private:
	enum {
		DIFFER_VALUE = 100,
	};
public:
	Random() { Init(); }

	void Init() {
		index = 0;
		uint64 seed = static_cast<uint64>(time(nullptr));
		for (int32 i = 0; i < 16; ++i) {
			state[i] = seed;
			seed += seed + DIFFER_VALUE;
		}
	}

	void Init(uint64 uSeed) {
		index = 0;
		uint64 seed = uSeed;
		for (int32 i = 0; i < 16; ++i) {
			state[i] = seed;
			seed += seed + DIFFER_VALUE;
		}
	}

public:
	uint64 Value(void) {
		uint64 a = state[index];
		uint64 c = state[(index + 13) & 15];
		uint64 b = a ^ c ^ (a << 16) ^ (c << 15);
		c = state[(index + 9) & 15];
		c ^= (c >> 11);
		a = state[index] = b ^ c;
		uint64 d = a ^ ((a << 5) & 0xda442d24U);
		index = (index + 15) & 15;
		a = state[index];
		state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return state[index];
	}

	uint64 GetValue(uint64 min, uint64 max) {
		return min + (Value() % (max - min));
	}
	float GetFloatValue() {
		uint64 value = Value();
		// ����ü�� �̿��ؼ� float�� �ٲ��ش�.
		union { uint64 u64; float f; } unit;
		unit.u64 = (((value *= 16807) & 0x007fffff) - 1) | 0x3f800000;
		return unit.f - 1.0f;
	}
	float GetFloatValue(float min, float max) {
		return min + GetFloatValue() * (max - min);
	}

private:

	uint64 index = 0;
	uint64 state[16] = { 0 };
};