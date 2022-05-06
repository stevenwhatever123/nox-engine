#pragma once

template <typename Type>
class Singleton {
	public:
		static Type* Instance() {
			static Type* inst = new Type;
			return inst;
		}
	protected:
		Singleton(){};
		virtual ~Singleton() {};
	private:
		Singleton(const Singleton &other);
};
