#ifndef NOX_SINGLETON_H
#define NOX_SINGLETON_H


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

#endif// NOX_SINGLETON
