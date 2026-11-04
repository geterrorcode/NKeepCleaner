#ifndef QUX_SINGLETON_H
#define QUX_SINGLETON_H

namespace Qux {

    template <typename T>
    class Singleton
    {
    public:
        static T& inst()
        {
            if (m_instance == nullptr) {
                m_instance = new T;
            }

            return *m_instance;
        }

        static const T& cinst()
        {
            return inst();
        }

        static void release()
        {
            delete m_instance;
            m_instance = nullptr;
        }

    protected:
        Singleton() {}

    private:
        Singleton(const Singleton&);
        const Singleton& operator=(const Singleton&);

        static T* m_instance;

    };

    template <typename T>
    T* Singleton<T>::m_instance = nullptr;

}

#endif