#pragma once

#include <memory>
#include <mutex>

namespace common {
    namespace ux {
        template <typename T>
        class Singleton
        {
        protected:
            Singleton() = default;
            Singleton(const Singleton<T>& src) = delete;
            Singleton<T>& operator = (const Singleton<T>& rhs) = delete;

        public:
            static T* inst()
            {
                std::call_once(_onceFlag, []() {
                    _instance.reset(new T);
                    });

                return _instance.get();
            }

        private:
            static std::unique_ptr<T> _instance;
            static std::once_flag _onceFlag;

        };

        template<typename T>
        std::unique_ptr<T> Singleton<T>::_instance;

        template<typename T>
        std::once_flag Singleton<T>::_onceFlag;
       /* template <typename T>
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
            {,
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
        */
    }
}

