#pragma once

namespace Random{

	class xor128{
	public:
		xor128();
		xor128(unsigned s){ w = s; }  // �^����ꂽ�V�[�h�ŏ�����

		unsigned random();
		float random(float min, float max);
		int random(int min, int max);
	private:
		unsigned x = 123456789u, y = 362436069u, z = 521288629u, w;
	
	};
}